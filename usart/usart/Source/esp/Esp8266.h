/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    Esp8266.h
  **  描    述:    
  **  创 建 人:    jim
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2023/3/16   jim    创建
************************************************************************************************/
#ifndef __ESP8266_H__
#define __ESP8266_H__

/* 本地预定义 */
#ifdef ESP8266_LOCAL
    #define EXT_ESP8266
#else
    #define EXT_ESP8266    extern
#endif
typedef struct
{
	INT8U Port;
	INT8U Mac_Address[6];
	INT8U IP_Get[4];
	INT8U RFID_ID[4];

}esp_ap_type;


EXT_ESP8266 void esp8266_init(void);

EXT_ESP8266 INT8U Get_fig_Success;
EXT_ESP8266 int Get_Length;
EXT_ESP8266		void Control_Delay_Task(void);
EXT_ESP8266 esp_ap_type esp_ap_info[5];
EXT_ESP8266 void Get_Info_To_Ap(void);
EXT_ESP8266 INT8U Mac_Address_ben[6];
EXT_ESP8266 void Get_Frist_To_Ap(void);
EXT_ESP8266 INT8U chack_Mac_Voild(INT8U *Mac_data);
EXT_ESP8266 void Send_data_task(INT8U *RFID);
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
