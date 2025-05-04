/****************************************Copyright (c)******************************************
**
**                ܲ��������   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------�ļ���Ϣ--------------------------------------------
  **  �� �� ��:    STM32_UART.c 
  **  ��    ��:    STM32 UART�����ļ�
  **  �� �� ��:    Devil
**--------------------------------------------------------------------------------------------
  **  �޸���ʷ:
  **  2011/09/19   Devil   ���� 
************************************************************************************************/
#include "stm32f10x.h"
#include "../YH_CLib/YH_Type.h"
#include "../YH_CLib/YH_Clibrary.h"

#define STM32_UART1LOCAL
#include "UART_Public.h"
#include "STM32_UART1.h"

#define U1_SENT_LONG	   (1000)
#define U1_REC_LONG		   (1000)

UART_TYPE STM32_UART1;

static INT8U U1_RE_buf[U1_REC_LONG+2];
static INT8U U1_TE_buf[U1_SENT_LONG+2];

/*************************************************************************************************
** ��������:    USART1_IRQHandler
** ��������: 	����1�ж�
** �䡡  ��:    ��
** �䡡  ��:    ��     
** �� �� ֵ:    ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
void USART1_IRQHandler(void)
{
	INT8U temp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 
 		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
 		temp=USART1->DR;
 		YH_Queue8_In(&STM32_UART1.RE_q8,(INT8U *)&temp,1);   
	} 
	 
}
/*************************************************************************************************
** ��������:    STM32_UART1_DMA_Init
** ��������: 	����1DMA��ʼ��
** �䡡  ��:    ��
** �䡡  ��:    ��     
** �� �� ֵ:    ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
static INT32U STM32_UART1_DMA_Init(void)
{
	DMA_InitTypeDef DMAStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMAStruct.DMA_PeripheralBaseAddr=(u32)&USART1->DR;
	DMAStruct.DMA_MemoryBaseAddr=(u32)U1_TE_buf;
	DMAStruct.DMA_DIR=DMA_DIR_PeripheralDST;
	DMAStruct.DMA_BufferSize=10;
	DMAStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMAStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMAStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMAStruct.DMA_Mode=DMA_Mode_Normal;
	DMAStruct.DMA_Priority=DMA_Priority_High;
	DMAStruct.DMA_M2M=DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4,&DMAStruct);

	return 0;
}
/*************************************************************************************************
** ��������:    STM32_U1_OPEN
** ��������: 	�򿪴���1
** �䡡  ��:    Baud �����ʣ� StopBit ֹͣλ�� Parity У��λ
** �䡡  ��:    ��     
** �� �� ֵ:    0 �򿪳ɹ������� ��ʧ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
INT8U STM32_U1_OPEN(INT32U Baud, INT16U StopBit, INT16U Parity)
{
	USART_InitTypeDef USARTStruct;
	GPIO_InitTypeDef  GPIO_Initstruct;
	NVIC_InitTypeDef  NVIC_InitStructure; 
  	
 // 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_Initstruct);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_Initstruct);

	USARTStruct.USART_BaudRate=Baud;
	USARTStruct.USART_WordLength=USART_WordLength_8b;
	USARTStruct.USART_StopBits=StopBit;
	USARTStruct.USART_Parity=Parity;
	USARTStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USARTStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1,&USARTStruct);
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

	STM32_UART1_DMA_Init();

	STM32_UART1.SubPort=COM1;
	STM32_UART1.SE_start=0;
	STM32_UART1.SE_buf=U1_TE_buf;

	YH_Queue8_Creat(&STM32_UART1.RE_q8,U1_RE_buf,U1_REC_LONG);

/* �����ض���ʹ�� */
#if GPIO_AFIO_UART1_ENABLE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_Initstruct);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_Initstruct);
#endif

	return 0;
}
/*************************************************************************************************
** ��������:    STM32_U1_CLOSE
** ��������: 	�رմ���1
** �䡡  ��:    ��
** �䡡  ��:    ��     
** �� �� ֵ:    0 �رճɹ������� ��ʧ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
INT8U STM32_U1_CLOSE(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure; 
  	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);

	return 0;
}

/*************************************************************************************************
** ��������:    STM32_U1_Read
** ��������: 	�Ӵ���1�ж���ָ����������
** �䡡  ��:    ��
** �䡡  ��:    ��     
** �� �� ֵ:    0 ��ȡʧ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
INT32U STM32_U1_Read(INT8U *buf, INT32U MaxLong)
{
	return YH_Queue8_Out(&STM32_UART1.RE_q8,buf,MaxLong);
}
/*************************************************************************************************
** ��������:    STM32_U1_Write
** ��������: 	�򴮿�1��д��ָ����������
** �䡡  ��:    ��
** �䡡  ��:    ��     
** �� �� ֵ:    0 д��ʧ��
** �Ⱦ�����: 
** ȫ�ֱ���: 
** ����ģ��: 
**--------------------------------------------------------------------------------------------
** �޸ļ�¼:
** 2011/09/19   Devil   ����  
*************************************************************************************************/
INT32U STM32_U1_Write(INT8U *buf, INT32U MaxLong)
{
	// �жϷ��ͳ����Ƿ���Ч
	if( (MaxLong == 0) || (MaxLong > U1_SENT_LONG) )
	{
		return 0;
	}
	// �ж����ݷ������
	if(!(DMA1->ISR&(1<<13))&&(STM32_UART1.SE_start==1))
	{
		return 0;
	} 
	if(STM32_UART1.SE_start==0)
	{
		STM32_UART1.SE_start=1;
	}
	DMA1->IFCR|=(1<<13);
	

	YHMemoryCopy(buf,U1_TE_buf,MaxLong);

	DMA1_Channel4->CCR&=~(1<<0);       						//�ر� DMA ����
	DMA1_Channel4->CMAR=(INT32U)U1_TE_buf; 
	DMA1_Channel4->CNDTR=MaxLong;

	USART1->CR3=1<<7;
	DMA1_Channel4->CCR|=1<<0;          						//���� DMA ����
	
    return  MaxLong; 
}

INT32U STM32_U1_Switch(INT8U Fig)
{

	if(Fig==1)
	{
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	}
	else
	{
		GPIO_PinRemapConfig(GPIO_Remap_USART1,DISABLE);
	}

	return 0;	
}
/************************************************************************************************
						                 END FILE
************************************************************************************************/
