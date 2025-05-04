/****************************************Copyright (c)******************************************
**
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    Button_Read.h
  **  描    述:    SX8636按键读取
  **  创 建 人:    Devil
**----------------------------------------------------------------------------------------------
  **  修改历史:
  **  2018/10/17   Devil    创建
************************************************************************************************/
#ifndef __BUTTON_READ_H__
#define __BUTTON_READ_H__

/* 本地预定义 */
#ifdef BUTTON_READ_LOCAL
#define EXT_BUTTON_READ
#else
#define EXT_BUTTON_READ    extern
#endif

#define KEYL1 0x04
#define KEYL2 0x10
#define KEYL3 0x40

#define KEYR1 0x02
#define KEYR2 0x08
#define KEYR3 0x20



// 网络设定模式
#define KEY_SETADDR 	(KEYL3 | KEYR3)

// 群组设置模式
#define KEY_SETSCENE 	(KEYL1 | KEYR1)

// 网络设定模式
#define KEY_SETNET 		(KEYL3 | KEYR3)

// 群组设置模式
#define KEY_SETGROUP 	(KEYL3 | KEYR1)

// 接近感应设置
#define KEY_SETSENSIT 	(KEYR3 | KEYR1) 

// 控制模式设置
#define KEY_SETMODE		(KEYL1 | KEYR3)

// 配置分身
#define KEY_SETCOPY 	(KEYL3 | KEYR2)

#define SET_DELAY_TIME 	(KEYL1 | KEYR3)
#define LEDL1 0x04
#define LEDL2 0x02
#define LEDL3 0x01

#define LEDR1 0x20
#define LEDR2 0x10
#define LEDR3 0x08


#define ALL_LED (LEDL1 | LEDL2 | LEDL3 | LEDR1 | LEDR2 | LEDR3)


typedef struct
{
	INT8U  Fig;			/* 0 - 灭；1 - 亮； 2 - 闪烁模式；*/
	INT16U OnTime;
	INT16U OffTime;
	INT8U  LED_IO;
	INT32U TimeCount;
}LedType;

typedef struct
{
	INT8U KEY;
	INT32U Time;
}KeyTimeType;



/* 按键值类 */
typedef struct
{
    INT32U  BacklightTime;  // 背光灯延时关闭计时 （单位 ms）
	INT32U  ResetCount;
    INT8U	AutoAddrStatus; // 自动分配地址状态   （1 - 开始分配地址；2 - 正在分配地址；3 - 地址分配结束）
    INT8U	LEDStatus;		// 用于存储当前灯光状态

    INT8U   KeyDownFig;		// 标志用户是否释放按键
    INT8U   KeyFig;			// 标志用户是否释放按键
	INT8U	MotorFig;
    INT8U	KeyValue;
    INT32U	KeyTime;		// 记录按键按下时间 (单位 ms)
    INT8U	RunStatus;		// 当前系统运行状态	（0 - 正常；1 - 地址分配模式；2 - 场景设置模式）
    INT8U	SenceSetLed;
	INT8U	OldSetLed;
	
	INT8U 	SoftKeyFig;
	
	INT8U	LedBlinkFig;	/* 0 - 正常模式；1 - 闪烁模式  */
	LedType Led1;
	LedType Led2;
	LedType Led3;
	LedType Led4;
	LedType Led5;
	LedType Led6;
} KEY_TYPE;

typedef struct
{
	GPIO_TypeDef *IO_pin;
	INT16U Pin_set;
}KeyIOType;

#define LED_Gpio_Pin GPIO_Pin_9
#define LED_set   PBout(9)

/* 按键值存储 */
EXT_BUTTON_READ KEY_TYPE KeyValue;

EXT_BUTTON_READ void Button_Read_Init(void);
EXT_BUTTON_READ void Button_Read_Task(void);
EXT_BUTTON_READ void Button_LedOnorOff(INT8U Onled,INT8U OffLed);
EXT_BUTTON_READ void LedStatrBlink(void);
EXT_BUTTON_READ void LedEndBlink(void);
EXT_BUTTON_READ void LedBlinkSet(INT8U Led,INT8U Fig,INT16U OnTime,INT16U OffTime);

#endif
/************************************************************************************************
                                         END FILE
************************************************************************************************/
