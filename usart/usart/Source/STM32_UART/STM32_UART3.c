/****************************************Copyright (c)******************************************
**
**                懿航工作室   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------文件信息--------------------------------------------
  **  文 件 名:    STM32_UART3.c 
  **  描    述:    STM32 UART3驱动文件
  **  创 建 人:    易仕尧
**--------------------------------------------------------------------------------------------
  **  修改历史:
  **  2011/09/19   易仕尧   建立 
************************************************************************************************/
#include "stm32f10x.h"

#include "../YH_CLib/YH_Type.h"
#include "../YH_CLib/YH_Clibrary.h"

#define STM32_UART3LOCAL
#include "UART_Public.h"
#include "STM32_UART3.h"

#define U3_SENT_LONG	   (200)
#define U3_REC_LONG		   (800)

UART_TYPE STM32_UART3;

static INT8U U3_RE_buf[U3_REC_LONG+2];
static INT8U U3_TE_buf[U3_SENT_LONG+2];

/*************************************************************************************************
** 函数名称:    USART3_IRQHandler
** 功能描述: 	
** 输　  入:    无
** 输　  出:    无     
** 返 回 值:    无
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
void USART3_IRQHandler(void)
{
	INT8U temp;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		temp=USART3->DR;	
	    YH_Queue8_In(&STM32_UART3.RE_q8,(INT8U *)&temp,1);

		Usart3RevBusyFig = 1;
	} 
	else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		temp = USART3->SR;
		temp = USART3->DR;
		Usart3RevBusyFig = 0;
	}

}

/*************************************************************************************************
** 函数名称:    STM32_UART3_DMA_Init
** 功能描述: 	串口3DMA初始化
** 输　  入:    无
** 输　  出:    无     
** 返 回 值:    无
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
static INT32U STM32_UART3_DMA_Init(void)
{
	DMA_InitTypeDef DMAStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMAStruct.DMA_PeripheralBaseAddr=(u32)&USART3->DR;
	DMAStruct.DMA_MemoryBaseAddr=(u32)U3_TE_buf;
	DMAStruct.DMA_DIR=DMA_DIR_PeripheralDST;
	DMAStruct.DMA_BufferSize=10;
	DMAStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMAStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMAStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMAStruct.DMA_Mode=DMA_Mode_Normal;
	DMAStruct.DMA_Priority=DMA_Priority_High;
	DMAStruct.DMA_M2M=DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2,&DMAStruct);

	return 0;
}
/*************************************************************************************************
** 函数名称:    STM32_U2_OPEN
** 功能描述: 	打开串口2
** 输　  入:    Baud 波特率， StopBit 停止位， Parity 校验位
** 输　  出:    无     
** 返 回 值:    0 打开成功，其他 打开失败
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
INT8U STM32_U3_OPEN(INT32U Baud, INT16U StopBit, INT16U Parity)
{
	USART_InitTypeDef USARTStruct;
	GPIO_InitTypeDef  GPIO_Initstruct;
	NVIC_InitTypeDef  NVIC_InitStructure; 
  	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_Initstruct);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_Initstruct);

	USARTStruct.USART_BaudRate=Baud;
	USARTStruct.USART_WordLength=USART_WordLength_8b;
	USARTStruct.USART_StopBits=StopBit;
	USARTStruct.USART_Parity=Parity;
	USARTStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USARTStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART3,&USARTStruct);
	USART_Cmd(USART3,ENABLE);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);

	STM32_UART3_DMA_Init();

	STM32_UART3.SubPort=COM3;
	STM32_UART3.SE_start=0;
	STM32_UART3.SE_buf=U3_TE_buf;

	YH_Queue8_Creat(&STM32_UART3.RE_q8,U3_RE_buf,U3_REC_LONG);

	return 0;
}
/*************************************************************************************************
** 函数名称:    STM32_U2_CLOSE
** 功能描述: 	关闭串口2
** 输　  入:    无
** 输　  出:    无     
** 返 回 值:    0 关闭成功，其他 打开失败
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
INT8U STM32_U3_CLOSE(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure; 
  	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);

	return 0;
}

/*************************************************************************************************
** 函数名称:    STM32_U2_Read
** 功能描述: 	从串口2中读出指定长度数据
** 输　  入:    无
** 输　  出:    无     
** 返 回 值:    0 读取失败
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
INT32U STM32_U3_Read(INT8U *buf, INT32U MaxLong)
{
	return YH_Queue8_Out(&STM32_UART3.RE_q8,buf,MaxLong);
}
/*************************************************************************************************
** 函数名称:    STM32_U2_Write
** 功能描述: 	向串口2中写入指定长度数据
** 输　  入:    无
** 输　  出:    无     
** 返 回 值:    0 写入失败
** 先决条件: 
** 全局变量: 
** 调用模块: 
**--------------------------------------------------------------------------------------------
** 修改记录:
** 2011/09/19   易仕尧   建立  
*************************************************************************************************/
INT32U STM32_U3_Write(INT8U *buf, INT32U MaxLong)
{
	// 判断发送长度是否有效
	if( (MaxLong == 0) || (MaxLong > U3_SENT_LONG) )
	{
		return 0;
	}
	// 判断数据发送完否
	if(!(DMA1->ISR&(1<<5))&&(STM32_UART3.SE_start==1))
	{
		return 0;
	} 
	if(STM32_UART3.SE_start==0)
	{
		STM32_UART3.SE_start=1;
	}
	DMA1->IFCR|=(1<<5);
	

	YHMemoryCopy(buf,U3_TE_buf,MaxLong);

	DMA1_Channel2->CCR&=~(1<<0);       						//关闭 DMA 传输
	DMA1_Channel2->CMAR=(INT32U)U3_TE_buf; 
	DMA1_Channel2->CNDTR=MaxLong;

	USART3->CR3=1<<7;
	DMA1_Channel2->CCR|=1<<0;          						//开启 DMA 传输
		

    return  MaxLong; 
}
/************************************************************************************************
						                 END FILE
************************************************************************************************/
