

/*  Seeed_ADIS16470.cpp
    /*

    Copyright (c) 2019 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Create Time: February 2019
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "Seeed_ADIS16470.h"

/** Initialize object.
    @param CS:Chip select pin
*/
ADIS16470::ADIS16470(int CS) {
    _CS = CS;

    SPI.begin();
    configSPI();

    pinMode(_CS, OUTPUT);
    digitalWrite(_CS, HIGH);
}

ADIS16470::~ADIS16470() {
    // Close SPI bus
    SPI.end();
}

/** Sets SPI bitorder(MSB First Mode ), clockdivider(SCLK ≤ 2 MHz), and datamode(SPI Mode 3 ).
    notice: Burst mode read requires this to be ≤1 MHz !
*/
void ADIS16470::configSPI() {
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.setDataMode(SPI_MODE3);
}

/** Read registers using SPI.
    @param regAddr:the address of register
    @return the value from register
*/
int16_t ADIS16470::regRead(uint8_t regAddr) {
    digitalWrite(_CS, LOW);
    SPI.transfer(regAddr);
    SPI.transfer(0x00);
    digitalWrite(_CS, HIGH);

    delayMicroseconds(time);

    digitalWrite(_CS, LOW);
    uint8_t _msbData = SPI.transfer(0x00);
    uint8_t _lsbData = SPI.transfer(0x00);
    digitalWrite(_CS, HIGH);

    delayMicroseconds(time);

    int16_t _dataOut = (_msbData << 8) | (_lsbData & 0xFF);

    return (_dataOut);
}

/** Write registers using SPI.
    @param regAddr:the address of register
    @param regData:the value is writed
    @return true
*/
int ADIS16470::regWrite(uint8_t regAddr, int16_t regData) {
    uint16_t addr = (((regAddr & 0x7F) | 0x80) << 8);
    uint16_t lowWord = (addr | (regData & 0xFF));
    uint16_t highWord = ((addr | 0x100) | ((regData >> 8) & 0xFF));

    // Split words into chars
    uint8_t highBytehighWord = (highWord >> 8);
    uint8_t lowBytehighWord = (highWord & 0xFF);
    uint8_t highBytelowWord = (lowWord >> 8);
    uint8_t lowBytelowWord = (lowWord & 0xFF);

    // Write highWord to SPI bus
    digitalWrite(_CS, LOW);
    SPI.transfer(highBytelowWord);
    SPI.transfer(lowBytelowWord);
    digitalWrite(_CS, HIGH);

    delayMicroseconds(time);;

    // Write lowWord to SPI bus
    digitalWrite(_CS, LOW);
    SPI.transfer(highBytehighWord);
    SPI.transfer(lowBytehighWord);
    digitalWrite(_CS, HIGH);

    delayMicroseconds(time);;

    return (1);
}

/** Burstread value(byte)
    @return  20 bytes value
*/
uint8_t* ADIS16470::byteBurst(void) {

    static uint8_t burstdata[21];

    // Trigger Burst Read
    digitalWrite(_CS, LOW);
    SPI.transfer(0x68);
    SPI.transfer(0x00);

    // Read Burst Data
    burstdata[0] = SPI.transfer(0x00); //DIAG_STAT
    burstdata[1] = SPI.transfer(0x00);
    burstdata[2] = SPI.transfer(0x00); //XGYRO_OUT
    burstdata[3] = SPI.transfer(0x00);
    burstdata[4] = SPI.transfer(0x00); //YGYRO_OUT
    burstdata[5] = SPI.transfer(0x00);
    burstdata[6] = SPI.transfer(0x00); //ZGYRO_OUT
    burstdata[7] = SPI.transfer(0x00);
    burstdata[8] = SPI.transfer(0x00); //XACCEL_OUT
    burstdata[9] = SPI.transfer(0x00);
    burstdata[10] = SPI.transfer(0x00); //YACCEL_OUT
    burstdata[11] = SPI.transfer(0x00);
    burstdata[12] = SPI.transfer(0x00); //ZACCEL_OUT
    burstdata[13] = SPI.transfer(0x00);
    burstdata[14] = SPI.transfer(0x00); //TEMP_OUT
    burstdata[15] = SPI.transfer(0x00);
    burstdata[16] = SPI.transfer(0x00); //TIME_STMP
    burstdata[17] = SPI.transfer(0x00);
    burstdata[18] = SPI.transfer(0x00); //CHECKSUM
    burstdata[19] = SPI.transfer(0x00);
    digitalWrite(_CS, HIGH);
    burstdata[20] = 0x01; //firmware version 1.0
    return burstdata;

}

