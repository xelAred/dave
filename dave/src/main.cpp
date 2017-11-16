/* Copyright (C) 2017 Armin Schlegel. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").
 */

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial9.h>
#include "IMU.h"
#include "Wheel.h"
#include <PID_v1.h>


//SDA - Pin A4
//SCL - Pin A5
IMU Imu(0x68);
SoftwareSerial9 LeftWheelSerial(8, 10);
SoftwareSerial9 RightWheelSerial(9, 11);
Wheel LeftWheel(&LeftWheelSerial, 31847, false);
Wheel RightWheel(&RightWheelSerial, 31847, false);

double Setpoint, Input, Output;
double consKp=1, consKi=0.05, consKd=0.25;
PID AngleControl(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup()
{
    Serial.begin(115200);
    Wire.begin();
#if ARDUINO >= 157
    Wire.setClock(400000UL); // Set I2C frequency to 400kHz
#else
    TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif

    if(Imu.Init() == -1)
    {
        Serial.println("Error initializing IMU");
    }
    //turn the PID on
    AngleControl.SetMode(AUTOMATIC);
    AngleControl.SetOutputLimits(-100.0, 100.0);
}

void loop()
{
    Setpoint = 0.0;
    signed int spWhl=0;
    signed int spWhr=0;

    Imu.Process();
    Input = Imu.GetAngleY();
    Serial.print(Input);

    AngleControl.Compute();

    Serial.print("\tPID: ");
    Serial.print(Output);
    Serial.print("\r\n");
    RightWheel.SetSpeed((int16_t)Output);
    LeftWheel.SetSpeed((int16_t)Output);
    delayMicroseconds(300);
}
