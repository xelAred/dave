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
#include <EEPROM.h>

void EEPROM_writeDouble(int ee, double value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
}

double EEPROM_readDouble(int ee)
{
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return value;
}
#define EEPROM_KP_ADDRESS 0
#define EEPROM_KI_ADDRESS 4
#define EEPROM_KD_ADDRESS 8
#define EEPROM_LIMIT_ADDRESS 12
#define EEPROM_ANGLE_OFFSET_ADDRESS 16
//SDA - Pin A4
//SCL - Pin A5
IMU Imu(0x68);
SoftwareSerial9 LeftWheelSerial(8, 10);
SoftwareSerial9 RightWheelSerial(9, 11);
Wheel LeftWheel(&LeftWheelSerial, 31847, false);
Wheel RightWheel(&RightWheelSerial, 31847, false);

double Setpoint, Input, Output;
// double consKp=20, consKi=0.0, consKd=0.0, offset=0.0;
double consKp, consKd, consKi, offset=0.0;
double AngleOffset;
PID AngleControl(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
int remoteMode = 1;
byte received_byte = 0;
double Limit;

String inString = "";
unsigned int delayUs = 1000;
unsigned int microSeconds = 0;
int SerialOutput = 0;

void setup()
{
    consKp = (double)EEPROM_readDouble(EEPROM_KP_ADDRESS);
    consKi = (double)EEPROM_readDouble(EEPROM_KI_ADDRESS);
    consKd = (double)EEPROM_readDouble(EEPROM_KD_ADDRESS);
    Limit = (double)EEPROM_readDouble(EEPROM_LIMIT_ADDRESS);
    AngleOffset = (double)EEPROM_readDouble(EEPROM_ANGLE_OFFSET_ADDRESS);

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
    AngleControl.SetOutputLimits(-Limit, Limit);
    AngleControl.SetSampleTime(1);
    AngleControl.SetTunings(consKp, consKi, consKd);
    AngleControl.SetMode(AUTOMATIC);
}


double roundFromZero(double in)
{
    if(((in - (double)(int)in)/ 2.0) > 0.5)
    {
        return (double) ((int)in + 1);
    }
    else
    {
            return (double)((int)in);
    }
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
                EEPROM_writeDouble(EEPROM_KP_ADDRESS, consKp);
                AngleControl.SetTunings(consKp, consKi, consKd);
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print(", Limits: ");
                Serial.print(Limit);
                Serial.print(", AngleOffset: ");
                Serial.print(AngleOffset);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'I' || inString[0] == 'i')
            {
                inString.remove(0, 1);
                consKi = inString.toFloat();
                EEPROM_writeDouble(EEPROM_KI_ADDRESS, consKi);
                AngleControl.SetTunings(consKp, consKi, consKd);
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print(", Limits: ");
                Serial.print(Limit);
                Serial.print(", AngleOffset: ");
                Serial.print(AngleOffset);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'D' || inString[0] == 'd')
            {
                inString.remove(0, 1);
                consKd = inString.toFloat();
                EEPROM_writeDouble(EEPROM_KD_ADDRESS, consKd);
                AngleControl.SetTunings(consKp, consKi, consKd);
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print(", Limits: ");
                Serial.print(Limit);
                Serial.print(", AngleOffset: ");
                Serial.print(AngleOffset);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'O' || inString[0] == 'o')
            {
                if(SerialOutput == 1)
                    SerialOutput = 0;
                else
                    SerialOutput = 1;
            }
            else if(inString[0] == 'R' || inString[0] == 'r')
            {
                Serial.print("P: ");
                Serial.print(consKp);
                Serial.print(", I: ");
                Serial.print(consKi);
                Serial.print(", D: ");
                Serial.print(consKd);
                Serial.print(", Limits: ");
                Serial.print(Limit);
                Serial.print(", AngleOffset: ");
                Serial.print(AngleOffset);
                Serial.print("\r\n");
            }
            else if(inString[0] == 'L' || inString[0] == 'l')
            {
                inString.remove(0, 1);
                Limit = inString.toFloat();
                EEPROM_writeDouble(EEPROM_LIMIT_ADDRESS, Limit);
                Serial.print("Limits: ");
                Serial.print(Limit);
                Serial.print("\r\n");
                AngleControl.SetOutputLimits(-Limit, Limit);
            }
            else if(inString[0] == 'F' || inString[0] == 'f')
            {
                inString.remove(0, 1);
                AngleOffset = inString.toFloat();
                EEPROM_writeDouble(EEPROM_ANGLE_OFFSET_ADDRESS,AngleOffset);
                Serial.print("AngleOffset: ");
                Serial.print(AngleOffset);
                Serial.print("\r\n");
            }
            else
            {

            }

            inString = "";
            received_byte = 0;
        }

    }

    Setpoint = 0.0;
    static signed short spWhl=0;
    static signed short spWhr=0;
    static signed short spWhlTmp=0;
    static signed short spWhrTmp=0;

    Imu.Process();
    Input = Imu.GetAngleY();
    Input -= AngleOffset;
    // if(abs(Input) > 0.8)
    // {
      AngleControl.Compute();
    // }
    // else
    // {
    //     Output = 0.0;
    // }

    if(Output < 0.0)
    {
      // Output -= offset;
        spWhlTmp = (int16_t)Output - (int16_t)offset;
        spWhrTmp = (int16_t)Output - (int16_t)offset;
    }
    else if(Output > 0.0)
    {
      // Output += offset;
        spWhlTmp = (int16_t)Output + (int16_t)offset;
        spWhrTmp = (int16_t)Output + (int16_t)offset;
    }
    else
    {

    }

    Output = roundFromZero(Output);


    // RightWheel.SetSpeed((int16_t)Output);
    // LeftWheel.SetSpeed((int16_t)Output);
    delayMicroseconds(delayUs);
    microSeconds += delayUs;
    //if(microSeconds >= 10000)
    {
      microSeconds = 0;
      if(SerialOutput == 1)
      {
          Serial.print(Input);
          Serial.print(", ");
          Serial.print(spWhlTmp);
          Serial.print(", ");
          Serial.print(Output);
          Serial.print("\r\n");
      }
      spWhl = spWhlTmp;
      spWhr = spWhrTmp;
    }
    RightWheel.SetSpeed(spWhl);
    LeftWheel.SetSpeed(spWhr);
}
