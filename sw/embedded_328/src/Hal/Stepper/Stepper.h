/*
 * Stepper.h
 *
 *  Created on: Jun 26, 2017
 *      Author: armin
 */

#ifndef STEPPER_H_
#define STEPPER_H_
#include "Std_Types.h"
#include "Gpio.h"

typedef struct
{
    Gpio_ChannelType Step_e;
    Gpio_ChannelType Direction_e
} Stepper_PinConfigurationType;

typedef enum
{
    STEPPER_CHANNEL_0 = 0u,
    STEPPER_CHANNEL_1,
    STEPPER_CHANNEL_2,
    STEPPER_MAX_NUM_OF_STEPPERS
} Stepper_ChannelType;

typedef enum
{
    STEPPER_DIR_CW = 0u,
    STEPPER_DIR_CCW
} Stepper_DirectionType;

typedef struct
{
    Stepper_DirectionType Dir_e;
    uint16                Speed_ui16;
    uint16                StepCounter_ui16;
} Stepper_SingleMotorType;

typedef struct
{
    Stepper_SingleMotorType Stepper_s[STEPPER_MAX_NUM_OF_STEPPERS];
} Stepper_DataType;

void Stepper_Init(void);
void Stepper_Handler(void);
void Stepper_SetSpeed(Stepper_ChannelType ch, uint16 speed);
void Stepper_SetDir(Stepper_ChannelType ch, Stepper_DirectionType dir);
void Stepper_SetSpeedAndDir(Stepper_ChannelType ch, sint16 speed_si16);

#endif /* STEPPER_H_ */
