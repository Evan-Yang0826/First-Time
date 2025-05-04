/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    SystemParameter.h
  **  描    述:    系统参数管理
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/10/25   Joshua    创建
************************************************************************************************/
#ifndef __SYSTEMPARAMETER_H__
#define __SYSTEMPARAMETER_H__

/* 本地预定义 */
#ifdef SYSTEMPARAMETER_LOCAL
    #define EXT_SYSTEMPARAMETER
#else
    #define EXT_SYSTEMPARAMETER    extern
#endif


/* 接近感应设置存储位 */
#define SYS_SENSI_SAVE		(0)
	
/* 运行模式存储 */
#define SYS_CONMODE_SAVE	(1)

/* 主ID */	
#define SYS_MADDR_SAVE		(2)
	
	
	/*  地址循环存储起始位 存储区域 1Kb*/	
#define ADDR_SAVEADDR		(0x08000000 + 1024*62)
	
	/*  参数循环存储起始位 存储区域 1Kb*/	
#define SYSDATA_SAVEADDR	(0x08000000 + 1024*61)
	
	/*  系统参数循环存储起始位 存储区域 1Kb*/	
#define SYSSTATUS_SAVEADDR	(0x08000000 + 1024*63)

typedef struct
{
	INT8U 	Addr;			//  地址
	INT8U 	Paneltype;		// 设备类型
	INT8U	Index;			// 存储序号
	INT8U	Version;		// 版本号
}ADDRSAVE_TYPE;

typedef struct
{
	INT8U Delay_Status;
	INT8U ActiveTime1;
	INT16U NetId;
	INT8U Value3;
	INT8U chk;
	
	INT8U BeepFig 	: 1;
	INT8U MotorFig 	: 1;
	INT8U SendFig	: 1;
	INT8U SensiFig	: 1;	/* 接近感应开关，0 - 开；1 - 关 */
}SysStatusType;

EXT_SYSTEMPARAMETER SysStatusType SysStatus;
EXT_SYSTEMPARAMETER void SystemStatusRead(void);
EXT_SYSTEMPARAMETER void SystemStatusSave(void);
EXT_SYSTEMPARAMETER void SystemStatusErase(void);
EXT_SYSTEMPARAMETER void SystemParameterDefault(void);
EXT_SYSTEMPARAMETER void AddrSaveToFlash(INT8U Addr);
EXT_SYSTEMPARAMETER INT8U AddrReadFromFlash(void);
EXT_SYSTEMPARAMETER void AddrEraseData(void);
EXT_SYSTEMPARAMETER ADDRSAVE_TYPE*  ReadParameterFromFlash(void);

EXT_SYSTEMPARAMETER void SysDataSave(INT8U data,INT8U num);
EXT_SYSTEMPARAMETER INT8U SysDataRead(INT8U num);
#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
