/****************************************Copyright (c)******************************************
*
*    http://www.aoretech.cn/main.asp  Copyright(C) 2008, YMHX ,All rights reserved.
*
*-------------------------------------文件信息--------------------------------------------
 *   文 件 名:  I2C1.h
 *	 功能描述:	I2C1驱动头文件
 *   创 建 人:
 *
 *   修改历史:
 *   2011/6/15 创建

 *	 范围性质:半全局头性文件 
************************************************************************************************/
#ifndef  __I2C1_H__ 
#define  __I2C1_H__ 


#define  USE_HARDI2C	0





  /* 本地预定义 */
  #ifdef I2C1_LOCAL   
  	
    #define  EXT_I2C1_LOCAL    

  /* 外部预定义 */
  #else  

    #define  EXT_I2C1_LOCAL  extern

  /* 结束预定义 */
  #endif  
 
#if   			USE_HARDI2C   
/* I2C flags definition */
#define I2C_FLAG_DUALF                  ((u32)0x00800000)
#define I2C_FLAG_SMBHOST                ((u32)0x00400000)
#define I2C_FLAG_SMBDEFAULT             ((u32)0x00200000)
#define I2C_FLAG_GENCALL                ((u32)0x00100000)
#define I2C_FLAG_TRA                    ((u32)0x00040000)
#define I2C_FLAG_BUSY                   ((u32)0x00020000)
#define I2C_FLAG_MSL                    ((u32)0x00010000)
#define I2C_FLAG_SMBALERT               ((u32)0x10008000)
#define I2C_FLAG_TIMEOUT                ((u32)0x10004000)
#define I2C_FLAG_PECERR                 ((u32)0x10001000)
#define I2C_FLAG_OVR                    ((u32)0x10000800)
#define I2C_FLAG_AF                     ((u32)0x10000400)
#define I2C_FLAG_ARLO                   ((u32)0x10000200)
#define I2C_FLAG_BERR                   ((u32)0x10000100)
#define I2C_FLAG_TXE                    ((u32)0x00000080)
#define I2C_FLAG_RXNE                   ((u32)0x00000040)
#define I2C_FLAG_STOPF                  ((u32)0x60000010)
#define I2C_FLAG_ADD10                  ((u32)0x20000008)
#define I2C_FLAG_BTF                    ((u32)0x60000004)
#define I2C_FLAG_ADDR                   ((u32)0xA0000002)
#define I2C_FLAG_SB                     ((u32)0x20000001)
#else
#define TWI_NOP 						I2CDelay(30)
#define TWI_SCL_0 						GPIOB->BRR=GPIO_Pin_6
#define TWI_SCL_1 						GPIOB->BSRR=GPIO_Pin_6
#define TWI_SDA_0 						GPIOB->BRR=GPIO_Pin_7
#define TWI_SDA_1 						GPIOB->BSRR=GPIO_Pin_7
#define TWI_SDA_STATE 					(GPIOB->IDR&GPIO_Pin_7)

#endif

	/* 以下是变量和函数声明 */

EXT_I2C1_LOCAL void I2C1_Iint(void); 
EXT_I2C1_LOCAL INT32U I2C1_ReadData(INT8U SLA,INT16U SubAddr,INT8U *DataBuf,INT32U Long);

EXT_I2C1_LOCAL void WriteByte(u16 addr, u8 data);
EXT_I2C1_LOCAL void Writestring(INT16U addr, INT8S *data,INT16U Long);

#endif  /* __I2C1_H__ */
