/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    STM32_I2C1.h
  **  描    述:    STM32硬件I2C1驱动
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/12/18   Joshua    创建
************************************************************************************************/
#ifndef __STM32_I2C1_H__
#define __STM32_I2C1_H__

/* 本地预定义 */
#ifdef STM32_I2C1_LOCAL
    #define EXT_STM32_I2C1
#else
    #define EXT_STM32_I2C1    extern
#endif

EXT_STM32_I2C1 void STM32_I2C1_Init(void);

EXT_STM32_I2C1 void Stm32iic1_Write(INT8U Addr,INT8U Reg,INT8U data);
EXT_STM32_I2C1 INT8U Stm32iic1_SingleRead(INT8U Addr);
EXT_STM32_I2C1 void Stm32iic1_MultiWrite(INT8U Addr,INT8U Reg,INT8U *data,INT8U dataLong);
EXT_STM32_I2C1 void Stm32iic1_SingleWrite(INT8U Addr,INT8U data);
	
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
