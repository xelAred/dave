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
double consKp=50, consKi=0.0, consKd=0.0;
PID AngleControl(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
int remoteMode = 1;
byte received_byte = 0;
double LimitUp = 100;
double LimitDown = -100;
String inString = "";

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
    AngleControl.SetOutputLimits(LimitDown, LimitUp);
}

void loop()
{
    if(Serial.available())
    {
        int inChar = Serial.read();

        if (inChar != '\n')
        {
            inString += (char)inChar;
        }
        else
        {
            if(inString[0] == 'P' || inString[0] == 'p')
            {
                inString.remove(0, 1);
                consKp = inString.toFloat();
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'I' || inString[0] == 'i')
            {
                inString.remove(0, 1);
                consKi = inString.toFloat();
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'D' || inString[0] == 'd')
            {
                inString.remove(0, 1);
                consKd = inString.toFloat();
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'Z' || inString[0] == 'z')
            {
                remoteMode = 1;
                Serial.println("Enable remote control");
            }
            else if(inString[0] == 'R' || inString[0] == 'r')
            {
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print(" Limits: ");
                Serial.print(LimitUp);
                Serial.print("\r\n");
            }

            else if(inString[0] == 'L' || inString[0] == 'l')
            {
                inString.remove(0, 1);
                LimitUp = inString.toFloat();
                LimitDown = -LimitUp;
                Serial.print("Limits: ");
                Serial.print(LimitUp);
                Serial.print("\r\n");
                AngleControl.SetOutputLimits(LimitDown, LimitUp);
            }
            else
            {

            }

            inString = "";
            received_byte = 0;
        }

    }

    Setpoint = 0.0;
    // signed int spWhl=0;
    // signed int spWhr=0;

    Imu.Process();
    Input = Imu.GetAngleY();
    AngleControl.Compute();

    Serial.print(Input);
    Serial.print("\tPID: ");
    Serial.print(-Output);
    Serial.print("\r\n");

    RightWheel.SetSpeed((int16_t)-Output);
    LeftWheel.SetSpeed((int16_t)-Output);
    delayMicroseconds(300);
}
