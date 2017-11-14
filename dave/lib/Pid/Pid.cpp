#include "Pid.h"
#include <algorithm>

Pid::Pid(float _SampleRateMs, float Min, float Max)
{
    Kp = 1.0f;
    Ki = 0.0f;
    Kd = 0.0f;
    Error = 0.0f;
    ErrorOld = 0.0f;
    ErrorSum = 0.0f;
    SampleTimeMs = _SampleRateMs;
    Setpoint = 0.0;
    MaxLimit = Max;
    MinLimit = Min;
    Output = 0.0;
    LastInput = 0.0;
    KdOutput = 0;
    KiOutput = 0;
    KpOutput = 0;
}

void Pid::SetSetpoint(float setpoint)
{
    Setpoint = setpoint;
}

void Pid::AddToError(float in)
{
  Error += in;
}
void Pid::SetParameters(float _p, float _i, float _d)
{
    Kp = _p;
    Ki = _i;
    Kd = _d;
}

float Pid::Handler(float input)
{
    Error = Setpoint - input;
    ErrorSum += (Error);

    ErrorSum = std::min(ErrorSum, MaxLimit);
    ErrorSum = std::max(ErrorSum, MinLimit);

    KpOutput = Kp * Error;
    KiOutput = Ki * ErrorSum * SampleTimeMs;
    KdOutput = Kd * (Error - ErrorOld) / SampleTimeMs;

    KiOutput = std::min(KiOutput, MaxLimit);
    KiOutput = std::max(KiOutput, MinLimit);

    Output =  KpOutput + KiOutput + KdOutput;

    Output = std::min(Output, MaxLimit);
    Output = std::max(Output, MinLimit);
    ErrorOld = Error;
    return Output;
}

Pid::IntermediateOutputsType Pid::GetIntermediateOutputs()
{
    IntermediateOutputsType Ret;
    Ret.KpOutput = KpOutput;
    Ret.KiOutput = KiOutput;
    Ret.KdOutput = KdOutput;
    return Ret;
}

float Pid::GetOutput()
{
    return Output;
}

