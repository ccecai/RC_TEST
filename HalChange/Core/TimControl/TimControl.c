#include "TimControl.h"
extern int circle;
extern float CurrentPosition;
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static unsigned char i = 0;
    if (htim == (&htim4))
    {
        Car_Control();
    }
}
*/
void Car_Control(void)
{
    Pwm = PidPosition_PwmGet();
    Pwm = Velocity_Restrict(Pwm, 20);
    Pwm = Pwm / 76;
    PwmOut = PidSpeed_PwmGet(MotorSpeed_B, Pwm);
    PwmOut = Pwm_Limit(PwmOut);
    AIN1_LOW;
    AIN2_HIGH;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PwmOut);
}
void Encoder_Control(void)
{
    MotorSpeed_B = -(short) (__HAL_TIM_GET_COUNTER(&htim2)*2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    CurrentPosition+=MotorSpeed_B;
}