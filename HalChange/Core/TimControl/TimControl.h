//
// Created by 1 on 2023-09-16.
//

#ifndef __TIMCONTROl_H
#define __TIMCONTROl_H
#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "tim.h"
#include "stm32f1xx_hal_tim.h"
#include "PidSpeed.h"
#include "PidPosition.h"
#include "stdio.h"
#include "usart.h"

extern int MotorSpeed_A,MotorSpeed_B;
extern int MotorOutput_A,MotorOutput_B;
extern int PwmOut;
extern float Pwm;
extern uint8_t TX_BUF[100];

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Car_Control(void);
void Encoder_Control(void);

#ifdef __cplusplus
}
#endif
#endif