/****************************************Copyright (c)******************************************
**                                                                                              
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.                  
**                                                                                              
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    SystemStorage.h
  **  描    述:    STM32参数存储管理
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:    
  **  2019/1/17   Devil    创建
************************************************************************************************/
#ifndef __SYSTEMSTORAGE_H__
#define __SYSTEMSTORAGE_H__

/* 本地预定义 */
#ifdef SYSTEMSTORAGE_LOCAL
    #define EXT_SYSTEMSTORAGE
#else
    #define EXT_SYSTEMSTORAGE    extern
#endif

EXT_SYSTEMSTORAGE INT8U SysStorageSave(INT32U Addr,void *data,INT32U DataLong);
EXT_SYSTEMSTORAGE INT8U SysStorageRead(INT32U Addr,void *data,INT32U DataLong);


#endif
/************************************************************************************************
                                         END FILE                                                
************************************************************************************************/
