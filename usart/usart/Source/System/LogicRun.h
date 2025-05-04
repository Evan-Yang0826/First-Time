/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    LogicRun.h
  **  描    述:    
  **  创 建 人:    Jim
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2023/3/12   Jim    创建
************************************************************************************************/
#ifndef __LOGICRUN_H__
#define __LOGICRUN_H__

/* 本地预定义 */
#ifdef LOGICRUN_LOCAL
    #define EXT_LOGICRUN
#else
    #define EXT_LOGICRUN    extern
#endif
typedef enum
{
	LogicRunNormal,
	LogicRunSet,
	LogicCheck,
}LogicRunEnum;
		
		
typedef struct
{
	LogicRunEnum state;
	INT8U Runstate;
}LogicRun_TYPE;
typedef struct
{
	INT8U Temp_ProValue;
	INT16U Smmog_ProValue;
	INT8U Temp_Set;
	INT16U Smmog;
}Alarm_Set_TYPE;

typedef struct
{
	INT8U Temp_Value;
	INT16U Smmog_Value;
	INT8U Temp_ProValue;
	INT16U Smmog_ProValue;
	
	INT16U year;
	INT8U  month;
	INT8U  day;
	INT8U  hour;
	INT8U  minute;
	INT8U  second;
}Time_Set_TYPE;
EXT_LOGICRUN void LogicRun_Set(LogicRunEnum state);
EXT_LOGICRUN LogicRun_TYPE  LogicRun;
EXT_LOGICRUN Alarm_Set_TYPE Alarm_Storge;
EXT_LOGICRUN INT8U Length;
EXT_LOGICRUN void LogicRun_Task(void);
EXT_LOGICRUN void Alarm_Init(void);
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
