#include <Arduino.h>
#include <SoftwareSerial9.h>
#include <Wheel.h>

SoftwareSerial9 LeftWheelSerial(8, 10);
SoftwareSerial9 RightWheelSerial(9, 11);
Wheel LeftWheel(&LeftWheelSerial, 31847, false);
Wheel RightWheel(&RightWheelSerial, 31847, true);

void setup()
{
    Serial.begin(115200);
}

char c = ' ';
signed int sp=0;

void loop() {
    Serial.println(c);
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
    }
    Serial.print("speed ");
    Serial.println(sp);

    do
    {
        RightWheel.SetSpeed(sp);
        LeftWheel.SetSpeed(sp);
        delayMicroseconds(300);
    } while(!Serial.available());
    c=Serial.read();
}
