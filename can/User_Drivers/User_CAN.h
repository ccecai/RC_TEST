#ifndef __User_CAN_H
#define	__User_CAN_H

#include "main.h"



typedef struct _CanFrame_
{
    uint32_t    ID;         //CAN报文ID
    uint8_t     IDE;        //标识符类型：CAN_ID_STD标准帧,CAN_ID_EXT扩展帧
    uint8_t     RTR;        //帧类型：CAN_RTR_DATA数据帧,CAN_RTR_REMOTE远程帧
    uint8_t     DLC;        //CAN报文数据长度
    uint8_t     Data[8];    //CAN报文数据内容
}CanFrame;




void User_CAN1_Init(void);
BOOL CAN1_TX_Data (CanFrame TXCanFrame);
void CAN1_TX_0x12345678(void);







#endif





