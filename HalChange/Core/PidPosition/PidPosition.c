#include "PidPosition.h"

float position_kp;
float position_ki;
float position_kd;
float CurrentPosition;
int circle;

float PidPosition_PwmGet(void)
{
    float bias,TargetPosition,Velocity;
    static float integral_bias,last_bias;
    TargetPosition=circle*1560;
    bias=TargetPosition-CurrentPosition;
    integral_bias+=bias;
    if(integral_bias> 900) integral_bias= 900;
    if(integral_bias<-900) integral_bias=-900;

    Velocity=position_kp*bias+position_ki*integral_bias+position_kd*(bias-last_bias);

    last_bias=bias;
    return Velocity;
}

int Velocity_Restrict(int PWM_P, int TargetVelocity)
{
    if     (PWM_P>+TargetVelocity*76) PWM_P=+TargetVelocity*76;
    else if(PWM_P<-TargetVelocity*76) PWM_P=-TargetVelocity*76;
    else PWM_P=PWM_P;
    return PWM_P;
}

void PidPosition_Init(void)
{
    position_kp = 7.5;
    position_ki = 0.35;
    position_kd = 20;
    CurrentPosition = 0;
    circle = 10;
}