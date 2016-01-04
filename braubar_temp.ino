#include <OneWire.h>
#include <TimerOne.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "config.h"
#include "ds18b20.h"

// Temp vars
unsigned long now;
double istTemperatur;

unsigned long TIME_SERIAL_LAST_SENT = 0;
const uint8_t SENSOR_ID = 1;

// Eth vars
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// IP address arduino
IPAddress ip(192,168,2, 178);

// IP address server
IPAddress server(255,255,255,255);

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// server port to connect to
unsigned int remotePort = 50505;

String ip2string(IPAddress address) ;


// fuer protokoll auf laptop...
// strings ueber serial
void printSerial() ;

void setup() {
    Serial.begin(9600);
    // start the Ethernet connection:
    if(Ethernet.begin(mac)==1) {
        Serial.println(Ethernet.localIP());
    } else {
        Serial.println("No DHCP Address :(");
    }
    // Open serial communications and wait for port to open:
    // give the Ethernet shield a second to initialize:
    delay(1000);
    Udp.begin(remotePort);

}

void loop() {
    istTemperatur = getTemperature();
    if (istTemperatur == -1000.0) { // initialisierungswert des temp sensors, soll vermutlich ein break darstellen.
        delay(10);
        return;
    }

    printSerial();

    // renews the DHCP lease.
    Ethernet.maintain();
}

String ip2string(IPAddress address) {
    byte a = address[0];
    byte b = address[1];
    byte c = address[2];
    byte d = address[3];
    return String(a)+"."+String(b)+"."+String(c)+"."+String(d);
}

// fuer protokoll auf laptop...
// strings ueber serial
void printSerial() {
    now = millis();
    if ((now - TIME_SERIAL_LAST_SENT) > TIME_SERIAL) {
        TIME_SERIAL_LAST_SENT = now;
        Serial.println(istTemperatur);// strings ueber serial

        Udp.beginPacket(server, remotePort);

        // protokoll ueberlegen mit syncbyte und check
        char buff[11];
        buff[0] = 0xF;
        buff[1] = 0x0;
        buff[2] = 0xF;
        buff[3] = 0xF;
        buff[4] = uint16_t(istTemperatur * 100) & 0x00FF;
        buff[5] = (uint16_t(istTemperatur * 100) >> 8) & 0x00FF;
        // heating
        uint8_t(heating);
        buff[6] = SENSOR_ID;
        buff[7] = 0xF;
        buff[8] = 0x0;
        buff[9] = 0xF;
        buff[10] = 0xF;
        Serial.println(buff);
        Udp.write(buff, 11);
        Udp.endPacket();
    }
}