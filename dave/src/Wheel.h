
#include <SoftwareSerial9.h>

class Wheel
{
public:
    Wheel(SoftwareSerial9 *serial, uint32_t Baud, bool inv);
    ~Wheel();
    void SetSpeed(int16_t sp);
    void Stop();
    void IncreaseSpeed();
    void DecreaseSpeed();
private:
    int16_t currentSpeed;
    bool inverted;
    uint32_t Baudrate;
    SoftwareSerial9 *mySerial;
    void SendSpeedOverUart(int16_t sp);
    const int16_t SpeedDelta = 50;
};
