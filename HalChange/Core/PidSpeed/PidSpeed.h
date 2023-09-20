#ifndef __PIDSPEED_H
#define __PIDSPEED_H
#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "tim.h"
#include "math.h"
#include "stdio.h"
#include "usart.h"

#define AIN1_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define AIN1_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define AIN2_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)
#define AIN2_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)
#define BIN1_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define BIN1_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)
#define BIN2_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)
#define BIN2_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)

typedef struct{
        float speed_kp;
        float speed_ki;
        float speed_kd;
        float bias;
        float last_bias;
        int PwmOut;
        float TargetSpeed;
    }PidSpeed;

int PidSpeed_PwmGet(int Encoder,int Target);
void PidSpeed_Init(void);
int Pwm_Limit(int Pwm);
void Set_PwmOut(int Pwm_A,int Pwm_B);

extern int MotorSpeed_B;
extern uint8_t TX_BUF[100];

#ifdef __cplusplus
}
#endif
#endif
