#ifndef BIER_DS18B20_H
#define BIER_DS18B20_H

#define TIME_SENSOR_WAIT 750

OneWire ds(10);

unsigned long TIME_SENSOR_LAST_TRIGGERED = 0;
unsigned long TIME_SENSOR_LAST_READ = 0;
float LAST_TEMPERATURE = -1000.0;
unsigned long SENSOR_NOW;
bool SENSOR_VALUE_AVAIL = false;
unsigned char i;

float getTemperature() {
    SENSOR_NOW = millis();
    if((SENSOR_NOW - TIME_SENSOR_LAST_TRIGGERED) > TIME_SENSOR_WAIT) {
        if(TIME_SENSOR_LAST_TRIGGERED != 0) {
            SENSOR_VALUE_AVAIL = true;
        }
        TIME_SENSOR_LAST_TRIGGERED = SENSOR_NOW;

        byte addr[8];

        if (!ds.search(addr)) { ds.reset_search(); }
        if (OneWire::crc8(addr, 7) != addr[7]) {}

        ds.reset();
        ds.select(addr);
        ds.write(0x44, 1);
    }

    SENSOR_NOW = millis();
    if((SENSOR_NOW - TIME_SENSOR_LAST_READ) > TIME_SENSOR_WAIT && SENSOR_VALUE_AVAIL) {
        TIME_SENSOR_LAST_READ = SENSOR_NOW;

        byte addr[8];
        byte present = 0;
        byte data[12];

        if (!ds.search(addr)) { ds.reset_search(); }
        if (OneWire::crc8(addr, 7) != addr[7]) {}

        present = ds.reset();
        ds.select(addr);
        ds.write(0xBE);

        for ( i = 0; i < 9; i++) {
            data[i] = ds.read();
        }

        int16_t raw = (data[1] << 8) | data[0];
        byte cfg = (data[4] & 0x60);
        if (cfg == 0x00) raw = raw & ~7; //9 bit Auflösung, 93,75ms
        else if (cfg == 0x20) raw = raw & ~3; //10 bit Auflösung, 187,5ms
        else if (cfg == 0x40) raw = raw & ~1; //11 bit Auflösung, 375ms

        LAST_TEMPERATURE = (float)raw / 16.0;
    }

    return LAST_TEMPERATURE;
}

#endif
