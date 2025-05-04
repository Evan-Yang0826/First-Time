/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    LogicRun.c
  **  描    述:    
  **  创 建 人:    Jim
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2023/3/12   Jim    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"


/* 本地头文件 */
#define LOGICRUN_LOCAL
#include "allinclude.h"


void LogicRun_Set(LogicRunEnum state)
{
	LogicRun.state = state;
	LogicRun.Runstate = 0;
}
void LogicRunNormal_Task(void)
{
	
	if(LogicRun.Runstate == 0)
	{
		LogicRun.Runstate = 1;
		LED_set = 0;
	}
	else if(KeyValue.KeyFig)
	{
		if(KeyValue.KeyValue == KEYL1)
		{
			if(KeyValue.KeyDownFig == 1)
			{
				if(KeyValue.KeyTime > 2000)
				{
					KeyValue.KeyFig = 0;
					LogicRun_Set(LogicCheck);
					
				}
			
			}
			else
			{
				KeyValue.KeyFig = 0;
			
			}
		
		}
	
	}


}
void LogicCheck_Task(void)
{
	if(LogicRun.Runstate == 0)
	{
		LogicRun.Runstate = 1;
		LED_set = 1;
	}
	else if(KeyValue.KeyFig)
	{
		if(KeyValue.KeyValue == KEYL1)
		{
			if(KeyValue.KeyDownFig == 1)
			{
				if(KeyValue.KeyTime > 2000)
				{
					KeyValue.KeyFig = 0;
				
					
				}
			
			}
			else
			{
				KeyValue.KeyFig = 0;
					LogicRun_Set(LogicRunNormal);
			}
		
		}
	
	}
}

void LogicRun_Task(void)
{
	INT8U Logic_Index = LogicRun.state;
			if(GET_MS_SIG(BEEPMSTIME) > 0)
		{
			CLR_MS_SIG(BEEPMSTIME);
			switch(Logic_Index)
			{
				case LogicRunNormal:
					LogicRunNormal_Task();
				
					break;
//				case LogicRunSet:
//					LogicRunSet_Task();
//					break;
				case LogicCheck:
					LogicCheck_Task();
					break;
			}
		}
}
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
