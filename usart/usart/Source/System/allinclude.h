/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    include.h
  **  描    述:    总头文件
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/10/25   Joshua    创建
************************************************************************************************/
#ifndef __INCLUDE_H__
#define __INCLUDE_H__

/* 本地预定义 */
#ifdef INCLUDE_LOCAL
    #define EXT_INCLUDE
#else
    #define EXT_INCLUDE    extern
#endif

#include "stdlib.h"
#include "string.h"
#include "YH_TYPE.h"
#include "../System/main.h"

#include "../STM32_UART/STM32_UART3.h"
#include "../STM32_delay/STM32_delay.h"
#include "YH_Clibrary.h"
#include "MsgManage.h"
#include "LogicRun.h"
		#include "Button_Read.h"
#include "../STM32_UART/STM32_UART2.h"
#include "rc522.h"
#include "Esp8266.h"
#include "../STM32_UART/STM32_UART1.h"
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
