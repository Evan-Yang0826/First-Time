/****************************************Copyright (c)******************************************
**
**                所能科技   Copyright(C) 2012, Soone ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名:    main.c 
  **  描    述:    主程序文件
  **  创 建 人:    Devil
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2011/09/19   Devil   建立 
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"

/* 本地声明头 */
#define STM32_IWDG_LOCAL
#include "STM32_IWDG.h"


void STM32_IWDG_Init(void)
{
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* 看门狗时钟分频 ，低速晶振 4M */
	IWDG_SetPrescaler(IWDG_Prescaler_64);

	IWDG_SetReload(40000/64);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* 使能看门狗 */
	IWDG_Enable();
}


void STM32_IWDG_Feed(void)
{
	IWDG->KR=0xAAAA;
}



/************************************************************************************************
						                 END FILE
************************************************************************************************/
