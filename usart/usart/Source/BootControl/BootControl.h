/****************************************Copyright (c)******************************************
**
**          所能科技   Copyright(C) 2012, Soone ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名: 
  **  描    述: 
  **  创 建 人:
  **  范围性质:    全局性头文件    
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2012/03/29   Devil   建立
************************************************************************************************/

#ifndef  __BOOTCONTROL_H__ 
#define  __BOOTCONTROL_H__ 

/* 本地预定义 */
#ifdef BOOTCONTROL_LOCAL   
  	#define  EXT_BOOTCONTROL    
#else  
	#define  EXT_BOOTCONTROL  extern
#endif  
 

/* 设置为 1 开启ID验证功能 */
#define STM32_ID_CHECK				 (0)
	
typedef struct 
{
	unsigned char Fig;				/* 0x12 需要进行启动验证 */
	unsigned short CrcValue;		/* STM32 唯一ID码CRC校验 */
	unsigned char Xor[4];			/* STM32 唯一ID码异或校验值 */
}BOOTFIG_TYPE;


/* ID加密后数据存储地址 */ 
#define BOOTFIG_ADDR     (0x8002C00)

/* STM32 唯一ID码基地址 */ 
#define MCU_ID_ADDR		((unsigned char*)0x1FFFF7E8)

/* 在初始化时调用，自动写校验ID */
EXT_BOOTCONTROL void BootControl_Init(void);

/* 在程序运行过程中调用，检查ID是否合法 */
EXT_BOOTCONTROL void BLC_CheckID(void);
EXT_BOOTCONTROL unsigned char BLC_FalshWrite(unsigned int addr,unsigned char *pData,unsigned int Wlong);
EXT_BOOTCONTROL unsigned short SN_Calc_CRC16(unsigned char *Data,unsigned int Mlong);
EXT_BOOTCONTROL void STM32_GetMAC(unsigned char *mac);


#endif  /* __BOOTCONTROL_H__ */


/*************************************************************************************************
**					　　　　　        END FILE
**************************************************************************************************/
