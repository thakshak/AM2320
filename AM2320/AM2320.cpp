#include "AM2320.h"
#include <Wire.h>

// AM2320 Temperature & Humidity Sensor library for Arduino
// Modified by Timofeev E.N. from https://github.com/thakshak/AM2320/

unsigned int CRC16(byte *ptr, byte length) {
        unsigned int crc = 0xFFFF;
        uint8_t s = 0x00;

        while (length--) {
                crc ^= *ptr++;
                for (s = 0; s < 8; s++) {
                        if ((crc & 0x01) != 0) {
                                crc >>= 1;
                                crc ^= 0xA001;
                        } else crc >>= 1;
                }
        }
        return crc;
}

AM2320::AM2320() {}

int AM2320::Read() {
        byte buf[8];
        for (int s = 0; s < 8; s++) buf[s] = 0x00;

        Wire.beginTransmission(AM2320_address);
        Wire.endTransmission();
        // request 4 bytes (temperature and humidity)
        Wire.beginTransmission(AM2320_address);
        Wire.write(0x03); // request
        Wire.write(0x00); // from the 0th address
        Wire.write(0x04); // 4 bytes
        if (Wire.endTransmission(1) != 0) return 1;
        delayMicroseconds(1600); //>1.5ms
        // read the query results
        Wire.requestFrom(AM2320_address, 0x08);
        for (int i = 0; i < 0x08; i++) buf[i] = Wire.read();

        // CRC check
        unsigned int Rcrc = buf[7] << 8;
        Rcrc += buf[6];
        if (Rcrc == CRC16(buf, 6)) {
                unsigned int temperature = ((buf[4] & 0x7F) << 8) + buf[5];
                t = temperature / 10.0;
                t = ((buf[4] & 0x80) >> 7) == 1 ? t * (-1) : t;

                unsigned int humidity = (buf[2] << 8) + buf[3];
                h = humidity / 10.0;
                return 0;
        }
        return 2;
}