/** Burstread value(word)
    @return 10 words value
*/
uint16_t* ADIS16470::wordBurst(void) {

    static uint16_t burstwords[10];

    // Trigger Burst Read
    digitalWrite(_CS, LOW);
    SPI.transfer(0x68);
    SPI.transfer(0x00);

    // Read Burst Data
    burstwords[0] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //DIAG_STAT
    burstwords[1] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //XGYRO
    burstwords[2] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //YGYRO
    burstwords[3] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //ZGYRO
    burstwords[4] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //XACCEL
    burstwords[5] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //YACCEL
    burstwords[6] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //ZACCEL
    burstwords[7] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //TEMP_OUT
    burstwords[8] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //TIME_STMP
    burstwords[9] = ((SPI.transfer(0x00) << 8) | (SPI.transfer(0x00) & 0xFF)); //CHECKSUM

    digitalWrite(_CS, HIGH);

    return burstwords;

}


/** Calculate checksum value.
    @param burstArray:the array of burstread value
    @return checksum value
*/
int16_t ADIS16470::checksum(uint16_t* burstArray) {
    int16_t s = 0;
    for (int i = 0; i < 9; i++) { // Checksum value is not part of the sum!!
        s += (burstArray[i] & 0xFF); // Count lower byte
        s += ((burstArray[i] >> 8) & 0xFF); // Count upper byte
    }

    return s;
}

/** Calculate accelerator data
    @param sensorData:data output from regRead()
    @return signed/scaled accelerometer in g's
*/
float ADIS16470::accelScale(int16_t sensorData) {
    float finalData = sensorData * 0.00125; // Multiply by accel sensitivity (0.00125g/LSB)
    return finalData;
}

/** Calculate gyro data
    @param sensorData:data output from regRead()
    @return signed/scaled gyro in degrees/sec
*/
float ADIS16470::gyroScale(int16_t sensorData) {
    float finalData = sensorData * 0.1; // Multiply by gyro sensitivity (0.1 deg/LSB)
    return finalData;
}

/** Calculate temperature data
    @param sensorData:data output from regRead()
    @return signed/scaled temperature in degrees Celcius
*/
float ADIS16470::tempScale(int16_t sensorData) {
    float finalData = (sensorData * 0.1); // Multiply by temperature scale (0.1 deg C/LSB)
    return finalData;
}

/** Calculate delta angle data
    @param sensorData:data output from regRead()
    @return signed/scaled delta angle in degrees
*/
float ADIS16470::deltaAngleScale(int16_t sensorData) {
    float finalData = sensorData * 0.061; // Multiply by delta angle scale (0.061 degrees/LSB)
    return finalData;
}

/** Calculate delta velocity
    @param sensorData:data output from regRead()
    @return signed/scaled delta velocity in m/sec
*/
float ADIS16470::deltaVelocityScale(int16_t sensorData) {
    float finalData = sensorData * 0.01221; // Multiply by velocity scale (0.01221 m/sec/LSB)
    return finalData;
}


/** Burstread value(word)
    @param burstdata:the 20 bytes value
    @return 10 words value
*/
uint16_t* ADIS16470::wordData(uint8_t* burstdata) {

    static uint16_t wordData[10];

    // Convert Data
    wordData[0] = ((burstdata[0] << 8) | (burstdata[1] & 0xFF)); //DIAG_STAT
    wordData[1] = ((burstdata[2] << 8) | (burstdata[3] & 0xFF)); //XGYRO
    wordData[2] = ((burstdata[4] << 8) | (burstdata[5] & 0xFF)); //YGYRO
    wordData[3] = ((burstdata[6] << 8) | (burstdata[7] & 0xFF)); //ZGYRO
    wordData[4] = ((burstdata[8] << 8) | (burstdata[9] & 0xFF)); //XACCEL
    wordData[5] = ((burstdata[10] << 8) | (burstdata[11] & 0xFF)); //YACCEL
    wordData[6] = ((burstdata[12] << 8) | (burstdata[13] & 0xFF)); //ZACCEL
    wordData[7] = ((burstdata[14] << 8) | (burstdata[15] & 0xFF)); //TEMP_OUT
    wordData[8] = ((burstdata[16] << 8) | (burstdata[17] & 0xFF)); //TIME_STMP
    wordData[9] = ((burstdata[18] << 8) | (burstdata[19] & 0xFF)); //CHECKSUM

    return wordData;

}