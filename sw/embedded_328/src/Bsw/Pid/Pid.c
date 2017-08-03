#include "Std_Types.h"
#include "Pid.h"

void Pid_Init(Pid_DataType *pid)
{
    pid->Kp = 0.0f;
    pid->Ki = 0.0f;
    pid->Kd = 0.0f;
    pid->Error = 0.0f;
    pid->ErrorOld = 0.0f;
    pid->ErrorSum = 0.0f;
    pid->SampleTimeMs = 0.0f;
    pid->Setpoint = 0.0;
    pid->MaxLimit = 100.0f;
    pid->MinLimit = 0.0f;
}

void Pid_SetSetpoint(Pid_DataType *pid, float setpoint)
{
    pid->Setpoint = setpoint;
}

void Pid_SetSampleRateMs(Pid_DataType *pid, float _SampleRateMs)
{
    pid->SampleTimeMs = _SampleRateMs;
}

void Pid_SetParameters(Pid_DataType *pid, float _p, float _i, float _d)
{
    pid->Kp = _p;
    pid->Ki = _i * pid->SampleTimeMs;
    pid->Kd = _d / pid->SampleTimeMs;
}

void Pid_Handler(Pid_DataType *pid, float CurrentValue)
{
    float CalcI = 0.0;

    pid->Error = pid->Setpoint - CurrentValue;
    CalcI += (CalcI * pid->Error);
    if(CalcI > pid->MaxLimit)
    {
        CalcI = pid->MaxLimit;
    }
    else if(CalcI < pid->MinLimit)
    {
        CalcI = pid->MinLimit;
    }
    pid->Output = pid->Kp * pid->Error + CalcI - pid->Kd * (CurrentValue - pid->LastInput);

    if(pid->Output > pid->MaxLimit)
    {
        pid->Output = pid->MaxLimit;
    }
    else if(pid->Output < pid->MinLimit)
    {
        pid->Output = pid->MinLimit;
    }

    pid->LastInput = CurrentValue;
}

float Pid_GetOutput(Pid_DataType *pid)
{
    return pid->Output;
}

