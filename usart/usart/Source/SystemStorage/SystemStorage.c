/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    SystemStorage.c
  **  描    述:    STM32参数存储管理
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2019/1/17   Devil    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"


/* 本地头文件 */
#define SYSTEMSTORAGE_LOCAL
#include "SystemStorage.h"

/*************************************************************************************************
** 函数名称:    SysStorageIsEmpty
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
INT8U SysStorageIsEmpty(INT8U *data,INT32U Long)
{
    INT8U i;
    for(i=0; i<Long; i++)
    {
        if(data[i] != 0xFF)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/*************************************************************************************************
** 函数名称:    ScenceDataFalshWrite
** 功能描述:    数据写入FLASH
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
void ScenceDataFalshWrite(INT32U Addr,INT8U *pData,INT32U DataLong)
{
	INT32U i;
	INT32U intsize = 0;
	
	intsize = DataLong/4;
	
	FLASH_Unlock();
	for(i=0;i<intsize;i++)
	{
		FLASH_ProgramWord(Addr + i*4,*(int*)(pData + i*4));
	}
}
/*************************************************************************************************
** 函数名称:    SysStorageSave
** 功能描述:    存储数据
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
INT8U SysStorageSave(INT32U Addr,void *data,INT32U DataLong)
{
	INT32U i;	

	INT32U TotalBlock;
	INT32U BlockSize;
	
	if((Addr % 1024) != 0) return FALSE;
	
	if((DataLong % 4) == 0)
	{
		BlockSize = DataLong;
	}
	else
	{
		BlockSize = (DataLong/4)*4 + 4;
	}
	
	TotalBlock = 1024 / BlockSize - 1;
	
	//  扫描存储区找到空位
	for(i = 0;i < TotalBlock;i++)
	{
		// 检验为空
		if(SysStorageIsEmpty((INT8U*)(Addr + i*BlockSize),BlockSize) == TRUE)
		{
			ScenceDataFalshWrite(Addr + i*BlockSize,(INT8U*)data,BlockSize);
			return TRUE;
		}
	}
	
	FLASH_Unlock();
	FLASH_ErasePage(Addr);
	
	ScenceDataFalshWrite(Addr,(INT8U *)data,BlockSize);
	
	return TRUE;
}
/*************************************************************************************************
** 函数名称:    SysStorageRead
** 功能描述:    读取数据
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
INT8U SysStorageRead(INT32U Addr,void *data,INT32U DataLong)
{
	INT32U i,j;	
	
	INT32U TotalBlock;
	INT32U BlockSize;
	
	if((Addr % 1024) != 0) return FALSE;
	
	if((DataLong % 4) == 0)
	{
		BlockSize = DataLong;
	}
	else
	{
		BlockSize = (DataLong/4)*4 + 4;
	}
	
	TotalBlock = 1024 / BlockSize - 1;
	
	//  扫描存储区找到空位
	for(i = 0;i < TotalBlock;i++)
	{
		// 检验为空
		if(SysStorageIsEmpty((INT8U*)(Addr + i*BlockSize),BlockSize) == TRUE)
		{
			if(i == 0) return FALSE;
			return TRUE;
		}
		
		for(j=0;j<DataLong;j++)
		{
			((INT8U *)data)[j] = *(INT8U*)(Addr + i*BlockSize + j);
		}
	}
	return FALSE;
}
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
