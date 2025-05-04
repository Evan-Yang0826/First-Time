/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    Esp8266.c
  **  描    述:    
  **  创 建 人:    jim
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2023/3/16   jim    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"



typedef void (*callback_task)(INT8U *data); 


typedef struct
{
	INT8U Char_cmd[30];
	callback_task  callback_Control;

}callback_type;



/* 本地头文件 */
#define ESP8266_LOCAL
#include "allinclude.h"
#if Esp_Ap_Sta == 1


char AT_RST[] = {"AT+RST\r\n"};
char AT_Mode[] = {"AT+CWMODE=2\r\n"};
char AT_SSID[] = {"AT+CWSAP=\"esr\",\"1234567890\",1,0\r\n"};
char AT_TCP[] = {"AT+CIPMUX=1\r\n"};
char AT_CIPCOM[] = {"AT+CIPSERVER=1,8088\r\n"};

char AT_Send_Data[] = {"AT+CIPSEND=0,10\r\n"};

char AT_GetStatus_Data[] = {"AT+CIPSTATUS\r\n\r\n"};


char AT_GetSR_Data[] = {"AT+CIFSR\r\n"};
void StringToHex(char *str, unsigned char *strhex)
{
	uint8_t i,cnt=0;
	char *p = str;             //直针p初始化为指向str
	uint8_t len = strlen(str); //获取字符串中的字符个数
	
	
		for (i = 0; i < len; i ++)  //循环判断当前字符是数字还是小写字符还是大写字母
		{
			if ((*p >= '0') && (*p <= '9')) //当前字符为数字0~9时
				strhex[cnt] = *p - '0';//转为十六进制
			
			if ((*p >= 'A') && (*p <= 'Z')) //当前字符为大写字母A~Z时
				strhex[cnt] = *p - 'A';//转为十六进制
			
			if ((*p >= 'a') && (*p <= 'z')) //当前字符为小写字母a~z时
				strhex[cnt] = *p - 'a';  //转为十六进制
		
			p ++;    //指向下一个字符
			cnt ++;  
		}
	
}

INT8U Mac_Init[6];
INT8U chack_Mac_Voild(INT8U *Mac_data)
{
	int i,j;
	u8 Get_fig = 0;
	u8 length_fig = 0;
	for(i=0;i<sizeof(esp_ap_info)/sizeof(esp_ap_type);i++)
	{
		for(j=0;j<6;j++)
		{
			if(Mac_data[j] == esp_ap_info[i].Mac_Address[j])
			{
				Get_fig = i;
			}
			else
			{
				
				break;
			}
		}
		if(j == 6)
		{
			break;
		}
	}
	if(i == sizeof(esp_ap_info)/sizeof(esp_ap_type))
	{
		return 0xff;
	}
	else
	{
		return i;
	}
}

INT8U chack_ip_Voild(INT8U *Mac_data)
{
	int i,j;
	u8 Get_fig = 0;
	for(i=0;i<sizeof(esp_ap_info)/sizeof(esp_ap_type);i++)
	{
		for(j=0;j<4;j++)
		{
			if(Mac_data[j] == esp_ap_info[i].IP_Get[j])
			{
				Get_fig = i;
			}
			else
			{
				break;
			}
		}
		if(j == 4)
		{
			break;
		}
	}
	if(i == sizeof(esp_ap_info)/sizeof(esp_ap_type))
	{
		return 0xff;
	}
	else
	{
		return i;
	}
}

INT8U Get_Kong_For_Table(void)
{
	INT8U i,j;
	INT8U Get_fig;
	
	for(i=0;i<sizeof(esp_ap_info)/sizeof(esp_ap_type);i++)
	{
		for(j=0;j<6;j++)
		{
			if(esp_ap_info[i].Mac_Address[j] != 0)
			{
				Get_fig = 0xff;
				break;
			}
			else
			{
				Get_fig = i;		
			}		
		}
		if(Get_fig != 0xff)
		{
			return Get_fig;
		}
	}
	return Get_fig;
}

