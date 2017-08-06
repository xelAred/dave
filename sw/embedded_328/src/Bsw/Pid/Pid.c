#include "Std_Types.h"
#include "Pid.h"

void Pid_Init(Pid_DataType *pid, float _SampleRateMs, float Min, float Max)
{
    pid->Kp = 1.0f;
    pid->Ki = 0.0f;
    pid->Kd = 0.0f;
    pid->Error = 0.0f;
    pid->ErrorOld = 0.0f;
    pid->ErrorSum = 0.0f;
    pid->SampleTimeMs = _SampleRateMs;
    pid->Setpoint = 0.0;
    pid->MaxLimit = Max;
    pid->MinLimit = Min;
}

void Pid_SetSetpoint(Pid_DataType *pid, float setpoint)
{
    pid->Setpoint = setpoint;
}

void Pid_SetParameters(Pid_DataType *pid, float _p, float _i, float _d)
{
    pid->Kp = _p;
    pid->Ki = _i;
    pid->Kd = _d;
}

float Pid_Handler(Pid_DataType *pid)
{
    float input = pid->Output;
    pid->Error = pid->Setpoint - input;
    pid->ErrorSum += (pid->Error);

    pid->Output =  pid->Kp * pid->Error;
    pid->Output += pid->Ki * pid->SampleTimeMs * pid->ErrorSum;
    pid->Output += pid->Kd * (pid->Error - pid->ErrorOld)/pid->SampleTimeMs;

    if(pid->Output > pid->MaxLimit)
    {
        pid->Output = pid->MaxLimit;
    }
    else if(pid->Output < pid->MinLimit)
    {
        pid->Output = pid->MinLimit;
    }

    pid->ErrorOld = pid->Error;
    return pid->Output;
}

float Pid_GetOutput(Pid_DataType *pid)
{
    return pid->Output;
}

