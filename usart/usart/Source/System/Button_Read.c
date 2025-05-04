/****************************************Copyright (c)******************************************
**
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    Button_Read.c
  **  描    述:    SX8636按键读取
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:
  **  2018/10/17   Devil    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"

/* 本地头文件 */
#define BUTTON_READ_LOCAL
#include "allinclude.h"


INT8U ButtonIRQFig = 0;



const INT8U KeyList[6] = 
{
	KEYL1,
	KEYL2,
	KEYL3,
	KEYR3
};

KeyIOType KeySetio_List[1] = 
{
	{GPIOC,GPIO_Pin_14},
	
};

KeyTimeType KeyTime[6];
/*************************************************************************************************
** 函数名称:    Button_Read_Init
** 功能描述:
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Button_Read_Init(void)
{
	INT32U i;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 输出控制口

 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	 //使能PG端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = KeySetio_List[0].Pin_set; 				 //PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);				 //初始化IO口

    // LED 初始化
 
    KeyValue.LEDStatus = 0;
	
	
    GPIO_InitStructure.GPIO_Pin = LED_Gpio_Pin;				 //PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //初始化IO口

		

	for(i=0;i<4;i++)
	{
		KeyTime[i].KEY = KeyList[i];
	}
}




/*************************************************************************************************
** 函数名称:    KeyRecognition
** 功能描述:
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/11/29   Devil    创建
*************************************************************************************************/
void KeyRecognition(INT8U keyv)
{
	INT32U i;
	
	KeyTimeType maxkey = {0,0};
	KeyTimeType seckey = {0,0};
	
	if(keyv)
	{
		for(i=0;i<6;i++)
		{
			if(keyv & KeyTime[i].KEY)
			{
				KeyTime[i].Time += 1;
			}
			else
			{
				KeyTime[i].Time = 0;
			}
			
			if(maxkey.Time < KeyTime[i].Time)
			{
				maxkey.Time = KeyTime[i].Time;
				maxkey.KEY = KeyTime[i].KEY;
				
			}
		}
		
		for(i=0;i<6;i++)
		{		
			if(KeyTime[i].Time <= maxkey.Time)
			if(seckey.Time < KeyTime[i].Time)
			if(maxkey.KEY != KeyTime[i].KEY)
			{
				seckey.Time = KeyTime[i].Time;
				seckey.KEY = KeyTime[i].KEY;
			}
			
		}
		
		if(seckey.Time > 200)
		{
			KeyValue.KeyDownFig = TRUE;
			KeyValue.KeyFig =  TRUE;
			KeyValue.KeyValue = maxkey.KEY | seckey.KEY;
			KeyValue.KeyTime = seckey.Time;
		}
		else if(maxkey.KEY > 2)
		{
			KeyValue.KeyDownFig = TRUE;
			KeyValue.KeyFig =  TRUE;
			KeyValue.KeyValue = maxkey.KEY;
			KeyValue.KeyTime = maxkey.Time;
		}
		else
		{
			
		}
	}
}
/*************************************************************************************************
** 函数名称:    KeyTimeClear
** 功能描述:
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/11/29   Devil    创建
*************************************************************************************************/
void KeyTimeClear(void)
{
	INT8U i;
	
	for(i=0;i<6;i++)
	{
		KeyTime[i].Time = 0;
	}
}
/*************************************************************************************************
** 函数名称:    Button_Read_Task
** 功能描述:		按键扫描函数
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Button_Read_Task(void)
{
    static INT8U keyirqvalue = 0;
    static INT8U BgOpenFig = 0;
    INT8U keyv;
	INT8U i;
    static INT8U KeyCountTime = 0;
	static INT8U KeyTimems = 0;

    // 执行频率 1ms 一次
    if(GET_MS_SIG(BUTTONMSTIME))
    {
        CLR_MS_SIG(BUTTONMSTIME);
		keyv = 0;
				for(i=0;i<1;i++)
			 {
					if((KeySetio_List[i].IO_pin->IDR & KeySetio_List[i].Pin_set) == 0)
					{
						keyv |= KeyList[i];
					}
			 }
				
					if(keyv)
					{
					// 阻止熄灯和复位


					//BeepAndMotor_Open(0,60);

					
						KeyRecognition(keyv);
					}
					else
					{
						if(KeyValue.KeyValue != 0)
						{
							KeyValue.KeyDownFig = 0;
							KeyTimeClear();
							
						}
						                
					}			
					
        }
}
/************************************************************************************************
                                         END FILE
************************************************************************************************/
