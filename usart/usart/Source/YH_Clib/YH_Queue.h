/****************************************Copyright (c)******************************************
**
**          ܲ��������   Copyright(C) 2011, Yehhon ,All rights reserved.
**
**-------------------------------------�ļ���Ϣ--------------------------------------------
  **  �� �� ��: 
  **  ��    ��: 
  **  �� �� ��:
  **  ��Χ����:    ȫ����ͷ�ļ�    
**--------------------------------------------------------------------------------------------
  **  �޸���ʷ:
  **  2011/09/19   Devil   ����
************************************************************************************************/

#ifndef  __YH_QUEUE_H__ 
#define  __YH_QUEUE_H__ 

/* ����Ԥ���� */
#ifdef YH_QUEUE_LOCAL   
  	#define  EXT_YH_QUEUE    
#else  
	#define  EXT_YH_QUEUE  extern
#endif   

/* �ַ����нṹ����:���в�������Ϊ�ַ� */
typedef struct 
{
    INT32U     INIndex;                 // ��������
    INT32U     OUTIndex;                // ��������        
    INT32U     QMAXLong;                // �������ռ� 
    INT8U      *QBuf;                   // ָ��洢���ݵĿռ��ַ 
}YH_Queue8_TYPE;


EXT_YH_QUEUE INT32U YH_Queue8_Creat(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U MaxLong);
EXT_YH_QUEUE INT32U YH_Queue8_In(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U CLong);
EXT_YH_QUEUE INT32U YH_Queue8_Out(YH_Queue8_TYPE *Q8,INT8U *Buf,INT32U CLong);

#endif  /* __YH_QUEUE_H__ */



/*************************************************************************************************
**					����������        END FILE
**************************************************************************************************/
