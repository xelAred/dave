#include <Arduino.h>

class IMU
{
    public:
        IMU(uint8_t Address);
        ~IMU(){};
        int Init();
        void Process();
        double GetAngleX();
        double GetAngleY();
    private:
        uint8_t IMUAddress;
        double accX, accY, accZ;
        double gyroX, gyroY, gyroZ;
        double compAngleX, compAngleY;
        int16_t tempRaw;
        uint32_t timer;
        uint8_t i2cData[14];
};
