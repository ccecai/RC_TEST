#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
/**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
 微信公众号【大鱼机器人】
 后台回复【平衡小车】：获取平衡小车全套DIY资料
 后台回复【电子开发工具】：获取电子工程师必备开发工具
 后台回复【电子设计资料】：获取电子设计资料包
 知乎：张巧龙 
**************************************************************************/
#define PI 3.14159265
#define C1 PBin(4)
#define C2 PBin(3)
#define C3 PAin(5)
#define C4 PAin(4)

void Kinematic_Analysis(int velocity,int turn);
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
void Get_RC(void);
void Tracking(void);
void TIM4_IRQHandler(void);
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity);

int Position_FeedbackControl(float Circle, int CurrentPosition);
int Velocity_Restrict(int PWM_P, int TargetVelocity);
#endif
