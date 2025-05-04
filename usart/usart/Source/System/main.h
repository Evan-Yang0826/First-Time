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

#ifndef  __YH_MAIN_H__ 
#define  __YH_MAIN_H__ 

/* 本地预定义 */
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
	
	
#define Esp_Ap_Sta  (0)  //1为ap 0为sta

EXT_MAIN INT32U Sys_Sig1ms[SYS_SIG1MS_MAX];
	
/* 本机地址 */	
EXT_MAIN INT8U MyAddr;

/* 全局可用临时存储器，禁止在中断中使用 */
EXT_MAIN INT8U DataTemp[100];

#define GET_MS_SIG(num)      (Sys_Sig1ms[num])
#define CLR_MS_SIG(num)      (Sys_Sig1ms[num]=0)
	
EXT_MAIN INT32U Get_Systick(void);
extern	unsigned char SN[4];

#endif  /* __YH_MAIN_H__ */



/*************************************************************************************************
**					　　　　　        END FILE
**************************************************************************************************/
