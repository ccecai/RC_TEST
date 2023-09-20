 /**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
 
 微信公众号【大鱼机器人】
 后台回复【平衡小车】：获取平衡小车全套DIY资料
 后台回复【电子开发工具】：获取电子工程师必备开发工具
 后台回复【电子设计资料】：获取电子设计资料包
 
 知乎：张巧龙 
**************************************************************************/
#include "sys.h"
/****************************全局变量*************************************/    
float Voltage;  															 //电池电压采样相关的变量
float SR04_Distance;                 //超声波测距
int   Encoder_Left,Encoder_Right;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0,Target_A=0,Target_B=0;												 //计算出来的最终赋给电机的PWM

u8 		CTRL_MODE=98,Mode_Change=0;   //在此选择小车模式  //97超声波避障；98蓝牙；99红外循迹；100 PS2；101自动跟随  

u8 		Flag_Direction=0;
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY; //PS2相关变量
u8 PS2_Plugin=0;

int Velocity=0,Turn=0;

int Uart_Receive=0;
u8 TkSensor=0;
/***********************************************************************/
int mode = 5;
extern int CurrentPosition,TargetCircle,flag,flag_,flag__,TargetVelocity;
extern float velocity_KP,velocity_KI;
int flag___ = 1,flag____ = 1;
int j = 1,arr = 0;
void Main_Init(void)
{
	LED_Init();                    //=====初始化与 LED 连接的IO
	KEY_Init();                    //=====初始化与按键连接的IO
	delay_init();	    	           //=====延时函数初始化	
	uart1_init(115200);	           //=====串口1初始化
//	uart3_init(9600);            //=====初始化串口3
	uart2_init(1200);            //=====初始化串口2
	
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	Adc_Init();                    //=====初始化ADC
	Encoder_Init_TIM2();           //=====初始化编码器2
	Encoder_Init_TIM3();           //=====初始化编码器4
	
	OLED_Init();
	OLED_Clear();
	oled_first_show();
	Motor_Init();									 //=====初始化与电机连接的硬件IO接口 
	
	Timer4_Init(100,7199);           //=====10MS进一次中断服务函数，中断服务函数在control.c
	TIM1_PWM_Init(7199,0);   			 //=====初始化PWM 10KHZ,用于驱动电机。 
	
	PWMA=0;
	PWMB=0;
	
	delay_ms(2000);
	
}

void Judge(void)
{
	if(mode == 1)
	{

		Target_A = 10;
		Target_B = 10;
	}
	else if(mode == 2)
	{
		Target_A = 50;
		Target_B = 50;
	}
	else if(mode == 3)
	{
		Target_A = 0;
		Target_B = 0;
		Set_Pwm(0,0);
	}
	else if(mode == 4)
	{
		Target_A = -10;
		Target_B = -10;
	}
	else if(mode == 5)
	{
		Target_A = -50;
		Target_B = -50;
	}
	else if(mode == 6)
	{
		Target_A = 0;
		Target_B = 0;
		Set_Pwm(0,0);
	}
	else if(mode == 7)
	{
		TargetCircle = -1;
	}
	else if(mode == 8)
	{
		if(flag_ == 1)
		{
			CurrentPosition = 0;
			flag_ = 0;
		}
		TargetCircle = 10;
	}
	else if(mode == 9)
	{
		if(flag____ == 1)
		{
			CurrentPosition = 0;
			flag____ = 0;
		}
		TargetCircle = 100;
	}
	else if(mode == 10)
	{
		
		Target_A = 0;
		Target_B = 0;
		Set_Pwm(0,0);
	}
	else if(mode == 11)
	{
		if(flag == 1)
		{
			CurrentPosition = 0;
			flag = 0;
		}
		
		TargetCircle = 1;
	}
	else if(mode == 12)
	{
		if(flag__ == 1)
		{
			CurrentPosition = 0;
			flag__ = 0;
		}
		TargetCircle = 20;
	}
	else if(mode == 13)
	{
		if(flag___ == 1)
		{
			CurrentPosition = 0;
			flag___ = 0;
			TargetVelocity = 0;
		}
		TargetCircle = 20;
		j++;
		if(j == 1000 && TargetVelocity < 20)
		{
			j = 0;
			TargetVelocity++;
		}
		
	}
	else if(mode == 14)
	{
		
		Target_A = 0;
		Target_B = 0;
		Set_Pwm(0,0);
	}
}


int main(void)
{
	
	Main_Init();

  while(1)	
	{
		
		printf("%d,%d\n",Target_B,Encoder_Right);
		oled_show();
		Judge();
		KEY_Press(1);
	}
}



