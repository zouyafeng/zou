#include "timer.h"
#include "led.h"
#include "usart.h"

#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!


//ͨ��������������
u16 ch1_unm=20,ch2_unm=20,ch3_unm=20,ch4_unm=20,ch5_unm=20,ch6_unm=20;


u16 ch1_time=0,ch2_time=0,ch3_time=0,ch4_time=0,ch5_time=0,ch6_time=0;

extern  u8 ch1_start,ch2_start,ch3_start,ch4_start,ch5_start,ch6_start;



extern u8 tong_start;	
extern u8 system_start;
extern u8 system_start_EL;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	
	
	 OSIntEnter();
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
			
		if((system_start>=1)&&(system_start_EL>=1) )

  		{
			
					if((!(tong_start&0x20))&& ch1_start )
					{
							ch1_time++;
							layout_ch1_pin();
					}
						
					if((!(tong_start&0x10))&& ch2_start )
					{
							
						  ch2_time++;
							layout_ch2_pin();
					}
					
							if((!(tong_start&0x08))&& ch3_start )
					{
						
							ch3_time++;
						layout_ch3_pin();
					}
								if((!(tong_start&0x04))&& ch4_start )
					{
						ch4_time++;	
						layout_ch4_pin();	
					}
								
					if((!(tong_start&0x02))&& ch5_start )
					{
						ch5_time++;
						 layout_ch5_pin();
					}
							
						if((!(tong_start&0x01))&& ch6_start )
					{
						ch6_time++;
						layout_ch6_pin();
					}
			

}
		
		}
		
		OSIntExit();
		
		
}




//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);  //Timer3������ӳ��  TIM3_CH2->PB5    GPIO_FullRemap_TIM3    GPIO_PartialRemap_TIM3
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;        //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);           //��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
  
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}
