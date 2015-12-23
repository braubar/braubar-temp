#include <OneWire.h>
#include <LiquidCrystal.h>
#include <PID_v1.h>
#include <RCSwitch.h>
#include <TimerOne.h>

#include "config.h"
#include "ds18b20.h"

#define PHASE_UP_INT 0
#define PHASE_DOWN_INT 1

LiquidCrystal lcd(12, 11, 5, 4, 9, 8);

unsigned long now;

uint8_t currentPhase = 0;
unsigned long currentPhaseStartTime = 0;

bool heating = true;

double ist;
double soll = phases[currentPhase].temp;
double result;
PID myPID(&ist, &result, &soll, PID_P, PID_I, PID_D, DIRECT);

RCSwitch hfswitch = RCSwitch();
unsigned long windowStartTime = 0;

unsigned long TIME_SERIAL_LAST_SENT = 0;

bool switchState;

uint16_t last60temps[60];
unsigned long lastStoredTempTime = 0;
bool last60tempsinit = true;

unsigned long lastLCDRefresh = 0;

bool switchingEnabled = false;
unsigned long switchTriggerIndex = 0;

void resetPID() {
    soll = phases[currentPhase].temp;
    myPID.SetMode(MANUAL);
    result = 0;
    myPID.SetMode(AUTOMATIC);
    heating = true;
}

volatile unsigned long phaseUpLastTime, phaseDownLastTime, phaseSwitchDuration = 500;

void phaseUpPressed() {
    if (millis() - phaseUpLastTime > phaseSwitchDuration) {
        if (currentPhase < NR_PHASES - 1) {
            currentPhase++;
            resetPID();
        }
        phaseUpLastTime = millis();
    }
}

void phaseDownPressed() {
    if (millis() - phaseDownLastTime > phaseSwitchDuration) {
        if (currentPhase > 0) {
            currentPhase--;
            resetPID();
        }
        phaseDownLastTime = millis();
    }
}

void doHFSwitch() {
    if (switchingEnabled) {
        if (switchTriggerIndex % 10 == 0 && int(result / 1000) > 0) {
            hfswitch.switchOn(1, 1);
        }
        if (switchTriggerIndex % 10 == int(result / 1000) && int(result / 1000) < 10) {
            hfswitch.switchOff(1, 1);
        }
        switchTriggerIndex++;
    }
}

void setup() {
    attachInterrupt(0, phaseUpPressed, RISING);
    // soll einen pullup widerstand vorschalten fuer den interrupt 0 am pin 2
    digitalWrite(2,HIGH);
    // soll einen pullup widerstand vorschalten fuer den interrupt 1 am pin 3
    attachInterrupt(1, phaseDownPressed, RISING);
    digitalWrite(3,HIGH);

    Serial.begin(9600);

    lcd.begin(16, 2);

    myPID.SetOutputLimits(0, PID_WINDOW_SIZE);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(PID_SAMPLE_TIME);
    windowStartTime = millis();

    hfswitch.enableTransmit(7);
    Timer1.initialize(SwITCH_WINDOW_SIZE);
    Timer1.attachInterrupt(doHFSwitch);
}

double calcDeltaT() {
    if (last60temps[59] <= last60temps[0]) return 0;
    return double(last60temps[59] - last60temps[0]) /
           100; // /100 weil, eigentlich ein double mit 2 nachkommastellen und besser vom microcontroller verarbeitet wird.
}

void printLCD() {
    if (millis() - lastLCDRefresh > LCD_REFRESH) {
        lastLCDRefresh = millis();
        lcd.clear();
        lcd.print("i:");
        lcd.print(ist);
        lcd.print((char) 223);
        lcd.print("C");
        lcd.setCursor(0, 1);
        lcd.print("s:");
        lcd.print(soll);
        lcd.print((char) 223);
        lcd.print("C");

        lcd.setCursor(10, 0);
        if (heating) {
            lcd.print(calcDeltaT());
        } else {
            int secs = (currentPhaseStartTime / 1000 + phases[currentPhase].duration * 60 - millis() / 1000);
            if (secs < 0) {
                secs = secs * (-1);
            }
            lcd.print(int(secs / 60));
            lcd.print(":");
            lcd.print(secs % 60);
        }

        lcd.setCursor(10, 1);
        lcd.print(result / 1000);
    }
}

// fuer protokoll auf laptop... 
// strings ueber serial
void printSerial() {
    now = millis();
    if ((now - TIME_SERIAL_LAST_SENT) > TIME_SERIAL) {
        TIME_SERIAL_LAST_SENT = now;

        //Serial.println(ist);
        //Serial.println(soll);

        // protokoll ueberlegen mit syncbyte und check
        char buff[7];
        buff[0] = uint16_t(ist * 100) & 0x00FF;
        buff[1] = (uint16_t(ist * 100) >> 8) & 0x00FF;
        buff[2] = uint16_t(soll * 100) & 0x00FF;
        buff[3] = (uint16_t(soll * 100) >> 8) & 0x00FF;
        // heating
        uint16_t tmp = 0;
        tmp = uint8_t(heating);
        buff[4] = tmp & 0x00FF;
        // currentphase (max 7)
        buff[5] = currentPhase & 0x00FF;
        // +4byte unsigned long fuer time



        // currentphasestarttime
        // interrupt ?
        Serial.write(buff, 6);

    }
}

// als queue realisieren...
void storeTemp() {
    if (last60tempsinit) {
        for (int i = 0; i < 60; i++) {
            last60temps[i] = uint16_t(ist * 100);
            last60tempsinit = false;
        }
    }
    if (millis() - lastStoredTempTime > 1000) { // warum sek und nicht veraenderung der temp
        lastStoredTempTime = millis();
        for (int i = 0; i < 60; i++) {
            // COMPILER WARNING: warning: iteration 59u invokes undefined behavior [-Waggressive-loop-optimizations]
            last60temps[i] = last60temps[i + 1];
        }
        last60temps[59] = uint16_t(ist * 100);
    }
}

void loop() {
    ist = getTemperature();
    if (ist == -1000.0) { // initialisierungswert des temp sensors, soll vermutlich ein break darstellen.
        delay(10);
        return;
    }
    storeTemp();
    myPID.Compute();
    printLCD();
    printSerial();

    // heating zeigt heiz oder rastphase an
    if ((ist + TOLERANCE >= phases[currentPhase].temp) && heating) {
        //
        currentPhaseStartTime = millis();
        heating = false;
    }
    if ((millis() - currentPhaseStartTime > phases[currentPhase].duration * 60000) && !heating &&
        (currentPhase < (NR_PHASES - 1))) {
        currentPhase++;
        resetPID();
    }
    switchingEnabled = true;
}
