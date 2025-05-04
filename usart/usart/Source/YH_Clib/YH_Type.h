/****************************************Copyright (c)******************************************
**
**          懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名:    type.h
  **  描    述:    定义与处理器无关的变量类型
  **  创 建 人:	   Devil
  **  范围性质:    全局性头文件    
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2011/09/19   Devil   建立
************************************************************************************************/

#ifndef __YH_TYPE_H__
#define __YH_TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

typedef unsigned long  DWORD;

typedef unsigned char  uint8;                     	/* 无符号8位整型变量      */                      
typedef signed   char  int8;                    	/* 有符号8位整型变量      */
                        
typedef unsigned short uint16;                    	/* 无符号16位整型变量     */                      
typedef signed   short int16;                   	/* 有符号16位整型变量     */ 
                     
typedef unsigned int   uint32;                    	/* 无符号32位整型变量     */                      
typedef signed   int   int32;                   	/* 有符号32位整型变量     */
                     
typedef float          fp32;                    	/* 单精度浮点数（32位长度） */                
typedef double         fp64;                    	/* 双精度浮点数（64位长度） */

typedef unsigned long long uint64;				 	/* 无符号64位整型变量       */
typedef unsigned short     U16;					 



typedef unsigned char  INT8U;                     	/* 无符号8位整型变量      */                      
typedef signed   char  INT8S;                    	/* 有符号8位整型变量      */
                        
typedef unsigned short INT16U;                    	/* 无符号16位整型变量     */                      
typedef signed   short INT16S;                   	/* 有符号16位整型变量     */ 
                     
typedef unsigned int   INT32U;                    	/* 无符号32位整型变量     */                      
typedef signed   int   INT32S;                   	/* 有符号32位整型变量     */
                     
typedef float          FP32;                    	/* 单精度浮点数（32位长度） */                
typedef double         FP64;                    	/* 双精度浮点数（64位长度） */

typedef unsigned long long INT64U;				 	/* 无符号64位整型变量       */

typedef     	 char  INT8;
typedef short	 int   INT16;
typedef 		 int   INT32;            

#endif  /* __YH_TYPE_H__ */




/*************************************************************************************************
**					　　　　　END FILE
**************************************************************************************************/
