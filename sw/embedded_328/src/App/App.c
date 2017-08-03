/*
 * App.c
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */

#include <string.h>
#include "App.h"
#include "Lcd.h"
#include "RotaryEncoder.h"
#include "Caliper.h"
#include "Heating.h"
#include "Led.h"
#include "Stepper.h"
#include <avr/io.h>

static void App_GetInputs(void);

App_DataType App_Data_s;
extern Caliper_DataType Caliper_Data_s;
extern Heating_DataType Heating_Data_s;

void App_Init(void)
{
    App_Data_s.HeatingPercentage_f32 = 0.0;

    App_Data_s.Temperature_s.Temperature_f32 = 0.0;
    App_Data_s.Temperature_s.Classifier_e = APP_CLASSIFIER_NEW_DATA;

    App_Data_s.RotaryEncoder_s.ClassifierButton_e = APP_CLASSIFIER_NEW_DATA;
    App_Data_s.RotaryEncoder_s.ClassifierRot_e = APP_CLASSIFIER_NEW_DATA;
    App_Data_s.RotaryEncoder_s.Data_s.State_s = ROTARY_ENCODER_BTN_NOT_PRESSED;
    App_Data_s.RotaryEncoder_s.Data_s.Value_si8 = 0;

    App_Data_s.RotaryEncoderNew_s.ClassifierButton_e = APP_CLASSIFIER_NEW_DATA;
    App_Data_s.RotaryEncoderNew_s.ClassifierRot_e = APP_CLASSIFIER_NO_NEW_DATA;
    App_Data_s.RotaryEncoderNew_s.Data_s.State_s = ROTARY_ENCODER_BTN_NOT_PRESSED;
    App_Data_s.RotaryEncoderNew_s.Data_s.Value_si8 = 0;

    App_Data_s.Caliper_s.Classifier_e = APP_CLASSIFIER_NEW_DATA;
    App_Data_s.Caliper_s.Value_f32 = 0.0f;
}

void App_Handler(void)
{
    App_GetInputs();
    App_StatusScreen();
}

static void App_GetInputs(void)
{
    if(Caliper_Data_s.Measurement.Result_f32 != App_Data_s.Caliper_s.Value_f32)
    {
        App_Data_s.Caliper_s.Value_f32 = Caliper_Data_s.Measurement.Result_f32;
        App_Data_s.Caliper_s.Classifier_e = APP_CLASSIFIER_NEW_DATA;
    }

    if(App_Data_s.RotaryEncoderNew_s.Data_s.State_s != App_Data_s.RotaryEncoder_s.Data_s.State_s)
    {
        App_Data_s.RotaryEncoder_s.Data_s.State_s = App_Data_s.RotaryEncoderNew_s.Data_s.State_s;
        App_Data_s.RotaryEncoder_s.ClassifierButton_e = APP_CLASSIFIER_NEW_DATA;
    }

    if(App_Data_s.RotaryEncoderNew_s.Data_s.Value_si8 != App_Data_s.RotaryEncoder_s.Data_s.Value_si8)
    {
        App_Data_s.RotaryEncoder_s.Data_s.Value_si8 = App_Data_s.RotaryEncoderNew_s.Data_s.Value_si8;
        App_Data_s.RotaryEncoder_s.ClassifierRot_e = APP_CLASSIFIER_NEW_DATA;
    }

    if(Heating_Data_s.ActualTemperature_f32 != App_Data_s.Temperature_s.Temperature_f32)
    {
        App_Data_s.Temperature_s.Temperature_f32 = Heating_Data_s.ActualTemperature_f32;
        App_Data_s.Temperature_s.Classifier_e = APP_CLASSIFIER_NEW_DATA;
    }

}

void App_AppSetOutputs(void)
{

}

void App_StatusScreen(void)
{
    uint8 string[30];
    if(App_Data_s.RotaryEncoder_s.ClassifierRot_e == APP_CLASSIFIER_NEW_DATA)
    {
        sprintf(string, "Rot: %i  ", (sint8)App_Data_s.RotaryEncoder_s.Data_s.Value_si8);
        Lcd_StringAtPosition(string, 0, 1);
        Stepper_SetSpeedAndDir(STEPPER_CHANNEL_0, (sint16)App_Data_s.RotaryEncoder_s.Data_s.Value_si8);
        Stepper_SetSpeedAndDir(STEPPER_CHANNEL_1, (sint16)App_Data_s.RotaryEncoder_s.Data_s.Value_si8);
        Stepper_SetSpeedAndDir(STEPPER_CHANNEL_2, (sint16)App_Data_s.RotaryEncoder_s.Data_s.Value_si8);
    }

    if(App_Data_s.RotaryEncoder_s.ClassifierButton_e == APP_CLASSIFIER_NEW_DATA)
    {
        App_Data_s.RotaryEncoder_s.ClassifierButton_e = APP_CLASSIFIER_NO_NEW_DATA;
        if(App_Data_s.RotaryEncoder_s.Data_s.State_s == ROTARY_ENCODER_BTN_PRESSED)
        {
            Lcd_StringAtPosition("Pressed ", 11, 1);
        }
        else
        {
            Lcd_StringAtPosition("Released", 11, 1);
        }
    }

    if(App_Data_s.Caliper_s.Classifier_e == APP_CLASSIFIER_NEW_DATA)
    {
        App_Data_s.Caliper_s.Classifier_e = APP_CLASSIFIER_NO_NEW_DATA;
        Lcd_StringAtPosition("d: ", 0, 3);
        Lcd_PrintFloat(Caliper_Data_s.Measurement.Result_f32, 2, 3);
        Lcd_StringAtPosition("mm", 7, 3);
    }

    if(App_Data_s.Temperature_s.Classifier_e == APP_CLASSIFIER_NEW_DATA)
    {
        App_Data_s.Temperature_s.Classifier_e = APP_CLASSIFIER_NO_NEW_DATA;
        Lcd_StringAtPosition("T:", 10, 3);
        Lcd_PrintFloat(Heating_Data_s.ActualTemperature_f32, 12, 3);
        Lcd_Data((char)223);
        Lcd_Data('C');
        if(Heating_Data_s.ActualTemperature_f32 < 100.0)
        {
            Lcd_Data(' ');
        }
    }
    Led_SetState(LED_STATE_BLINKING_FAST);
    //Led_SetState(LED_STATE_BLINKING_SLOW);
//    Led_SetState(LED_STATE_OFF);
    //Led_SetState(LED_STATE_ON);

}
