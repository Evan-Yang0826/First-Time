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
  **  2011/09/19   Devil   建立
************************************************************************************************/
#ifndef  __STM32_UART1_H__ 
#define  __STM32_UART1_H__ 

#define GPIO_AFIO_UART1_ENABLE			1

#ifdef STM32_UART1LOCAL
    #define EXT_STM32_UART1
#else
    #define EXT_STM32_UART1    extern
#endif


#define SYN6288_UART_ENABLE()		 STM32_U1_Switch(1)
#define SYN6288_UART_DISABLE()		 STM32_U1_Switch(0)

EXT_STM32_UART1 INT8U  STM32_U1_OPEN(INT32U Baud, INT16U StopBit, INT16U Parity);
EXT_STM32_UART1 INT32U STM32_U1_Write(INT8U *buf, INT32U MaxLong);
EXT_STM32_UART1 INT32U STM32_U1_Read(INT8U *buf, INT32U MaxLong);
EXT_STM32_UART1 INT32U STM32_U1_Switch(INT8U Fig);

#endif  /* __STM32_UART1_H__  */
