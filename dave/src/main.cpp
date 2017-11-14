#include <Arduino.h>
#include <SoftwareSerial9.h>
#include <Wheel.h>
#include <Pid.h>

SoftwareSerial9 LeftWheelSerial(8, 10);
SoftwareSerial9 RightWheelSerial(9, 11);
Wheel LeftWheel(&LeftWheelSerial, 31847, false);
Wheel RightWheel(&RightWheelSerial, 31847, false);

signed int spWhl=0;
signed int spWhr=0;

void setup()
{
    Serial.begin(115200);
}



void loop()
{


    do
    {
        RightWheel.SetSpeed(spWhr);
        LeftWheel.SetSpeed(spWhl);
        delayMicroseconds(300);
    } while(!Serial.available());
}
