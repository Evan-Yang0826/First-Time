/****************************************Copyright (c)******************************************
**
**          懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名: 
  **  描    述: 
  **  创 建 人:
  **  范围性质:    全局性头文件    
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2011/09/19   Devil   建立
************************************************************************************************/

#ifndef  __YH_QUEUE_H__ 
#define  __YH_QUEUE_H__ 

/* 本地预定义 */
#ifdef YH_QUEUE_LOCAL   
  	#define  EXT_YH_QUEUE    
#else  
	#define  EXT_YH_QUEUE  extern
#endif   

/* 字符队列结构类型:队列操作对象为字符 */
typedef struct 
{
    INT32U     INIndex;                 // 出列索引
    INT32U     OUTIndex;                // 入列索引        
    INT32U     QMAXLong;                // 队列最大空间 
    INT8U      *QBuf;                   // 指向存储数据的空间地址 
}YH_Queue8_TYPE;


EXT_YH_QUEUE INT32U YH_Queue8_Creat(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U MaxLong);
EXT_YH_QUEUE INT32U YH_Queue8_In(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U CLong);
EXT_YH_QUEUE INT32U YH_Queue8_Out(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U CLong);

#endif  /* __YH_QUEUE_H__ */



/*************************************************************************************************
**					　　　　　        END FILE
**************************************************************************************************/
