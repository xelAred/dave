/*
 * App.h
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */

#ifndef APP_H_
#define APP_H_

#include "Std_Types.h"
#include "RotaryEncoder.h"

typedef enum
{
    APP_CLASSIFIER_NO_NEW_DATA = 0u,
    APP_CLASSIFIER_NEW_DATA
} App_NewDataClassifierType;


typedef struct
{
    App_NewDataClassifierType Classifier_e;
    float32 Temperature_f32;
} App_TemperatureType;

typedef struct
{
    App_NewDataClassifierType ClassifierButton_e;
    App_NewDataClassifierType ClassifierRot_e;
    RotaryEncoder_EncodedValuesType Data_s;
} App_RotaryEncoderType;

typedef struct
{
    App_NewDataClassifierType Classifier_e;
    float32 Value_f32;
} App_CaliperType;

typedef struct
{
    float32 HeatingPercentage_f32;
    App_TemperatureType     Temperature_s;
    App_RotaryEncoderType   RotaryEncoder_s;
    App_RotaryEncoderType   RotaryEncoderNew_s;
    App_CaliperType         Caliper_s;
} App_DataType;




void App_Init(void);
void App_Handler(void);
void App_StatusScreen(void);

#endif /* APP_H_ */
