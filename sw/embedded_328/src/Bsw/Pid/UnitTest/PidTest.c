#include "../Pid.h"
#include <stdio.h>

float SinusArray[100] =
{
        512,544,576,608,639,670,700,730,
        759,786,813,838,862,885,907,926,
        944,961,975,988,999,1008,1015,1020,
        1023,1024,1023,1020,1015,1008,999,988,
        975,961,944,926,907,885,862,838,
        813,786,759,730,700,670,639,608,
        576,544,512,480,448,416,385,354,
        324,294,265,238,211,186,162,139,
        117,98,80,63,49,36,25,16,
        9,4,1,0,1,4,9,16,
        25,36,49,63,80,98,117,139,
        162,186,211,238,265,294,324,354,
        385,416,448,480
};

void main()
{
    int it = 0;
    int Running = 1;
    float output = 0;

    Pid_DataType Pid;

    Pid_Init(&Pid, 5.0, 0, 1023);
    Pid_SetParameters(&Pid, 0.52, 0.1, 0.3);
    while(Running)
    {
        Pid_SetSetpoint(&Pid, SinusArray[it + 1]);
        output = Pid_Handler(&Pid);
        printf("%d, %f, %f\n", it, SinusArray[it + 1], output);
        if(it < 99)
        {
            it++;
        }
        else
        {
            it = 0;
            Running = 0;
        }
    }

    return;
}
