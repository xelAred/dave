#ifndef __H_
#define __H_

class Pid
{
public:
    typedef struct
    {
        float KpOutput;
        float KiOutput;
        float KdOutput;
    } IntermediateOutputsType;

    Pid(float _SampleRateMs, float Min, float Max);
    ~Pid() {};
    float Handler(float input);
    void SetParameters(float _p, float _i, float _d);
    void SetSetpoint(float setpoint);
    float GetOutput();
    IntermediateOutputsType GetIntermediateOutputs();
    void AddToError(float in);
private:
    float SampleTimeMs;
    float Kp;
    float Ki;
    float Kd;
    float ErrorSum;
    float Error;
    float ErrorOld;
    float Output;
    float Setpoint;
    float MaxLimit;
    float MinLimit;
    float LastInput;
    float KpOutput;
    float KiOutput;
    float KdOutput;
};

#endif /* __H_ */
