#include "PidSpeed.h"

float speed_kp=360;
float speed_ki=1.5;
float speed_kd;

int PidSpeed_PwmGet(int Encoder,int Target)
{
    int bias;
    static int PwmOut,last_bias;
    bias = Target-Encoder;
    PwmOut += speed_kp*(bias-last_bias)+speed_ki*bias;
    last_bias = bias;

    return PwmOut;
}

int Pwm_Limit(int Pwm)
{
    if(Pwm > 7000) return 7000;
    else if(Pwm < 0) return 0;
}

void Set_PwmOut(int Pwm_A,int Pwm_B)
{
    if(Pwm_A > 0)
    {
        AIN1_LOW;
        AIN2_HIGH;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Pwm_A);
    }
    else if(Pwm_A < 0)
    {
        AIN1_HIGH;
        AIN2_LOW;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, -Pwm_A);
    }
    else
    {
        AIN1_HIGH;
        AIN2_HIGH;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Pwm_A);
    }

    if(Pwm_B > 0)
    {
        BIN1_LOW;
        BIN2_HIGH;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Pwm_B);
    }
    else if(Pwm_B < 0)
    {
        BIN1_HIGH;
        BIN2_LOW;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, -Pwm_B);
    }
    else
    {
        BIN1_HIGH;
        BIN2_HIGH;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Pwm_B);
    }

}

void PidSpeed_Init(void)
{
    PidSpeed speed;
    speed.speed_kp = 360;
    speed.speed_ki = 1.5;
    speed.speed_kd = 0;
    speed.bias = 0;
    speed.last_bias = 0;
    speed.PwmOut = 0;
    speed.TargetSpeed = 0;
}