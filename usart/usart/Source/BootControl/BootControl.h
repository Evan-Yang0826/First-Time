/****************************************Copyright (c)******************************************
**
**          ���ܿƼ�   Copyright(C) 2012, Soone ,All rights reserved.
**
**-------------------------------------�ļ���Ϣ--------------------------------------------
  **  �� �� ��: 
  **  ��    ��: 
  **  �� �� ��:
  **  ��Χ����:    ȫ����ͷ�ļ�    
**--------------------------------------------------------------------------------------------
  **  �޸���ʷ:
  **  2012/03/29   Devil   ����
************************************************************************************************/

#ifndef  __BOOTCONTROL_H__ 
#define  __BOOTCONTROL_H__ 

/* ����Ԥ���� */
#ifdef BOOTCONTROL_LOCAL   
  	#define  EXT_BOOTCONTROL    
#else  
	#define  EXT_BOOTCONTROL  extern
#endif  
 

/* ����Ϊ 1 ����ID��֤���� */
#define STM32_ID_CHECK				 (0)
	
typedef struct 
{
	unsigned char Fig;				/* 0x12 ��Ҫ����������֤ */
	unsigned short CrcValue;		/* STM32 ΨһID��CRCУ�� */
	unsigned char Xor[4];			/* STM32 ΨһID�����У��ֵ */
}BOOTFIG_TYPE;


/* ID���ܺ����ݴ洢��ַ */ 
#define BOOTFIG_ADDR     (0x8002C00)

/* STM32 ΨһID�����ַ */ 
#define MCU_ID_ADDR		((unsigned char*)0x1FFFF7E8)

/* �ڳ�ʼ��ʱ���ã��Զ�дУ��ID */
EXT_BOOTCONTROL void BootControl_Init(void);

/* �ڳ������й����е��ã����ID�Ƿ�Ϸ� */
EXT_BOOTCONTROL void BLC_CheckID(void);
EXT_BOOTCONTROL unsigned char BLC_FalshWrite(unsigned int addr,unsigned char *pData,unsigned int Wlong);
EXT_BOOTCONTROL unsigned short SN_Calc_CRC16(unsigned char *Data,unsigned int Mlong);
EXT_BOOTCONTROL void STM32_GetMAC(unsigned char *mac);


#endif  /* __BOOTCONTROL_H__ */


/*************************************************************************************************
**					����������        END FILE
**************************************************************************************************/
