#include "User_CAN.h"
#include "can.h"


static BOOL CAN1_RX_Manage (CanFrame RXCanFrame);//CAN接收处理函数

//CAN初始化函数
void User_CAN1_Init(void)
{
    CAN_FilterTypeDef FilterConfig;

    /*配置CAN过滤器*/
    FilterConfig.FilterBank = 0;                      //过滤器组号
    FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  //过滤模式：屏蔽位模式--CAN_FILTERMODE_IDMASK，列表模式--CAN_FILTERMODE_IDLIST
    FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; //过滤器位宽：32位
    FilterConfig.FilterIdHigh = 0x0000;               //32位ID
    FilterConfig.FilterIdLow = 0x0000;
    FilterConfig.FilterMaskIdHigh = 0x0000;           //32位MASK
    FilterConfig.FilterMaskIdLow = 0x0000;
    FilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; //过滤器0关联到FIFO0
    FilterConfig.FilterActivation = ENABLE;           //激活滤波器0
    FilterConfig.SlaveStartFilterBank = 14;           //单CAN此参数无意义
    //过滤器配置
    if (HAL_CAN_ConfigFilter(&hcan1, &FilterConfig) != HAL_OK)
    {
    Error_Handler();
    }

    //启动CAN外围设备
    if(HAL_CAN_Start(&hcan1) != HAL_OK) 
    {
    Error_Handler();
    }
    //激活CAN接收中断
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
    Error_Handler();
    }
}


//CAN接收回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    CAN_RxHeaderTypeDef     RxHeader;      //接收
    CanFrame                RXCanFrame;

    if(HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RXCanFrame.Data) == HAL_OK)
    {
        RXCanFrame.IDE = RxHeader.IDE;      //标识符类型：CAN_ID_STD标准帧,CAN_ID_EXT扩展帧
        if( RXCanFrame.IDE == CAN_ID_EXT) 
        {
            RXCanFrame.ID = RxHeader.ExtId; //扩展帧ID (29位:0x00000000~0x1FFFFFFF)
        }
        else
        {
            RXCanFrame.ID = RxHeader.StdId; //标准帧ID (11位:0x00~0x7FF)
        }
        RXCanFrame.RTR = RxHeader.RTR;      //帧类型：CAN_RTR_DATA数据帧,CAN_RTR_REMOTE远程帧
        RXCanFrame.DLC = RxHeader.DLC;      //CAN报文数据长度
        
        CAN1_RX_Manage (RXCanFrame);        //CAN接收处理函数
    }
    else
    {
        Error_Handler();
    }
}

//CAN1发送数据函数
BOOL CAN1_TX_Data (CanFrame TXCanFrame)
{
    BOOL result = TRUE;
    
    CAN_TxHeaderTypeDef  TXHeader;  //can发送信息的结构体
    uint32_t    mailbox;       
    
    if(TXCanFrame.IDE == CAN_ID_EXT)  //CAN报文ID
    {
        TXHeader.StdId = 0;
        TXHeader.ExtId = TXCanFrame.ID;
    }
    else
    {
        TXHeader.StdId = TXCanFrame.ID;
        TXHeader.ExtId = 0;
    }
    
    TXHeader.IDE = TXCanFrame.IDE;    //标识符类型：CAN_ID_STD标准帧,CAN_ID_EXT扩展帧
    TXHeader.RTR = TXCanFrame.RTR;    //帧类型：CAN_RTR_DATA数据帧,CAN_RTR_REMOTE远程帧
    TXHeader.DLC = TXCanFrame.DLC;    //CAN报文数据长度
    TXHeader.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &TXHeader, TXCanFrame.Data, &mailbox) != HAL_OK) 
    {
        result = FALSE;
    }

    return result;
}




//CAN1接收处理函数
static BOOL CAN1_RX_Manage (CanFrame RXCanFrame)
{
    BOOL result = TRUE;
    
    CanFrame  TXCanFrame;
    TXCanFrame.ID = 0x18888888;     //帧ID
    TXCanFrame.IDE = CAN_ID_EXT;    //扩展帧
    TXCanFrame.RTR = CAN_RTR_DATA;  //数据帧
    TXCanFrame.DLC = 4;             //数据长度
    
    TXCanFrame.Data[0] = RXCanFrame.ID>>24;
    TXCanFrame.Data[1] = RXCanFrame.ID>>16;
    TXCanFrame.Data[2] = RXCanFrame.ID>>8;
    TXCanFrame.Data[3] = RXCanFrame.ID;
    
    
    result = CAN1_TX_Data (TXCanFrame);
    return result;
}



void CAN1_TX_0x12345678(void)
{
    CanFrame  TXCanFrame;
    TXCanFrame.ID = 0x12345678;     //帧ID
    TXCanFrame.IDE = CAN_ID_EXT;    //扩展帧
    TXCanFrame.RTR = CAN_RTR_DATA;  //数据帧
    TXCanFrame.DLC = 8;             //数据长度
    
    TXCanFrame.Data[0] = 1;
    TXCanFrame.Data[1] = 2;
    TXCanFrame.Data[2] = 3;
    TXCanFrame.Data[3] = 4;
    TXCanFrame.Data[4] = 5;
    TXCanFrame.Data[5] = 6;
    TXCanFrame.Data[6] = 7;
    TXCanFrame.Data[7] = 8;
    
    CAN1_TX_Data (TXCanFrame);
}








