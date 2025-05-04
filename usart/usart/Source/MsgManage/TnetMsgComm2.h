/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    TnetMsgComm2.h
  **  描    述:    
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2019/8/5   Devil    创建
************************************************************************************************/
#ifndef __TNETMSGCOMM2_H__
#define __TNETMSGCOMM2_H__

/* 本地预定义 */
#ifdef TNETMSGCOMM2_LOCAL
    #define EXT_TNETMSGCOMM2
#else
    #define EXT_TNETMSGCOMM2    extern
#endif




EXT_TNETMSGCOMM2 void TnetMsgComm2_Init(void);
EXT_TNETMSGCOMM2 void TnetMsgComm2_Task(void);
EXT_TNETMSGCOMM2 void MsgSendtoBusCom1(MsgType *msg);

#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
