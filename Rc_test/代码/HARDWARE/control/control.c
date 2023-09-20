#include "control.h"	
#include "usart2.h"
/**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
 微信公众号【大鱼机器人】
 后台回复【平衡小车】：获取平衡小车全套DIY资料
 后台回复【电子开发工具】：获取电子工程师必备开发工具
 后台回复【电子设计资料】：获取电子设计资料包
 知乎：张巧龙 
**************************************************************************/
/**************************************************************************
函数功能：所有的控制代码都在这里面
         Timer3 10ms定时中断
**************************************************************************/
//根据自己的电机选择，只能有一个电机类型define生效
#define GA12N20
//#define GM25310
//#define TT130
//#define GB37520

#ifdef GB37520
#define E_DIR 1
#define K_DIR -1
#endif

#ifdef TT130
#define E_DIR -1
#define K_DIR -1
#endif

#ifdef GA12N20
#define E_DIR -1
#define K_DIR -1
#endif

#ifdef GM25310
#define E_DIR 1
#define K_DIR 1
#endif

#define SPEED_Y 40 //前后最大设定速度
#define SPEED_Z 100//左右最大设定速度 
int   TargetVelocity=20, TargetCircle=0, CurrentPosition = 0, PWM = 0;  //目标速度、目标圈数(位置)、编码器读数、PWM控制变量
float Position_Kp=8, Position_Ki=0.23, Position_Kd=3; //相关位置PID参数
int delay_flag = 0;
int count,TimeCount;
extern int mode;

float velocity_KP= 360;     // 小车速度环PI参数
float velocity_KI= 1.5;
int i = 0;
int Position_FeedbackControl(float Circle, int CurrentPosition);
int flag = 1;
int flag_ = 1,flag__ = 1;
void TIM4_IRQHandler(void)   //TIM4中断
{
	static u8 Voltage_Counter=0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		  Led_Flash(100);
			
				Encoder_Left=-Read_Encoder(3);
			
		  Encoder_Right=-Read_Encoder(2);                           //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
			usart2_send(Encoder_Left);
			                           //===读取编码器的值
		  CurrentPosition+=Encoder_Left;
			Voltage_Counter++;
			if(Voltage_Counter==10)									 //===100ms读取一次超声波的数据以及电压
			{
				Voltage_Counter=0;
				Voltage=Get_battery_volt();		                         //===读取电池电压		
			}
			if(delay_flag == 1) // 给其他函数精确延迟
			{
				count++;
				if(count == 3)
				{
					delay_flag = 0;
					count = 0;
				}
			}
			if(mode <= 5)
			{
				Moto2=Incremental_PI_B(Encoder_Right,Target_B);                 //===计算右轮电机最终PWM
				Xianfu_Pwm();  																					 //===PWM限幅
				Turn_Off(Voltage);																 //===检查角度以及电压是否正常
				Set_Pwm(0,Moto2);                                    //===赋值给PWM寄存器  
			}
			else if(mode == 7 || mode == 8 ||mode == 9){
				PWM=Position_FeedbackControl(TargetCircle, CurrentPosition); //位置闭环控制
				Set_Pwm(PWM,0); 
				
			}
			else if(mode == 11 || mode == 12 || mode ==13)
			{
				PWM=Position_FeedbackControl(TargetCircle, CurrentPosition); //位置闭环控制
				PWM=Velocity_Restrict(PWM,TargetVelocity);
				PWM = PWM/76;
				Moto1=Incremental_PI_A(Encoder_Left,PWM);
				Moto1 = -Moto1;
				
					Set_Pwm(Moto1,0);
					
				
			}
			else if(mode == 15)
			{
				Moto1 = Incremental_PI_A(-Encoder_Left,Encoder_Right);
				Xianfu_Pwm();  																					 //===PWM限幅
				Turn_Off(Voltage);																 //===检查角度以及电压是否正常
				Set_Pwm(Moto1,0);
			}
			else
			{
				Target_A = 0;
				Target_B = 0;
				Set_Pwm(0,0);
			}
	}
}

/**************************************************************************
函数功能：PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=velocity_KP*(Bias-Last_bias)+velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=velocity_KP*(Bias-Last_bias)+velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：目标圈数(位置) 当前位置
返回  值：速度控制值
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_FeedbackControl(float Circle, int CurrentPosition)
{
		float TargetPosition,Bias, ControlVelocity;     //定义相关变量
		static float Last_bias, Integral_Bias;          //静态变量，函数调用结束后其值依然存在
		
			TargetPosition=Circle*1560;
		
		
	   //目标位置=目标圈数*1040
	                                   //10ms读取一次编码器(即100HZ)，电机减速比为20，霍尔编码器精度13，AB双相组合得到4倍频，
	                                   //则转1圈编码器读数为20*13*4=1040，电机转速=Encoder*100/1040r/s 使用定时器2
	                                   //1.04是误差系数，电机本身存在误差，可根据实际情况调整该系数以提高控制精度
		Bias=TargetPosition-CurrentPosition; //求位置偏差
		
	  Integral_Bias+=Bias;
    if(Integral_Bias> 750) Integral_Bias= 750;	//积分限幅 防止到达目标位置后过冲
	  if(Integral_Bias<-750) Integral_Bias=-750;	//积分限幅 防止到达目标位置后过冲
	
		ControlVelocity=Position_Kp*Bias+Position_Ki*Integral_Bias+Position_Kd*(Bias-Last_bias);  //增量式PI控制器
	                                                                                            //Position_Kp*Bias 偏差越大速度越大
	                                                                                            //Position_Ki*Integral_Bias 减小稳态误差
	                                                                                            //Position_Kd*(Bias-Last_bias) 限制速度
		Last_bias=Bias;	
		return ControlVelocity;    //返回速度控制值 
}

int Velocity_Restrict(int PWM_P, int TargetVelocity)
{
    if     (PWM_P>+TargetVelocity*76) PWM_P=+TargetVelocity*76;
	  else if(PWM_P<-TargetVelocity*76) PWM_P=-TargetVelocity*76;
	  else PWM_P=PWM_P;
	  return PWM_P;
}


