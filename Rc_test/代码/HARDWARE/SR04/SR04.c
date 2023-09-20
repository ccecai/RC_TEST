#include "SR04.h"

/**************************************************************************
 作  者 ：大鱼电子
 淘宝地址：https://shop119207236.taobao.com
 微信公众号【大鱼机器人】
 后台回复【平衡小车】：获取平衡小车全套DIY资料
 后台回复【电子开发工具】：获取电子工程师必备开发工具
 后台回复【电子设计资料】：获取电子设计资料包
 知乎：张巧龙 
**************************************************************************/
#define	TRIG_PIN       GPIO_Pin_10   //TRIG       
#define	ECHO_PIN       GPIO_Pin_11		//ECHO   
uint8_t lock =0;
/*
 * 函数名：SR04_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void SR04_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					 //PB10接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOB, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //PB11接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(GPIOB,&GPIO_InitStructure);						 //初始化GPIOB
	
	 //GPIOB.11	  中断线以及中断初始化配置
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
}


void EXTI15_10_IRQHandler(void)
{
	u8 fac_us=SystemCoreClock/8000000;
	u32 tnow;
	float tcnt=0;
	Flag_Direction=16;
	delay_us(10);		                      //延时10us
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
			SysTick->LOAD=65535;					//LOAD的值
			SysTick->VAL=65535;        					//刚进入时的计数器值
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	
			while(GPIO_ReadInputDataBit(GPIOB,ECHO_PIN));	       				//等待低电平
			tnow=SysTick->VAL;
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
			tcnt+=65535-tnow;
			SR04_Distance=tcnt*340/fac_us/1000/7.25;			//计算距离，未完成待修正
	}
	EXTI_ClearITPendingBit(EXTI_Line11);  //清除EXTI11线路挂起位
	lock=0;
}		

/*
 * 函数名：SR04_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void SR04_StartMeasure(void)
{	
	if(lock==1) return;
  GPIO_SetBits(GPIOB,TRIG_PIN); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有说
  delay_us(20);		                      //延时20US
  GPIO_ResetBits(GPIOB,TRIG_PIN);
	lock=1;
}

/******************* (C) 1209 Lab *****END OF FILE************/
