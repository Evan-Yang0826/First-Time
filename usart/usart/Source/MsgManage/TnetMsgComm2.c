/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    TnetMsgComm2.c
  **  描    述:    
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2019/8/5   Devil    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"


/* 本地头文件 */
#define TNETMSGCOMM2_LOCAL

#include "allinclude.h"


struct
{
	INT8U index;
	INT8U sendaddr;
	INT8U revaddr;
	INT8U Fig;
}AskStatus1;

static INT32U MsgOutTime;

/*************************************************************************************************
** 函数名称:    TnetMsgComm2_Init
** 功能描述:      
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:      
** 调用模块:      
**--------------------------------------------------------------------------------------------
** 修改记录:      
** 2019/8/5   Devil    创建
*************************************************************************************************/
void TnetMsgComm2_Init(void)
{

}

/*************************************************************************************************
** 函数名称:    MsgSendtoBus
** 功能描述:	T-Net 协议 发送函数
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void MsgSendtoBusCom1(MsgType *msg)
{
    INT8U data[100];
    INT32U i;
	INT32U SendLong = 7;

	if((msg->MsgSource == MsgCom2) || (msg->MsgSource == MsgComAll))
	{
		msg->Sendaddr = MyAddr;
	}
    data[0] = 0xFD;
    data[1] = 0x7E;
    data[2] = msg->Sendaddr;
	data[3] = msg->Revaddr;
	data[4] = msg->cmd;
	data[5] = msg->index;
    data[6] = msg->Long;


    for(i=0; i < msg->Long; i++)
    {
        data[SendLong] = msg->data[i];
        SendLong++;
    }

	msg->chk = 0;
	for(i=2;i<SendLong;i++)
	{
		msg->chk += data[i];
	}
	
	data[SendLong++] = msg->chk;
    data[SendLong++] = 0xFC;
    data[SendLong++] = 0x0D;
	
    //  等待总线空闲
//    while(Usart3RevBusyFig)
//    {
//        CountTime++;
//        if(CountTime > 200)
//        {
//            break;
//        }
//        delay_ms(1);
//    }
	delay_ms(2);
    STM32_U2_Write(data,SendLong);
	
	AskStatus1.Fig = 0;
	AskStatus1.index = msg->index;
	AskStatus1.sendaddr = msg->Sendaddr;
	AskStatus1.revaddr = msg->Revaddr;
}
/*************************************************************************************************
** 函数名称:    MsgRevice_Task
** 功能描述:	T-Net协议解析任务
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void MsgRevice_TaskCom1(void)
{
    INT32U i,j;
    static INT8U DataTemp[MAX_REVICE_NUM];
    static INT32U index = 0;
    static INT32U dataindex = 0;
    static INT32U Long;
    static MsgType msg;

    Long = STM32_U2_Read(DataTemp,MAX_REVICE_NUM);
    if(Long > 0)
    {
        
        for(i=0; i<Long; i++)
        {
            switch(index)
            {
            case 0:
                // 包头
                if(DataTemp[i] == 0xFD)
                {
                    index = 1;
					// 清除接收超时定时器
					MsgOutTime = 0;
                }
				else
				{
					index = 0;
				}
                break;

            case 1:
                // 包头
                if(DataTemp[i] == 0x7E)
                {
                    index = 2;

                    // 初始化消息缓存
                    for(j=0; j<sizeof(MsgType); j++)
                    {
                        *((INT8U*)&msg+j) = 0;
                    }
                    dataindex = 0;
					
					
                }
				else
				{
					index = 0;
				}
                break;

            case 2:
				msg.Sendaddr = DataTemp[i];
                msg.chk += DataTemp[i];
                index = 3;
                break;

			case 3:
				msg.Revaddr = DataTemp[i];
                msg.chk += DataTemp[i];
                index = 4;
                break;
			
			case 4:
				msg.cmd = DataTemp[i];
                msg.chk += DataTemp[i];
                index = 5;
                break;
			
			case 5:
				msg.index = DataTemp[i];
                msg.chk += DataTemp[i];
                index = 6;
                break;
			
            case 6:
                msg.Long = DataTemp[i];
                msg.chk += DataTemp[i];
				if(msg.Long >= MAX_MSGDATA)
				{
					index = 0;
				}
				else if(msg.Long == 0)
				{
					index = 8;
				}
				else
				{
					index = 7;
				}
                break;

            case 7:
                msg.data[dataindex] = DataTemp[i];
                msg.chk += DataTemp[i];
                dataindex ++;
                if(dataindex >= msg.Long)
                {
                    index = 8;
                }
                break;

            case 8:
                if(msg.chk == DataTemp[i])
				{
					index = 9;
				}
				else
				{
					index = 0;
				}
				break;

            case 9:
                if(DataTemp[i] == 0xFC)
                {
                    index = 10;
                }
				else
				{
					index = 0;
				}
                break;

            case 10:
                index = 0;
                if(DataTemp[i] == 0x0D)
                {
					if(msg.Revaddr  != MyAddr)
					{
						msg.MsgSource = MsgCom2_1;
						MsgSend(&msg,FALSE);
						
					}
					msg.MsgSource = MsgCom2;
					
                     // 校验成功，存储消息
					if(msg.cmd == 0x00)
					{
						// 应答消息
						
						if(msg.Sendaddr == AskStatus1.revaddr)
						if(msg.Revaddr  == MyAddr)
						if(msg.index    == AskStatus1.index)
						{
							AskStatus1.Fig = 1;
							break;
						}							
					}	
					#if(DEVICE_TYPE != TYPE_CONTROL_TOTAL_SCENE) 	
                    else if(((msg.Revaddr == MyAddr) || (msg.Revaddr == 0xFE)) && (msg.Sendaddr != 0x01))
					#endif
					#if(DEVICE_TYPE == TYPE_CONTROL_TOTAL_SCENE)
					else if((msg.Revaddr == MyAddr) || (msg.Revaddr == 0xFE))
					#endif
                    {
                        MsgInBuf(&MsgRevicebuf,&msg);
						
						// 如果消息需要应答，直接应答					
						if((msg.index > 0) && (msg.Revaddr != 0xFE))
						{
							msg.data[0] = msg.index;
							msg.Revaddr = msg.Sendaddr;
							msg.Long = 1;
							msg.cmd = 0x00;
							
							MsgSend(&msg,FALSE);
						}
                    }
					#if(DEVICE_TYPE != TYPE_CONTROL_TOTAL_SCENE) 	
                    else if(msg.Sendaddr != 0x01)
					#endif
					#if(DEVICE_TYPE == TYPE_CONTROL_TOTAL_SCENE)
					else 
					#endif
					{
						MsgInBuf(&MsgOtherbuf,&msg);
					}
					
					
                }
                break;
            }
        }
    }
    if(MsgOutTime > MAX_WAIT_TIME)
    {
        if(index != 0)
        {
            index = 0;
            dataindex =0;
        }
    }
}

/*************************************************************************************************
** 函数名称:    MsgSend_Task
** 功能描述:
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void MsgSend_TaskCom2(void)
{
	static MsgType msg;
	static INT32U sendwaittime = 0;
	static INT8U SendIndex = 1;
	static INT8U sendStatus = 0;
	static INT8U SendCount = 0;
	
	switch(sendStatus)
	{
		default:
		case 0:
			if(MsgOutBuf(&MsgSendbuf1,&msg) == TRUE)
			{
				
				if(msg.AutoRepeat == 0)
				{
					// 0x00 为应答消息
					if(msg.cmd != 0x00) msg.index = 0;
					sendwaittime = 20;
					MsgSendtoBusCom1(&msg);
					sendStatus = 3;
				}
				
				else
				{
					msg.index = SendIndex++;
					if(SendIndex == 0) SendIndex=1;
					MsgSendtoBusCom1(&msg);
					// 延时 等待应答
					sendwaittime = (rand() % 170) + 30;
					sendStatus = 2;
					SendCount = 1;
					
				}
				
			}
			break;
			
		case 1:
			msg.index = SendIndex++;
			if(SendIndex == 0) SendIndex=1;
		
			MsgSendtoBusCom1(&msg);
		
			// 延时 等待应答
			sendwaittime = (rand() % 170) + 30;
			sendStatus = 2;
			SendCount++;		
			break;
			
		case 2:
			if(AskStatus1.Fig == 1)
			{
				// 收到应答
				sendStatus = 0;
			}
			if(sendwaittime > 0)
			{
				sendwaittime--;
				if(sendwaittime == 0)
				{
					if(SendCount >= MAX_SEND_COUNT)
					{
						//  超过最大发送次数
						sendStatus = 0;
					}
					else
					{
						sendStatus = 1;
					}
				}
			}
			break;
		case 3:
			// 已发送一条消息，等待20ms 后才可重发
			if(sendwaittime > 0)
			{
				sendwaittime--;
				if(sendwaittime == 0)
				{
					sendStatus = 0;
				}
			}
			break;
	}	
	
}


/*************************************************************************************************
** 函数名称:    TnetMsgComm2_Task
** 功能描述:      
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:      
** 调用模块:      
**--------------------------------------------------------------------------------------------
** 修改记录:      
** 2019/8/5   Devil    创建
*************************************************************************************************/
void TnetMsgComm2_Task(void)
{
	
	
	if(GET_MS_SIG(TNETMSGMANAGE1_MS) > 0)
    {
        CLR_MS_SIG(TNETMSGMANAGE1_MS);
		MsgRevice_TaskCom1();
		MsgSend_TaskCom2();
	}
}

/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
