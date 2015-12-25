#ifndef BIER_DS18B20_H
#define BIER_DS18B20_H


#define TIME_SENSOR_WAIT 750

OneWire ds(9);

unsigned long TEMP_TIME_SENSOR_LAST_TRIGGERED = 0;
unsigned long TEMP_TIME_SENSOR_LAST_READ = 0;
float TEMP_LAST_TEMPERATURE = -1000.0;
unsigned long TEMP_SENSOR_NOW;
bool TEMP_SENSOR_VALUE_AVAIL = false;
unsigned char i;

float getTemperature() {
    TEMP_SENSOR_NOW = millis();
    if((TEMP_SENSOR_NOW - TEMP_TIME_SENSOR_LAST_TRIGGERED) > TIME_SENSOR_WAIT) {
        if(TEMP_TIME_SENSOR_LAST_TRIGGERED != 0) {
            TEMP_SENSOR_VALUE_AVAIL = true;
        }
        TEMP_TIME_SENSOR_LAST_TRIGGERED = TEMP_SENSOR_NOW;

        byte addr[8];

        if (!ds.search(addr)) { ds.reset_search(); }
        if (OneWire::crc8(addr, 7) != addr[7]) {}

        ds.reset();
        ds.select(addr);
        ds.write(0x44, 1);
    }

    TEMP_SENSOR_NOW = millis();
    if((TEMP_SENSOR_NOW - TEMP_TIME_SENSOR_LAST_READ) > TIME_SENSOR_WAIT && TEMP_SENSOR_VALUE_AVAIL) {
        TEMP_TIME_SENSOR_LAST_READ = TEMP_SENSOR_NOW;

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

        TEMP_LAST_TEMPERATURE = (float)raw / 16.0;
    }

    return TEMP_LAST_TEMPERATURE;
}

#endif
