#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define KEY0_PE2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����0
#define KEY0_PE3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����0
#define KEY0_PE4  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����0

#define KEY0_PE5  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//��ȡ����0
#define KEY0_PE7  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)//��ȡ����0
#define KEY0_PE8  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//��ȡ����0
#define KEY0_PE9  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)//��ȡ����0

#define KEY0_PE11  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//��ȡ����0
#define KEY0_PE12  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//��ȡ����0
#define KEY0_PE13  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)//��ȡ����1
#define KEY0_PE14  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)//��ȡ����2 
#define KEY0_PE15  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)//��ȡ����2 

#define KEY0_PD10  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//��ȡ����0



void check_ch1x6_dk(void);
void EXTIX_Init(void);//�ⲿ�жϳ�ʼ��		 					    
#endif

