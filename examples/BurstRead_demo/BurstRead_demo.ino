/*  BurstRead_demo.ino
/*
 *  
 * Copyright (c) 2019 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Create Time: February 2019
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Seeed_ADIS16470.h>
#include <Wire.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

uint8_t burstdata[20];
uint16_t *wordData;
// Accelerometer
float AXS, AYS, AZS = 0;

// Gyro
float GXS, GYS, GZS = 0;

// Temperature
float TEMPS = 0;

// Call ADIS16470 Class
ADIS16470 IMU(10);

void scaleData()
{
  GXS = IMU.gyroScale(*(wordData + 1)); //Scale X Gyro
  GYS = IMU.gyroScale(*(wordData + 2)); //Scale Y Gyro
  GZS = IMU.gyroScale(*(wordData + 3)); //Scale Z Gyro
  AXS = IMU.accelScale(*(wordData + 4)); //Scale X Accel
  AYS = IMU.accelScale(*(wordData + 5)); //Scale Y Accel
  AZS = IMU.accelScale(*(wordData + 6)); //Scale Z Accel
  TEMPS = IMU.tempScale(*(wordData + 7)); //Scale Temp Sensor
}

void setup() 
{
  Wire.begin();        // join i2c bus (address optional for master)
  SERIAL.begin(9600);  // start serial for output
}

void loop() 
{
  Wire.requestFrom(0x36, 20);    // request 6 bytes from slave device #8
  int i=0;
  while (Wire.available()) 
  { // slave may send less than requested
    uint8_t c = Wire.read(); // receive a byte as character
    burstdata[i] = c;
    i++;
    //SERIAL.print(c,HEX);
    //SERIAL.print("  ");
  }
  i=0;

  wordData = IMU.wordData(burstdata);
  scaleData();

  //print the array of burstread and checksum
  /*
    int s=0;
    for(int i=0;i<20;i++)
    {
        if(i<18)
        {
            s+=burstdata[i];
        }
        else
        {
            burstChecksum = s;
        }
        
        SERIAL.print(burstdata[i],HEX);
        SERIAL.print("  ");
        if(i%5==0)SERIAL.println();
    }
    SERIAL.print("checksum: ");
    SERIAL.println(burstChecksum,HEX);
  */

  // Print scaled gyro data
  SERIAL.print("XGYRO(degrees/sec): ");
  SERIAL.println(GXS);
  SERIAL.print("YGYRO(degrees/sec): ");
  SERIAL.println(GYS);
  SERIAL.print("ZGYRO(degrees/sec): ");
  SERIAL.println(GZS);
  
  // Print scaled accel data
  SERIAL.print("XACCL(g's): ");
  SERIAL.println(AXS);
  SERIAL.print("YACCL(g's): ");
  SERIAL.println(AYS);
  SERIAL.print("ZACCL(g's): ");
  SERIAL.println(AZS);
  SERIAL.println(" ");

  // Print Status Registers
  SERIAL.print("DIAG_STAT: ");
  SERIAL.println((*(burstdata + 0)));
  SERIAL.print("TIME_STMP: ");
  SERIAL.println((*(burstdata + 8)));
  SERIAL.print("CHECKSUM: ");
  SERIAL.println((*(burstdata + 9)));
  
  SERIAL.println();
  SERIAL.println();
  //delay(2000);
}