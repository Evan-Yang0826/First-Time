/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    STM32_I2CM.h
  **  描    述:    STM32 IO模拟I2C
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/10/12   Devil    创建
************************************************************************************************/
#ifndef __STM32_I2CM_H__
#define __STM32_I2CM_H__

/* 本地预定义 */
#ifdef STM32_I2CM_LOCAL
    #define EXT_STM32_I2CM
#else
    #define EXT_STM32_I2CM    extern
#endif


#define TWI_NOP 						I2CDelay(22)
#define TWI_SCL_0 						GPIOB->BRR=GPIO_Pin_6
#define TWI_SCL_1 						GPIOB->BSRR=GPIO_Pin_6
#define TWI_SDA_0 						GPIOB->BRR=GPIO_Pin_7
#define TWI_SDA_1 						GPIOB->BSRR=GPIO_Pin_7
#define TWI_SDA_STATE 					(GPIOB->IDR&GPIO_Pin_7)		
	
#define TWI_SDA_IN		GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x40000000
#define TWI_SDA_OUT		GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x70000000

	
	
/* 函数声明 */
EXT_STM32_I2CM void I2CM_STOP(void);
EXT_STM32_I2CM INT8U I2CM_START(void);
	
EXT_STM32_I2CM void I2C1_Init(void); 
EXT_STM32_I2CM INT32U I2C1_ReadData(INT8U SLA,INT16U SubAddr,INT8U *DataBuf,INT32U Long);
	
EXT_STM32_I2CM INT8U I2CM_SendByte(INT8U Data);
EXT_STM32_I2CM INT8U I2CM_ReceiveByte(void);


#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
