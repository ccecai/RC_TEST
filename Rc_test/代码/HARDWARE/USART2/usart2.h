#ifndef __USRAT3_H
#define __USRAT3_H 
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

void uart2_init(u32 bound);
void USART2_IRQHandler(void);

void uart3_init(u32 bound);
void USART3_IRQHandler(void);
void BluetoothCMD(u8 Uart_Receive);
void usart2_send(u8 data);
#endif

