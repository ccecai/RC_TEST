#ifndef __PIDPOSITION_H
#define __PIDPOSITION_H
#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "stdio.h"
#include "usart.h"

typedef struct{
    float position_kp;
    float position_ki;
    float position_kd;
    float bias;
    float integral_bias;
    float last_bias;
    float TargetPosition;
    float CurrentPosition;
    float Velocity;
    int TargetVelocity;
    int circle;
    int PwmOut;
}PidPosition;

float PidPosition_PwmGet(void);
void PidPosition_Init(void);
int Velocity_Restrict(int PWM_P, int TargetVelocity);
extern uint8_t TX_BUF[100];

#ifdef __cplusplus
}
#endif
#endif
