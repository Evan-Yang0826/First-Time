/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    MsgManage.c
  **  描    述:    RS485  通讯管理
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/11/2   Joshua    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
#include "stdlib.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"
#include "../YH_CLib/YH_Clibrary.h"


/* 本地头文件 */
#define MSGMANAGE_LOCAL
#include "allinclude.h"


struct
{
	INT8U index;
	INT8U sendaddr;
	INT8U revaddr;
	INT8U Fig;
}AskStatus;


static INT32U MsgOutTime;

/*************************************************************************************************
** 函数名称:    MsgInBuf
** 功能描述:
** 输　  入:    msgbuf 缓存区，msg 消息体
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U MsgInBuf(MsgBufType *msgbuf, MsgType *msg)
{
    INT32U InIndex = msgbuf->InIndex + 1;
    /*  数据入队列 */

    if(InIndex >= MAX_MEG_BUFF)
    {
        InIndex = 0;
    }

    if(InIndex == msgbuf->OutIndex)
    {
        return FALSE;
    }

    YHMemoryCopy((INT8U*)msg,(INT8U*)&msgbuf->Msg[InIndex],sizeof(MsgType));
    msgbuf->InIndex = InIndex;
	
	return TRUE;
}
/*************************************************************************************************
** 函数名称:    MsgOutBuf
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
INT8U MsgOutBuf(MsgBufType *msgbuf , MsgType *msg)
{
    /*  数据入队列 */

    if(msgbuf->InIndex == msgbuf->OutIndex)
    {
        /* 当前没有消息 */
        return FALSE;
    }

    msgbuf->OutIndex ++;
    if(msgbuf->OutIndex >= MAX_MEG_BUFF)
    {
        msgbuf->OutIndex = 0;
    }

    YHMemoryCopy((INT8U*)&msgbuf->Msg[msgbuf->OutIndex],(INT8U*)msg,sizeof(MsgType));

    return TRUE;
}
/*************************************************************************************************
** 函数名称:    MsgSend
** 功能描述:
** 输　  入:    msg：消息地址；RepeatFig ：TRUE-如果发生失败则自动重发  FALSE-禁用自动重发
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U  MsgSend(MsgType *msg,INT8U RepeatFig)
{


	msg->AutoRepeat = RepeatFig;
	
		
			MsgInBuf(&MsgSendbuf,msg);
		
	
	
	return 0;
	
}
/*************************************************************************************************
** 函数名称:    MsgRead
** 功能描述:	读取消息
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
INT8U MsgRead(MsgType *msg)
{
	return MsgOutBuf(&MsgRevicebuf,msg);
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
void MsgSendtoBus(MsgType *msg)
{
    INT8U data[100];
    INT32U i;
	INT32U SendLong = 3;
	
    data[0] = 0x55;
    data[1] = 0xAA;
    
    data[2] = msg->Long;


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
    data[SendLong++] = 0xaa;
    data[SendLong++] = 0x55;
	
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
    STM32_U1_Write(data,SendLong);
	
	AskStatus.Fig = 0;
	AskStatus.index = msg->index;

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
void MsgRevice_Task(void)
{
    INT32U i,j;
    static INT8U DataTemp[MAX_REVICE_NUM];
    static INT32U index = 0;
    static INT32U dataindex = 0;
    static INT32U Long;
    static MsgType msg;

    Long = STM32_U1_Read(DataTemp,MAX_REVICE_NUM);
    if(Long > 0)
    {
        
        for(i=0; i<Long; i++)
        {
            switch(index)
            {
            case 0:
                // 包头
                if(DataTemp[i] == 0x55)
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
                if(DataTemp[i] == 0xAA)
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
                msg.Long = DataTemp[i];
                
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
									//printf("check err\r\n");
									
									
									index = 0;
								}
								break;

            case 9:
                if(DataTemp[i] == 0xaa)
                {
                    index = 10;
                }
				else
				{
					dataindex = 0;
					index = 0;
				}
                break;

            case 10:
                index = 0;
						dataindex = 0;
                if(DataTemp[i] == 0x55)
                {
//									printf("check success\r\n");
//									printf("[0x55 0xaa ");
//									for(j=0;j<msg.Long;j++)
//									{
//										printf("0x%02x ",msg.data[j]);
//									}
//									printf("0xaa 0x55]\r\n");
									MsgInBuf(&MsgRevicebuf,&msg);
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
void MsgSend_Task(void)
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
			if(MsgOutBuf(&MsgSendbuf,&msg) == TRUE)
			{
			
			
					MsgSendtoBus(&msg);
					
				
			
			}
			break;
			
		
	}	
	
}
/*************************************************************************************************
** 函数名称:    MsgManage_Init
** 功能描述:      
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:      
** 调用模块:      
**--------------------------------------------------------------------------------------------
** 修改记录:      
** 2018/11/2   Joshua    创建
*************************************************************************************************/
void MsgManage_Init(void)
{
	// 初始化缓存
	YHMemoryClear(((INT8U*)&MsgSendbuf),sizeof(MsgBufType));
	YHMemoryClear(((INT8U*)&MsgRevicebuf),sizeof(MsgBufType));

	
}

void Cmd_Reviver(void)
{
	static MsgType msg;
	INT8U temp = 0xff;
	if(MsgOutBuf(&MsgRevicebuf,&msg) == TRUE)
	{
		#if Esp_Ap_Sta != (0) 
		switch(msg.data[0])
		{
		
			case 0x06:
				if(msg.data[1] == 0xff)
				{
				
					temp = chack_Mac_Voild(&msg.data[2]);
					if(temp  != 0xff)
					{
							 MsgType Msg_temp;
						Msg_temp.Long = 11;
						Msg_temp.data[0] = 0x03;
						YHMemoryCopy(esp_ap_info[temp].Mac_Address,&Msg_temp.data[1],6);
						YHMemoryCopy(esp_ap_info[temp].RFID_ID,&Msg_temp.data[7],4);
						
						MsgSend(&Msg_temp,0);
					}
				
				}
				else if(msg.data[1] == 0)
				{
					MsgType Msg_temp;
					Msg_temp.Long = 11;
					Msg_temp.data[0] = 0x03;
					YHMemoryCopy(Mac_Address_ben,&Msg_temp.data[1],6);
					YHMemoryCopy(SN,&Msg_temp.data[7],4);
					
					MsgSend(&Msg_temp,0);
				}
				break;
			case 0x05:
				LogicRun_Set(LogicCheck); 
				break;
			case 0x07:
					LogicRun_Set(LogicRunNormal); 
				break;
		}
		#endif
	}

}
/*************************************************************************************************
** 函数名称:    MsgManage_Task
** 功能描述:      
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:      
** 调用模块:      
**--------------------------------------------------------------------------------------------
** 修改记录:      
** 2018/11/2   Joshua    创建
*************************************************************************************************/
void MsgManage_Task(void)
{

    /* 定时处理任务 */
    if(GET_MS_SIG(LOGICCONTROL_MS) > 0)
    {
        CLR_MS_SIG(LOGICCONTROL_MS);
		
		MsgOutTime++;
		
		
		MsgRevice_Task();
		Cmd_Reviver();
		MsgSend_Task();
    }
}
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
