/****************************************Copyright (c)******************************************
**
**          ܲ��������   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------�ļ���Ϣ--------------------------------------------
  **  �� �� ��: 
  **  ��    ��: 
  **  �� �� ��:
  **  ��Χ����:    ȫ����ͷ�ļ�    
**--------------------------------------------------------------------------------------------
  **  �޸���ʷ:
  **  2011/09/19   ����Ң   ����
************************************************************************************************/
#ifndef  __STM32_UART2_H__ 
#define  __STM32_UART2_H__ 

#ifdef STM32_UART2LOCAL
    #define EXT_STM32_UART2
#else
    #define EXT_STM32_UART2    extern
#endif

 
EXT_STM32_UART2 INT8U  STM32_U2_OPEN(INT32U Baud, INT16U StopBit, INT16U Parity);
EXT_STM32_UART2 INT32U STM32_U2_Write(INT8U *buf, INT32U MaxLong);
EXT_STM32_UART2 INT32U STM32_U2_Read(INT8U *buf, INT32U MaxLong);


#endif  /* __STM32_UART2_H__  */
