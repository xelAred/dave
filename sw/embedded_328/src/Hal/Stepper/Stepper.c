/*
 * Stepper.c
 *
 *  Created on: Jun 26, 2017
 *      Author: armin
 */

#include "Stepper.h"
#include "Timer.h"

static void Stepper_PerformStep(Stepper_ChannelType Channel);

Stepper_DataType Stepper_Data_s;

Stepper_PinConfigurationType Stepper_Configuration[STEPPER_MAX_NUM_OF_STEPPERS] =
{
        /* Winding Stepper */
        {
                GPIO_CHANNEL_PC4,
                GPIO_CHANNEL_PC5
        },
        /* Spindle Stepper */
        {
                GPIO_CHANNEL_PD0,
                GPIO_CHANNEL_PD1
        },
        /* Reel Stepper */
        {
                GPIO_CHANNEL_PD2,
                GPIO_CHANNEL_PD3
        }
};

void Stepper_Init(void)
{
    Stepper_SetSpeedAndDir(STEPPER_CHANNEL_0, 0);
    Stepper_SetSpeedAndDir(STEPPER_CHANNEL_1, 0);
    Stepper_SetSpeedAndDir(STEPPER_CHANNEL_2, 0);
    Stepper_Data_s.Stepper_s[STEPPER_CHANNEL_0].StepCounter_ui16 = 0;
    Stepper_Data_s.Stepper_s[STEPPER_CHANNEL_1].StepCounter_ui16 = 0;
    Stepper_Data_s.Stepper_s[STEPPER_CHANNEL_2].StepCounter_ui16 = 0;
}

void Stepper_Handler(void)
{
    Stepper_ChannelType Channel_e;
    for(Channel_e = STEPPER_CHANNEL_0; Channel_e < STEPPER_MAX_NUM_OF_STEPPERS; Channel_e++)
    {
        if(++Stepper_Data_s.Stepper_s[Channel_e].StepCounter_ui16 >= Stepper_Data_s.Stepper_s[Channel_e].Speed_ui16)
        {
            Stepper_Data_s.Stepper_s[Channel_e].StepCounter_ui16 = 0;
            Stepper_PerformStep(Channel_e);
        }
    }
}

void Stepper_SetSpeed(Stepper_ChannelType ch, uint16 speed)
{
    Stepper_Data_s.Stepper_s[ch].Speed_ui16 = speed;
}

void Stepper_SetDir(Stepper_ChannelType ch, Stepper_DirectionType dir)
{
    Stepper_Data_s.Stepper_s[ch].Dir_e = dir;
    Gpio_WriteChannel(Stepper_Configuration[ch].Direction_e, (Gpio_PinState)dir);
}

void Stepper_SetSpeedAndDir(Stepper_ChannelType ch, sint16 speed_si16)
{
    if(speed_si16 < 0)
    {
        Stepper_SetDir(ch, STEPPER_DIR_CCW);
        Stepper_Data_s.Stepper_s[ch].Speed_ui16  = (uint16)(-1 * speed_si16);
    }
    else
    {
        Stepper_SetDir(ch, STEPPER_DIR_CW);
        Stepper_Data_s.Stepper_s[ch].Speed_ui16  = (uint16)speed_si16;
    }
}

static void Stepper_PerformStep(Stepper_ChannelType Channel)
{
    Gpio_WriteChannel(Stepper_Configuration[Channel].Step_e, GPIO_HIGH);
    SoftTimer_DelayUsBlocking(10);
    Gpio_WriteChannel(Stepper_Configuration[Channel].Step_e, GPIO_LOW);
    SoftTimer_DelayUsBlocking(10);
}



