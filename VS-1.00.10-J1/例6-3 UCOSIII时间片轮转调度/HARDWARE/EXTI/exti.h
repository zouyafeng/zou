#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define KEY0_PE2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键0
#define KEY0_PE3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键0
#define KEY0_PE4  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0

#define KEY0_PE5  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//读取按键0
#define KEY0_PE7  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)//读取按键0
#define KEY0_PE8  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//读取按键0
#define KEY0_PE9  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)//读取按键0

#define KEY0_PE11  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//读取按键0
#define KEY0_PE12  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//读取按键0
#define KEY0_PE13  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)//读取按键1
#define KEY0_PE14  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)//读取按键2 
#define KEY0_PE15  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)//读取按键2 

#define KEY0_PD10  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//读取按键0



void check_ch1x6_dk(void);
void EXTIX_Init(void);//外部中断初始化		 					    
#endif

