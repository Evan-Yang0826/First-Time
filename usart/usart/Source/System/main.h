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
  **  2011/09/19   Devil   ����
************************************************************************************************/

#ifndef  __YH_MAIN_H__ 
#define  __YH_MAIN_H__ 

/* ����Ԥ���� */
#ifdef YH_MAIN_LOCAL   
  	#define  EXT_MAIN    
#else  
	#define  EXT_MAIN  extern
#endif   

#define SYS_SIG1MS_MAX        (10)

#define M485REVTIME	  		(0)
#define BUTTONMSTIME	  	(1)
#define BEEPMSTIME	  		(2)
#define AUTOADDRTASK		(3)
#define LOGICCONTROL_MS		(4)
#define MSGMANAGE_MS	 	(5)
#define TNETMSGMANAGE1_MS    (6)
	
	
#define Esp_Ap_Sta  (0)  //1Ϊap 0Ϊsta

EXT_MAIN INT32U Sys_Sig1ms[SYS_SIG1MS_MAX];
	
/* ������ַ */	
EXT_MAIN INT8U MyAddr;

/* ȫ�ֿ�����ʱ�洢������ֹ���ж���ʹ�� */
EXT_MAIN INT8U DataTemp[100];

#define GET_MS_SIG(num)      (Sys_Sig1ms[num])
#define CLR_MS_SIG(num)      (Sys_Sig1ms[num]=0)
	
EXT_MAIN INT32U Get_Systick(void);
extern	unsigned char SN[4];

#endif  /* __YH_MAIN_H__ */



/*************************************************************************************************
**					����������        END FILE
**************************************************************************************************/
