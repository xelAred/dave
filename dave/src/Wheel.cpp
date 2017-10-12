
#include "Wheel.h"

Wheel::Wheel(SoftwareSerial9 *serial, uint32_t Baud, bool inv)
{
    mySerial = serial;
    Baudrate = Baud;
    if(inv == true)
    {
        inverted = -1;
    }
    else
    {
        inverted = 1;
    }
    mySerial->begin(Baudrate);
}

Wheel::~Wheel(){ }

void Wheel::SetSpeed(int16_t sp)
{
    currentSpeed = sp;
    SendSpeedOverUart(currentSpeed);
}

void Wheel::Stop()
{
    SetSpeed(0);
}

void Wheel::IncreaseSpeed()
{
    currentSpeed += SpeedDelta;
    SetSpeed(currentSpeed);
}

void Wheel::DecreaseSpeed()
{
    currentSpeed -= SpeedDelta;
    SetSpeed(currentSpeed);
}

void Wheel::SendSpeedOverUart(int16_t sp)
{
    sp *= inverted;
    mySerial->write9(256);
    mySerial->write9(sp & 0xFF);
    mySerial->write9((sp >> 8) & 0xFF);
    mySerial->write9(sp & 0xFF);
    mySerial->write9((sp >> 8) & 0xFF);
    mySerial->write9(85);
    mySerial->write9(82);
    mySerial->write9(82);
}
