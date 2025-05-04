/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    MsgManage.h
  **  描    述:    RS485  通讯管理
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/11/2   Joshua    创建
************************************************************************************************/
#ifndef __MSGMANAGE_H__
#define __MSGMANAGE_H__

/* 本地预定义 */
#ifdef MSGMANAGE_LOCAL
    #define EXT_MSGMANAGE
#else
    #define EXT_MSGMANAGE    extern
#endif
	
/*  如发送失败，重发次数 */
#define MAX_SEND_COUNT			(3)
	
/*  最大缓存命令条数 */
#if (DEVICE_TYPE == TYPE_CONTROL_SIX_SCENE)
	
	#define MAX_MEG_BUFF			(30)
	
#else
	
	#define MAX_MEG_BUFF			(10)
	
#endif

/*  接收超时时间，单位毫秒 */
#define MAX_WAIT_TIME			(100)
	
/*  协议解析单次处理最大字节数 */
#define MAX_REVICE_NUM			(100)
	
/*  数据体最大数据长度 */
#define MAX_MSGDATA				(50)

typedef enum
{
	MsgCom1 = 0,
	MsgCom2,
	MsgCom1_2,
	MsgCom2_1,
	MsgComAll
}MsgSourceType;	

typedef struct 
{

	INT8U chk;
	INT8U Long;
	INT8U index;				
	INT8U AutoRepeat;				/* 0-此消息不用自动重发；1 - 此消息需要自动重发 */
	INT8U data[MAX_MSGDATA];
}MsgType;


typedef struct
{
	INT32U InIndex;
	INT32U OutIndex;
	MsgType Msg[MAX_MEG_BUFF];	
}MsgBufType;
EXT_MSGMANAGE MsgBufType MsgSendbuf,MsgRevicebuf;


EXT_MSGMANAGE MsgBufType MsgSendbuf1;


EXT_MSGMANAGE void MsgManage_Init(void);
EXT_MSGMANAGE void MsgManage_Task(void);

EXT_MSGMANAGE INT8U  MsgSend(MsgType *msg,INT8U RepeatFig);
EXT_MSGMANAGE INT8U MsgRead(MsgType *msg);
EXT_MSGMANAGE INT8U MsgOtherRead(MsgType *msg);
EXT_MSGMANAGE INT8U MsgInBuf(MsgBufType *msgbuf, MsgType *msg);
EXT_MSGMANAGE INT8U MsgOutBuf(MsgBufType *msgbuf , MsgType *msg);
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
