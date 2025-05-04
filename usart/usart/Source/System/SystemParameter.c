/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    SystemParameter.c
  **  描    述:    系统参数管理
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2018/10/25   Joshua    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"
#include "../YH_CLib/YH_Clibrary.h"
#include "../SystemStorage/SystemStorage.h"

/* 本地头文件 */
#define SYSTEMPARAMETER_LOCAL
#include "allinclude.h"
/*************************************************************************************************
** 函数名称:    SystemParameterRead
** 功能描述:    
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SystemStatusRead(void)
{
	INT32U i;
	INT8U *p;
	INT8U rq;
	INT8U chk = 0;
	INT8U Savechk;
	
	p = (INT8U*)&SysStatus;
	
	YHMemoryClear((INT8U*)&SysStatus,sizeof(SysStatus));
	
	rq = SysStorageRead(SYSSTATUS_SAVEADDR,&SysStatus,sizeof(SysStatus));
	
	if(rq == TRUE)
	{
		Savechk = SysStatus.chk;
		SysStatus.chk = 0;
		for(i=0;i<sizeof(SysStatus);i++)
		{
			chk += p[i];
		}
		
		if(chk != Savechk)
		{
			rq = FALSE;
		}
	}
	
	if(rq == FALSE)
	{
		SysStatus.NetId = 0;
		SysStatus.Value3 = 0;
		SysStatus.ActiveTime1 = 0;
		SysStatus.Delay_Status = 0;
		SysStatus.MotorFig = 1;
		SysStatus.BeepFig = 1;
		SysStatus.SendFig = 0;
		SysStatus.SensiFig = 0;
	}
}
/*************************************************************************************************
** 函数名称:    SystemParameterDefault
** 功能描述:    系统参数复位
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SystemParameterDefault(void)
{
	AddrEraseData();
	//FLASH_ErasePage(SCENCE_SAVEADDR);
	FLASH_ErasePage(SYSSTATUS_SAVEADDR);
}
/*************************************************************************************************
** 函数名称:    SystemParameterSave
** 功能描述:     
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SystemStatusSave(void)
{
	INT32U i;
	INT8U *p;
	INT8U chk = 0;
	
	p = (INT8U*)&SysStatus;
	
	SysStatus.chk = 0;
	for(i=0;i<sizeof(SysStatus);i++)
	{
		chk += p[i];
	}
	SysStatus.chk = chk;
	
	SysStorageSave(SYSSTATUS_SAVEADDR,&SysStatus,sizeof(SysStatus));
}
/*************************************************************************************************
** 函数名称:    SystemStatusErase
** 功能描述:     
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SystemStatusErase(void)
{
	FLASH_Unlock();
	FLASH_ErasePage(SYSSTATUS_SAVEADDR);
}
/*************************************************************************************************
** 函数名称:    ScenceDataIsEmpty
** 功能描述:    校验是否全部为 0xFF
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    TRUE 表示存储位置空，可以写入数据
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U ScenceDataIsEmpty(INT8U *data,INT32U Long)
{
	INT8U i;
	for(i=0;i<Long;i++)
	{
		if(data[i] != 0xFF)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*************************************************************************************************
** 函数名称:    Sub_FalshWrite
** 功能描述:    写入数据到 FLASH
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
unsigned char Sub_FalshWrite(unsigned int addr,unsigned char *pData)
{
	FLASH_Unlock();
    if(FLASH_ProgramWord(addr,*(int*)pData) !=FLASH_COMPLETE)
    {
        return 0;
    }

//    if(FLASH_ProgramWord(addr + 4,*(int*)(pData + 4))!=FLASH_COMPLETE)
//    {
//        return 0;
//    }

    return 1;
}
/*************************************************************************************************
** 函数名称:    AddrSaveToFlash
** 功能描述:    存入一个地址
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void AddrSaveToFlash(INT8U Addr)
{
	INT32U i;	
	ADDRSAVE_TYPE addrtemp;

	//  扫描存储区找到空位
	for(i = 0;i < 250;i++)
	{
		// 检验为空
		if(ScenceDataIsEmpty((INT8U*)(ADDR_SAVEADDR + i*4),4) == TRUE)
		{
			addrtemp.Addr = Addr;
			addrtemp.Index = i;
			addrtemp.Paneltype = DEVICE_TYPE;
			addrtemp.Version = VERSION_NUM;
			Sub_FalshWrite(ADDR_SAVEADDR + i*4,(INT8U*)&addrtemp);
			return;
		}
	}
	
	FLASH_ErasePage(ADDR_SAVEADDR);
	
	addrtemp.Addr = Addr;
	addrtemp.Index = 0;
	addrtemp.Paneltype = DEVICE_TYPE;
	addrtemp.Version = VERSION_NUM;
	Sub_FalshWrite(ADDR_SAVEADDR,(INT8U*)&addrtemp);
}
/*************************************************************************************************
** 函数名称:    AddrReadFromFlash
** 功能描述:    读取一个地址
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U AddrReadFromFlash(void)
{
	INT32U i;	
	ADDRSAVE_TYPE *addrtemp;
	INT8U  addr = 0x01;

	//  扫描存储区找到空位
	for(i = 0;i < 256;i++)
	{
		// 检验为空
		if(ScenceDataIsEmpty((INT8U*)(ADDR_SAVEADDR + i*4),4) == TRUE)
		{
			return addr;
		}
		
		addrtemp = (ADDRSAVE_TYPE*)(ADDR_SAVEADDR + i*4);
		addr = addrtemp->Addr;
	}
	return addr;
}
/*************************************************************************************************
** 函数名称:    AddrReadFromFlash
** 功能描述:    读取一个地址
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
ADDRSAVE_TYPE*  ReadParameterFromFlash(void)
{
	INT32U i;	
	ADDRSAVE_TYPE *addrtemp;

	addrtemp = 0;
	//  扫描存储区找到空位
	for(i = 0;i < 256;i++)
	{
		// 检验为空
		if(ScenceDataIsEmpty((INT8U*)(ADDR_SAVEADDR + i*4),4) == TRUE)
		{
			return addrtemp;
		}
		
		addrtemp = (ADDRSAVE_TYPE*)(ADDR_SAVEADDR + i*4);

	}
	return 0;
}
/*************************************************************************************************
** 函数名称:    AddrEraseData
** 功能描述:    擦除地址存储区
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void AddrEraseData(void)
{
	FLASH_Unlock();
	FLASH_ErasePage(ADDR_SAVEADDR);
}
/*************************************************************************************************
** 函数名称:    SysDataSaveToFlash
** 功能描述:    存入一个地址
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SysDataSaveToFlash(INT8U *data)
{
	INT32U i;	

	//  扫描存储区找到空位
	for(i = 0;i < 250;i++)
	{
		// 检验为空
		if(ScenceDataIsEmpty((INT8U*)(SYSDATA_SAVEADDR + i*4),4) == TRUE)
		{
			Sub_FalshWrite(SYSDATA_SAVEADDR + i*4,data);
			return;
		}
	}
	
	FLASH_ErasePage(SYSDATA_SAVEADDR);
	
	Sub_FalshWrite(SYSDATA_SAVEADDR,data);
}
/*************************************************************************************************
** 函数名称:    SysDataReadFromFlash
** 功能描述:     读取参数
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    TRUE - 读取成功
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U SysDataReadFromFlash(INT8U *data)
{
	INT32U i,j;	
	//  扫描存储区找到空位
	for(i = 0;i < 256;i++)
	{
		// 检验为空
		if(ScenceDataIsEmpty((INT8U*)(SYSDATA_SAVEADDR + i*4),4) == TRUE)
		{
			if(i == 0) return FALSE;
			return TRUE;
		}
		
		for(j=0;j<4;j++)
		{
			data[j] = *(INT8U*)(SYSDATA_SAVEADDR + i*4 + j);
		}
	}
	return FALSE;
}
/*************************************************************************************************
** 函数名称:    SysDataRead
** 功能描述:     读取参数
** 输　  入:    num - 最大为3
** 输　  出:    无
** 返 回 值:    TRUE - 读取成功
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
INT8U SysDataRead(INT8U num)
{
	INT8U data[4];
	if(num > 3) return 0;
	if(SysDataReadFromFlash(data) == TRUE)
	{
		return data[num];
	}
	return 0;
}
/*************************************************************************************************
** 函数名称:    SysDataSave
** 功能描述:     读取参数
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/13   Joshua    创建
*************************************************************************************************/
void SysDataSave(INT8U data,INT8U num)
{
	INT8U temp[4];
	if(num > 3) return;
	if(SysDataReadFromFlash(temp) == TRUE)
	{
		if(temp[num] != data)
		{
			temp[num] = data;
			SysDataSaveToFlash(temp);
		}
	}
	else
	{
		temp[0] = 0;
		temp[1] = 0;
		temp[2] = 0;
		temp[3] = 0;
		temp[num] = data;
		SysDataSaveToFlash(temp);
	}
	
}
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