void STA_CONNECTED_Task(INT8U * data)
{
	INT8U temp = 0;
	INT8U lengoo = 0;
	//printf("STA_CONNECTED_Task\r\n");
	if(data[0] == ':')
	{
		sscanf(&data[2], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",&Mac_Init[0],&Mac_Init[1],&Mac_Init[2],&Mac_Init[3],&Mac_Init[4],&Mac_Init[5]);
		lengoo = chack_Mac_Voild(Mac_Init) ;
		if(lengoo== 0xff)
		{
			temp = Get_Kong_For_Table();
			if(temp != 0xff)
			{
				YHMemoryCopy(Mac_Init,esp_ap_info[temp].Mac_Address,6);
			}
		}
		else
		{
			YHMemoryCopy(Mac_Init,esp_ap_info[lengoo].Mac_Address,6);
		}
	}
	
}
void STA_DISCONNECTED_Task(INT8U * data)
{
	INT8U temp = 0xff;
	//	printf("STA_DISCONNECTED_Task\r\n");
	if(data[0] == ':')
	{
		sscanf(&data[2], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",&Mac_Init[0],&Mac_Init[1],&Mac_Init[2],&Mac_Init[3],&Mac_Init[4],&Mac_Init[5]);
		temp = chack_Mac_Voild(Mac_Init);
		if(temp != FALSE && temp != 0xff)
		{
			YHMemoryClear((INT8U *)&esp_ap_info[temp],sizeof(esp_ap_type));
			
		}
	}
	
}
INT8U According_ToPort(INT8U port)
{
	INT8U i;
	for(i=0;i<sizeof(esp_ap_info)/sizeof(esp_ap_type);i++)
	{
		if(port == esp_ap_info[i].Port)
		{
			return i;
		}
	}
	return 0xff;
}
void IPD_Get_Task(INT8U * data)
{
	INT8U temp = 0xff;
		//printf("IPD_Get_Task\r\n");
	if(data[0] == ',')
	{
		if(data[5] == 'i' && data[6] == 'n' && data[7]=='i'&& data[8] == 't')
		{
			
		
		}
		else if(data[6] == 'R' && data[7] == 'F' && data[8]=='I'&& data[9] == 'D')
		{
			temp = According_ToPort(data[1] - '0');
			if(temp != 0xff)
			{
				
					sscanf(&data[10],"%02x%02x%02x%02x",&esp_ap_info[temp].RFID_ID[0],&esp_ap_info[temp].RFID_ID[1],&esp_ap_info[temp].RFID_ID[2],&esp_ap_info[temp].RFID_ID[3]);
				  MsgType Msg_temp;
					Msg_temp.Long = 11;
					Msg_temp.data[0] = 0x03;
					YHMemoryCopy(esp_ap_info[temp].Mac_Address,&Msg_temp.data[1],6);
					YHMemoryCopy(esp_ap_info[temp].RFID_ID,&Msg_temp.data[7],4);
					if(LogicRun.state == LogicRunNormal)
					{
						MsgSend(&Msg_temp,0);
					}
			}
			
		}
	
	}
}

void CIPSTATUS_Get_Task(INT8U * data)
{
	INT8U temp;
	//INT8U Mac_Init_temp[6];
	INT8U Ip_Set[5];
	printf("CIPSTATUS_Get_Task\r\n");
	
		if(data[0] != 0)
		{
			sscanf((char *)&data[9], "%d.%d.%d.%d",(char *)&Ip_Set[0],(char *)&Ip_Set[1],(char *)&Ip_Set[2],(char *)&Ip_Set[3]);
			//sscanf(&data[9], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",&Mac_Init_temp[0],&Mac_Init_temp[1],&Mac_Init_temp[2],&Mac_Init_temp[3],&Mac_Init_temp[4],&Mac_Init_temp[5]);
			if(Ip_Set[0] > 0)
			{
				temp = chack_ip_Voild(Ip_Set);
				
				if(temp != 0xff)
				{
					esp_ap_info[temp].Port = data[0] - '0';
				}
			}
		}
	
	
}

void DIST_STA_IP_Get_Task(INT8U * data)
{
	INT8U temp = 0xff;
	INT8U Mac_Init_temp[6];
	//printf("DIST_STA_IP_Get_Task\r\n");
	INT8U Ip_Set[4];
		if(data[0] != 0)
		{
			sscanf(&data[2], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",&Mac_Init_temp[0],&Mac_Init_temp[1],&Mac_Init_temp[2],&Mac_Init_temp[3],&Mac_Init_temp[4],&Mac_Init_temp[5]);
			temp = chack_Mac_Voild(Mac_Init_temp);
			if(temp != 0xff)
			{
				sscanf(&data[22], "%d.%d.%d.%d",&Ip_Set[0],&Ip_Set[1],&Ip_Set[2],&Ip_Set[3]);
				YHMemoryCopy(Ip_Set,esp_ap_info[temp].IP_Get,4);
			}
			
		}
	
	
}

void DIST_APMAC_Task(INT8U * data)
{
	INT8U temp = 0xff;
	INT8U Mac_Init_temp[6];
	//printf("DIST_STA_IP_Get_Task\r\n");
	
		if(data[0] != 0)
		{
			sscanf(&data[1], "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",&Mac_Init_temp[0],&Mac_Init_temp[1],&Mac_Init_temp[2],&Mac_Init_temp[3],&Mac_Init_temp[4],&Mac_Init_temp[5]);
			YHMemoryCopy(Mac_Init_temp,Mac_Address_ben,6);
			
		}
	
	
}

void CONNECT_Task(INT8U * data)
{
	if(data[0] != 0)
		{
			if ((data[0] >= '0') && (data[0] <= '9')) //当前字符为数字0~9时
			{
				delay_ms(100);
				STM32_U2_Write(AT_GetStatus_Data,sizeof(AT_GetStatus_Data));
			}
		}
}


callback_type cmd_table[] = 
{
	
	{"+STA_DISCONNECTED",STA_DISCONNECTED_Task},
	{"+STA_CONNECTED",STA_CONNECTED_Task},
	{"+IPD",IPD_Get_Task},
	{"+CIPSTATUS:",CIPSTATUS_Get_Task},
	{"+DIST_STA_IP",DIST_STA_IP_Get_Task},
	{"+CIFSR:APMAC,",DIST_APMAC_Task},
	{"CONNECT",CONNECT_Task},
};



void esp8266_init(void)
{
	
		STM32_U2_OPEN(115200,USART_StopBits_1,USART_Parity_No);
		STM32_U2_Write(AT_RST,sizeof(AT_RST));
		delay_ms(100);
		STM32_U2_Write(AT_Mode,sizeof(AT_Mode));
		delay_ms(100);
		STM32_U2_Write(AT_SSID,sizeof(AT_SSID));
		delay_ms(100);
	  STM32_U2_Write(AT_TCP,sizeof(AT_TCP));
		delay_ms(100);
		STM32_U2_Write(AT_CIPCOM,sizeof(AT_CIPCOM));
	delay_ms(100);
}



void Control_Delay_Task(void)
{
	 u8 data[1000];
	 u8 data_temp[1000];
	char * length_temp = NULL;
	char * temp_data = NULL;
	u16 i,j,k;
	u16 length = 0;
	u16 set_fig = 0;
	//static INT8U First_fig = 0;
	memset(data,0,sizeof(data));

	//printf("temp_length :%d\r\n",temp_length);
	if(Get_Length > 0)
	{
		if(STM32_U2_Read(data,Get_Length)>0)
		{
			Get_Length = 0;
		//	printf(" =========================== \r\n");
			
				printf("%s",data);
			
			temp_data = data;
			for(i=0;i<sizeof(cmd_table)/sizeof(callback_type);i++)
			{
				length_temp = strstr((char *)(temp_data),(const char *)cmd_table[i].Char_cmd);
				
				
				{
					if(length_temp != NULL)
					{
						if(cmd_table[i].callback_Control != NULL)
						{
							
							length = strlen((const char*)cmd_table[i].Char_cmd);
							if(i != 6)
							{
								cmd_table[i].callback_Control((INT8U *)(length_temp+length));
							}
							else
							{
								cmd_table[i].callback_Control((INT8U *)(length_temp-2));
							}
							if(i== 3 || i == 4)
							{
								temp_data = length_temp + 45;
								i-=1;
								
							}
							else if(i == 1 || i == 0)
							{
								temp_data = length_temp + 32;
								i-=1;
							}
							else if(i==2)
							{
									temp_data = length_temp + 20;
								i-=1;
							}
							length_temp = NULL;
						}
					}
				}
				
			}
			
			printf("   ting\r\n");
			//printf("rev:[");
			
			
			//printf("]\r\n");
	  }
	}
		
}
void Get_Info_To_Ap(void)
{
if(GET_MS_SIG(TNETMSGMANAGE1_MS) > 10000)
	{
			CLR_MS_SIG(TNETMSGMANAGE1_MS);
			STM32_U2_Write(AT_GetStatus_Data,sizeof(AT_GetStatus_Data));
	
	}
}
void Get_Frist_To_Ap(void)
{

			STM32_U2_Write(AT_GetSR_Data,sizeof(AT_GetSR_Data));
}

#else
char AT_RST[] = {"AT+RST\r\n"};
char AT_Mode[] = {"AT+CWMODE=1\r\n"};
char AT_SSID[] = {"AT+CWJAP_DEF=\"esr\",\"1234567890\"\r\n"};
char AT_CIPCOM[] = {"AT+CIPSTART=\"TCP\",\"192.168.4.1\",8088\r\n"};

char AT_Send_Data[] = {"AT+CIPSEND=12\r\n"};
void STA_CLOSED_Task(INT8U *data)
{
	delay_ms(100);
	STM32_U2_Write(AT_CIPCOM,sizeof(AT_CIPCOM));
}

callback_type cmd_table[] = 
{
	
	{"CLOSED",STA_CLOSED_Task},
	{"WIFI GOT IP",STA_CLOSED_Task},
};
void esp8266_init(void)
{
		delay_ms(rand()%500+100);
		STM32_U2_OPEN(115200,USART_StopBits_1,USART_Parity_No);
		STM32_U2_Write(AT_RST,sizeof(AT_RST));
		delay_ms(100);
		STM32_U2_Write(AT_Mode,sizeof(AT_Mode));
		delay_ms(100);
		STM32_U2_Write(AT_SSID,sizeof(AT_SSID));
		
		delay_ms(100);
		STM32_U2_Write(AT_CIPCOM,sizeof(AT_CIPCOM));
		delay_ms(100);
}

void Control_Delay_Task(void)
{
	 u8 data[1000];
	 u8 data_temp[1000];
	char * length_temp = NULL;
	char * temp_data = NULL;
	u16 i,j,k;
	u16 length = 0;
	u16 set_fig = 0;
	//static INT8U First_fig = 0;
	memset(data,0,sizeof(data));

	//printf("temp_length :%d\r\n",temp_length);
	if(Get_Length > 0)
	{
		if(STM32_U2_Read(data,Get_Length)>0)
		{
			Get_Length = 0;
		//	printf(" =========================== \r\n");
			
				printf("%s",data);
			
			temp_data = data;
			for(i=0;i<sizeof(cmd_table)/sizeof(callback_type);i++)
			{
				length_temp = strstr((char *)(temp_data),(const char *)cmd_table[i].Char_cmd);
				
				
				{
					if(length_temp != NULL)
					{
						if(cmd_table[i].callback_Control != NULL)
						{
							
							length = strlen((const char*)cmd_table[i].Char_cmd);
							cmd_table[i].callback_Control((INT8U *)(length_temp+length));
							
							if(i== 3)
							{
								temp_data = length_temp + 45;
								i-=1;
								
							}
							length_temp = NULL;
						}
					}
				}
				
			}
			
			printf("   ting\r\n");
			//printf("rev:[");
			
			
			//printf("]\r\n");
	  }
	}
		
}
void Send_data_task(INT8U *RFID)
{
	char send_data[14];
	INT16U time = rand() % 200 + 100;
	
//	sprintf(send_data,"AT+CIPSEND=0,10\r\n");
	STM32_U2_Write(AT_Send_Data,sizeof(AT_Send_Data));
	delay_ms(time);
	sprintf(send_data,"RFID%02x%02x%02x%02x",RFID[0],RFID[1],RFID[2],RFID[3]);
	STM32_U2_Write(send_data,sizeof(send_data));
}
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
