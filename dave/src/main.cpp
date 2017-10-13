#include <Arduino.h>
#include <SoftwareSerial9.h>
#include <Wheel.h>

SoftwareSerial9 LeftWheelSerial(8, 10);
SoftwareSerial9 RightWheelSerial(9, 11);
Wheel LeftWheel(&LeftWheelSerial, 31847, false);
Wheel RightWheel(&RightWheelSerial, 31847, false);

void setup()
{
    Serial.begin(115200);
}

char cWheel = ' ', cRot = ' ';
signed int spWhl=0;
signed int spWhr=0;

void loop() {
    Serial.println(cWheel);
    Serial.println(cRot);

    switch (cWheel)
    {
       case 'r':
       {
         if(cRot == '+')
         {
            spWhr += 50;
         }
         else if(cRot == '-')
         {
            spWhr -= 50;
         }
         else if(cRot == '0')
         {
            if(spWhr > 150 || spWhr < -150)
            {
               Serial.println("Decrease speed first! Stopping only allowed within speed =+-100");
            }
            else
            {
               spWhr = 0;
            }
         }
         break;
       }
       case 'l':
       {
         if(cRot == '+')
         {
            spWhl += 50;
         }
         else if(cRot == '-')
         {
            spWhl -= 50;
         }
         else if(cRot == '0')
         {
            if(spWhl > 150 || spWhl < -150)
            {
               Serial.println("Decrease speed first! Stopping only allowed within speed =+-100");
            }
            else
            {
               spWhl = 0;
            }
         }
         break;
       }
       case 'b':
       {
         if(cRot == '+')
         {
            spWhl += 50;
            spWhr += 50;
         }
         else if(cRot == '-')
         {
            spWhl -= 50;
            spWhr -= 50;
         }
         else if(cRot == '0')
         {
            if(spWhr > 150 || spWhr < -150 && spWhl > 150 || spWhl < -150)
            {
               Serial.println("Decrease speed first! Stopping only allowed within speed =+-100");
            }
            else
            {
               spWhl = 0;
               spWhr = 0;
            }
         }
         break;
       }
       default:
       {
          break;
       }
    }
    /*
    if(c == '5')
    {
        if(sp > 100 && sp < 100)
        {
            Serial.println("Decrease speed first! Stopping only allowed within speed =+-100");
        }
        else
        {
            sp = 0;
        }

    }
    else if(c == '4')
    {
        sp -= 10;
    }
    else if(c == '6')
    {
        sp += 10;
    }
    else if(c == '7')
    {
        sp += 100;
    }
    else if(c == '3')
    {
        sp -= 100;
    } */
    Serial.print("speed rechts ");
    Serial.println(spWhr);
    Serial.print("speed links ");
    Serial.println(spWhl);

    do
    {
        RightWheel.SetSpeed(spWhr);
        LeftWheel.SetSpeed(spWhl);
        delayMicroseconds(300);
    } while(!Serial.available());
    cWheel=Serial.read();
    cRot=Serial.read();
}
