/****************************************Copyright (c)******************************************
**
**          懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名: 
  **  描    述: 
  **  创 建 人:
  **  范围性质:    全局性头文件    
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2011/09/19   易仕尧   建立
************************************************************************************************/
#ifndef  __STM32_UART3_H__ 
#define  __STM32_UART3_H__ 

#ifdef STM32_UART3LOCAL
    #define EXT_STM32_UART3
#else
    #define EXT_STM32_UART3    extern
#endif

 
EXT_STM32_UART3 INT8U  Usart3RevBusyFig;
	
EXT_STM32_UART3 INT8U  STM32_U3_OPEN(INT32U Baud, INT16U StopBit, INT16U Parity);
EXT_STM32_UART3 INT32U STM32_U3_Write(INT8U *buf, INT32U MaxLong);
EXT_STM32_UART3 INT32U STM32_U3_Read(INT8U *buf, INT32U MaxLong);


#endif  /* __STM32_UART2_H__  */
