
/*  Stm32F030_Firmware.ino
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  SCK = D13/SCK
//  CS = D10/CS
//  DOUT(MISO) = D12/MISO
//  DIN(MOSI) = D11/MOSI
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Seeed_ADIS16470.h>
#include <SPI.h>
#include <Wire.h>


// Initialize Variables
// Temporary Data Array
uint8_t *burstData;

// Call ADIS16470 Class
ADIS16470 IMU(10); // Chip Select, Data Ready, Reset Pin Assignments

// Checksum variable
int16_t burstChecksum = 0;

void setup()
{
    Wire.begin(0x36);                // join i2c bus with address 
    Wire.onRequest(requestEvent); // register event

    Serial.begin(115200); // Initialize serial output via USB
    IMU.configSPI(); // Configure SPI communication
    delay(500); // Give the part time to start up
    IMU.regWrite(MSC_CTRL, 0xC1);  // Enable Data Ready, set polarity
    IMU.regWrite(FILT_CTRL, 0x04); // Set digital filter
    IMU.regWrite(DEC_RATE, 0x00);// Disable decimation


}

void requestEvent() {

  Wire.write(burstData,20); // respond with message of 6 bytes
  
}


// Main loop. Print data to the serial port. Sensor sampling is performed in the ISR
void loop()
{   
    burstData = IMU.byteBurst();
    /*
    int s=0;
    for(int i=0;i<20;i++)
    {
        if(i<18)
        {
            s+=burstData[i];
        }
        else
        {
            burstChecksum = s;
        }
        
        Serial.print(burstData[i],HEX);
        Serial.print("  ");
        if(i%5==0)Serial.println();
    }
    Serial.print("checksum: ");
    Serial.println(burstChecksum,HEX);
    
    Serial.println("................");
    Serial.println("................");

    delay(2000);
    */
}
