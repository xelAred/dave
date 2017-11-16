// #include <Arduino.h>
// #include <SoftwareSerial9.h>
// #include <Wheel.h>
// #include <Pid.h>
//
// SoftwareSerial9 LeftWheelSerial(8, 10);
// SoftwareSerial9 RightWheelSerial(9, 11);
// Wheel LeftWheel(&LeftWheelSerial, 31847, false);
// Wheel RightWheel(&RightWheelSerial, 31847, false);
//
// signed int spWhl=0;
// signed int spWhr=0;
//
// void setup()
// {
//     Serial.begin(115200);
// }
//
//
//
// void loop()
// {
//
//
//     do
//     {
//         RightWheel.SetSpeed(spWhr);
//         LeftWheel.SetSpeed(spWhl);
//         delayMicroseconds(300);
//     } while(!Serial.available());
// }
//GND - GND
//VCC - VCC
//SDA - Pin A4
//SCL - Pin A5


//
// #include <I2Cdev.h>
// #include <MPU60X0.h>
// #include <EEPROM.h>
//
// //#define DEBUG
// #include "DebugUtils.h"
// #include "CommunicationUtils.h"
// #include "FreeIMU.h"
// #include <Wire.h>
// #include <SPI.h>
//
// int raw_values[9];
// //char str[512];
// float ypr[3]; // yaw pitch roll
// float val[9];
//
// // Set the FreeIMU object
// FreeIMU my3IMU = FreeIMU();
//
// void setup() {
//   Serial.begin(115200);
//   Wire.begin();
//
//   delay(5);
//   my3IMU.init(); // the parameter enable or disable fast mode
//   delay(5);
// }
//
// void loop() {
//
//   my3IMU.getYawPitchRoll(ypr);
//   Serial.print("Yaw: ");
//   Serial.print(ypr[0]);
//   Serial.print(" Pitch: ");
//   Serial.print(ypr[1]);
//   Serial.print(" Roll: ");
//   Serial.print(ypr[2]);
//   Serial.println("");
//
//   delay(10);
// }

/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */
#include <Arduino.h>
#include <Wire.h>
#include "IMU.h"

const uint8_t IMUAddress = 0x68; // AD0 is logic low on the PCB

IMU Imu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
#else
  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif

    if(Imu.Init(IMUAddress) == -1)
    {
            Serial.println("Error initializing IMU");
    }
}

void loop()
{
    Imu.Process();
    Serial.print(Imu.GetAngleX());
    Serial.print("\t");
    Serial.print(Imu.GetAngleY());
    Serial.print("\t");
    Serial.print(Imu.GetAngleZ());
    Serial.print("\r\n");
    delay(2);
}
