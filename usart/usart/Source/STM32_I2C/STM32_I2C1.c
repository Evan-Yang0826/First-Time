/****************************************Copyright (c)******************************************
**
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**----------------------------------------文件信息----------------------------------------------
  **  文 件 名:    STM32_I2C1.c
  **  描    述:    STM32硬件I2C1驱动
  **  创 建 人:    Joshua
**----------------------------------------------------------------------------------------------
  **  修改历史:
  **  2018/12/18   Joshua    创建
************************************************************************************************/
/* 系统头文件 */
#include "stdio.h"
/* 库文件头 */
#include "stm32f10x.h"
#include "../YH_Clib/YH_Type.h"
#include "../STM32_delay/STM32_delay.h"


/* 本地头文件 */
#define STM32_I2C1_LOCAL
#include "STM32_I2C1.h"

#define TimeOut  (60000)

/*************************************************************************************************
** 函数名称:    STM32_I2C1_Init
** 功能描述:    I2C 硬件初始化
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void STM32_I2C1_Init(void)
{
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 配置硬件IIC需要的变量 */
    I2C_InitTypeDef I2C_InitStructure;

    /* PB6-I2C1_SCL、PB7-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*IIC外设初始化*/

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ;     //配置为普通IIC模式
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    //I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;  //使能自动应答
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;   //5K的速度


    /* I2C1 初始化 */
    I2C_Init(I2C1, &I2C_InitStructure);

    /* 使能 I2C1 */
    I2C_Cmd  (I2C1,ENABLE);
    /*允许应答模式*/
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
/*************************************************************************************************
** 函数名称:    Stm32iic1_Reset
** 功能描述:    遇硬件问题后，复位IIC
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Stm32iic1_Reset(void)
{
	/*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
	
	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIOB->BSRR = GPIO_Pin_6;
	GPIOB->BSRR = GPIO_Pin_7;
	
	delay_us(300);
	
	STM32_I2C1_Init();
}
/*************************************************************************************************
** 函数名称:    Stm32iic1_WatiFig
** 功能描述:    查询寄存器标志，带超时，替代库函数
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
INT8U Stm32iic1_WatiFig(INT16U sr1_fig,INT16U sr2_fig)
{
    
	INT32U TimeCount = 0;
	INT8U Fig = SUCCESS;

    TimeCount = 0;
    while(!(((INT16U)(I2C1->SR1) & (INT16U)(sr1_fig)) == sr1_fig))
    {
        TimeCount++;
        if(TimeCount > TimeOut)
		{			
			Fig = ERROR;
			break;
		}
    };

    TimeCount = 0;
    while(!(((INT16U)(I2C1->SR2) & (INT16U)(sr2_fig)) == sr2_fig))
    {
        TimeCount++;
        if(TimeCount > TimeOut)
		{		
			Fig = ERROR;
			break;
		}
    };

	return Fig;
}
/*************************************************************************************************
** 函数名称:    Stm32iic1_Write
** 功能描述:    I2C 写数据
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Stm32iic1_SingleWrite(INT8U Addr,INT8U data)
{	
    I2C_GenerateSTART(I2C1,ENABLE);

	if(Stm32iic1_WatiFig(0x0001,0x0003) == ERROR) {Stm32iic1_Reset(); return;}

    I2C_Send7bitAddress(I2C1,Addr,I2C_Direction_Transmitter);

	if(Stm32iic1_WatiFig(0x0082,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
		
    I2C_SendData(I2C1,data);
	
	if(Stm32iic1_WatiFig(0x0084,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
	
    I2C_GenerateSTOP(I2C1,ENABLE);
}
/*************************************************************************************************
** 函数名称:    Stm32iic1_Write
** 功能描述:    I2C 写数据
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Stm32iic1_Write(INT8U Addr,INT8U Reg,INT8U data)
{
    I2C_GenerateSTART(I2C1,ENABLE);

	if(Stm32iic1_WatiFig(0x0001,0x0003) == ERROR) {Stm32iic1_Reset(); return;}

    I2C_Send7bitAddress(I2C1,Addr,I2C_Direction_Transmitter);

	if(Stm32iic1_WatiFig(0x0082,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
		
    I2C_SendData(I2C1,Reg);

	if(Stm32iic1_WatiFig(0x0084,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
	
    I2C_SendData(I2C1,data);
	
	if(Stm32iic1_WatiFig(0x0084,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
	
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/*************************************************************************************************
** 函数名称:    Stm32iic1_Write
** 功能描述:    I2C 写数据
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
void Stm32iic1_MultiWrite(INT8U Addr,INT8U Reg,INT8U *data,INT8U dataLong)
{
	INT8U i;
	
    I2C_GenerateSTART(I2C1,ENABLE);

	if(Stm32iic1_WatiFig(0x0001,0x0003) == ERROR) {Stm32iic1_Reset(); return;}

	// 发送地址
    I2C_Send7bitAddress(I2C1,Addr,I2C_Direction_Transmitter);

	if(Stm32iic1_WatiFig(0x0082,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
		
	// 寄存器
    I2C_SendData(I2C1,Reg);

	if(Stm32iic1_WatiFig(0x0084,0x0007) == ERROR) {Stm32iic1_Reset(); return;}
	
	// 数据
	for(i = 0; i < dataLong; i++)
	{
		I2C_SendData(I2C1,data[i]);  
		if(Stm32iic1_WatiFig(0x0084,0x0007) == ERROR) {Stm32iic1_Reset(); return;}		
	}
	
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/*************************************************************************************************
** 函数名称:    Stm32iic1_SingleRead
** 功能描述:    I2C 读取一个数据
** 输　  入:    无
** 输　  出:    无
** 返 回 值:    无
** 先决条件:    无
** 全局变量:
** 调用模块:
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2018/10/17   Devil    创建
*************************************************************************************************/
INT8U Stm32iic1_SingleRead(INT8U Addr)
{
    INT8U data;
    INT32U TimeCount = 0;

    
    /*④发送起始信号 */
    I2C_GenerateSTART(I2C1, ENABLE);

    /*检测EV5并清除标志*/
    if(Stm32iic1_WatiFig(0x0001,0x0003) == ERROR) {Stm32iic1_Reset(); return 0;}

    /*⑤发送从机设备地址，读选通*/
    I2C_Send7bitAddress(I2C1, Addr, I2C_Direction_Receiver);

    /*检测EV6并清除标志*/
    TimeCount = 0;
    while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (TimeCount < 80000))
    {
        TimeCount++;
    };

    /*⑦非应答*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    /*⑧产生停止信号*/
    I2C_GenerateSTOP(I2C1, ENABLE);

    /*⑥检测EV7，然后读取数据清除标志*/
    TimeCount = 0;
    while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)) && (TimeCount < 80000))
    {
        TimeCount++;
    };

    data = I2C_ReceiveData(I2C1);

    return data;
}
/************************************************************************************************
                                         END FILE
************************************************************************************************/
