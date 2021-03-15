#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"

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
//�ⲿ�ж�0�������

u8 tan_xue_boo=0; //̽Ѩ��⵽���庯�� ����⵽ѨλʱΪ2�����Ѩλ��Ӧ��Ϊ0��ӦΪ���𶯵�·������Ѩλ�źţ�����ÿ����Ӧ�궼��Ϊ0 .

extern bool tang_bool;  //̽Ѩ��־

//��·��·������ʶ������chX_tong ����·������־��chX_duan��·������־
u16 ch1_tong,ch1_duan,ch2_tong,ch2_duan,ch3_tong,ch3_duan,ch4_tong,ch4_duan,ch5_tong,ch5_duan,ch6_tong,ch6_duan;
//������·��·����־������
u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;

extern u8 ch1_duan_kai;  //ͨ��1��·��·����
extern u8 ch2_duan_kai;  //ͨ��2��·��·����
extern u8 ch3_duan_kai;  //ͨ��3��·��·����
extern u8 ch4_duan_kai;  //ͨ��4��·��·����
extern u8 ch5_duan_kai;  //ͨ��5��·��·����
extern u8 ch6_duan_kai;  //ͨ��6��·��·����




void EXTIX_Init(void)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);	//ʹ�ܸ��ù���ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	
	
	 //��ʼ��̽Ѩ������жϿ� PD10
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;			     // �������
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		  
	  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD.0
	
    //GPIOD.10 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


    //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //GPIOE.4	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


 //GPIOE.5	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //GPIOE.7  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  //GPIOE.8	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���



  //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource9);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���




  //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource14);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
 //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource15);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;					//�����ȼ�4
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 


    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 



}


//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
  OSIntEnter();
	
	 if(KEY0_PE2==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
		//printf("zhong duan 2\r\n");
		 
		 //1ͨ����·����
		 
		 if(ch2_chexe){			 
			ch2_duan++; 
			 
		 }
		 
		 
		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line2); //���LINE0�ϵ��жϱ�־λ  
	
	OSIntExit(); 
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
OSIntEnter();
	
	 if(KEY0_PE3==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
		//printf("zhong duan 3\r\n");
		 
		  //1ͨ����·����
		  if(ch2_chexe){			 
			ch2_tong++; 	 
		 }
		 
		 
		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line3); //���LINE0�ϵ��жϱ�־λ  
	
	OSIntExit();  
}



//�ⲿ�ж�0������� 
void EXTI4_IRQHandler(void)
{
	OSIntEnter();
	
	 if(KEY0_PE4==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
	//	printf("zhong duan 4\r\n");
		 
		 
		 //1ͨ����·����
		 if(ch1_chexe){			 
			ch1_duan++; 
			 
		 }
		 

		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE0�ϵ��жϱ�־λ  
	
	OSIntExit();
	
}
 


void EXTI9_5_IRQHandler(void)
{
	//delay_ms(10);//����
	OSIntEnter();
	
	if(EXTI_GetITStatus(EXTI_Line5)==1){
		
   if(KEY0_PE5==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
		//printf("zhong duan 5\r\n");
		 
		 //1ͨ����·����
		  if(ch1_chexe){			 
			ch1_tong++; 	 
		 }
		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE4�ϵ��жϱ�־λ  
		
	}	
	
	
	
	
	if(EXTI_GetITStatus(EXTI_Line7)==1){
		
   if(KEY0_PE7==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
		//printf("zhong duan 7\r\n");
		 
		 	//3ͨ����·����
		 if(ch3_chexe){			 
			ch3_duan++; 
			 
		 }
		 

		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE4�ϵ��жϱ�־λ  
		
	}		
	
	
if(EXTI_GetITStatus(EXTI_Line8)==1){
		
   if(KEY0_PE8==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
		//printf("zhong duan 8\r\n");
		 
		 //3ͨ����·����
		  if(ch3_chexe){			 
			ch3_tong++; 	 
		 }
		  
		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line8);  //���LINE4�ϵ��жϱ�־λ  
		
	}		
	
	
	
	

	if(EXTI_GetITStatus(EXTI_Line9)==1){
   if(KEY0_PE9==0){
	
		 //1ͨ����·����
		 if(ch6_chexe){			 
			ch6_duan++; 
			 
		 }
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line9);  //���LINE4�ϵ��жϱ�־λ  
		
	}		

	

	
	
	OSIntExit();
}






 void EXTI15_10_IRQHandler(void)
{
	
	
 OSIntEnter();
	
	//delay_ms(10);//����
	
	if(EXTI_GetITStatus(EXTI_Line10)==1){
     
		delay_ms(20);
		
		if(KEY0_PD10==1){
		
		delay_ms(20);
		
  
		 if(KEY0_PD10==1){	
		 delay_ms(10);
		 
		if(KEY0_PD10==1){	
			
		if(tang_bool){
		tan_xue_boo=2;     //��̽Ѩ�ź���Ϊ2 
		}		
	}
		
  }
	
	
		
	 }
		EXTI_ClearITPendingBit(EXTI_Line10);  //���LINE4�ϵ��жϱ�־λ  
	}		

	
	
	if(EXTI_GetITStatus(EXTI_Line11)==1){
   if(KEY0_PE11==0){

	
			//6ͨ����·����
		  if(ch6_chexe){			 
			ch6_tong++; 	 
		 } 
		 
		 
	 }
		EXTI_ClearITPendingBit(EXTI_Line11);  //���LINE4�ϵ��жϱ�־λ  
		
	}		

	
	
	
	
	
	
  if(EXTI_GetITStatus(EXTI_Line12)==1){


		if(KEY0_PE12==0){
			//	tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
	//	printf("zhong duan 12\r\n");
			
				 //1ͨ����·����
		 if(ch5_chexe){			 
			ch5_duan++; 
			 
		 }
		 
			
			
		}
		EXTI_ClearITPendingBit(EXTI_Line12);  //���LINE4�ϵ��жϱ�־λ  
		
	}		


	
	if(EXTI_GetITStatus(EXTI_Line13)==1){
		
  if(KEY0_PE13==0){
	//	tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
	//	printf("zhong duan 13\r\n");
		
			//4ͨ����·����
		  if(ch5_chexe){			 
			ch5_tong++; 	 
		 } 
		 
		
		
	}
		EXTI_ClearITPendingBit(EXTI_Line13);  //���LINE4�ϵ��жϱ�־λ  
		
	}		
	
	
	if(EXTI_GetITStatus(EXTI_Line14)==1){
   if(KEY0_PE14==0){
		//tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
	//	printf("zhong duan 14\r\n");
		 
		 	 //1ͨ����·����
		 if(ch4_chexe){			 
			ch4_duan++; 
			 
		 }
		 

		 
		 
		 
  }
		EXTI_ClearITPendingBit(EXTI_Line14);  //���LINE4�ϵ��жϱ�־λ  
		
	}		


	
	if(EXTI_GetITStatus(EXTI_Line15)==1){
		
   if(KEY0_PE15==0){
	//	tan_xue_boo=2;  //��̽Ѩ�ź���Ϊ2 
	//	printf("zhong duan 15\r\n");
		 
			//4ͨ����·����
		  if(ch4_chexe){			 
			ch4_tong++; 	 
		 } 
		 
		 
		 
	 }
		EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE4�ϵ��жϱ�־λ  
		
	}		
	


	
	OSIntExit();
}



//�ڶ��汾��·��·������
void check_ch1x6_dk(void)
{
	u16 ch1_unmb,ch2_unmb,ch3_unmb,ch4_unmb,ch5_unmb,ch6_unmb;
	
	
	
	//��1ͨ�����д����ж�
	if(ch1_chexe){
		
		//������·���
		if(ch1_duan>ch1_tong){
			
			ch1_unmb=(ch1_duan-ch1_tong);
			if(ch1_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch1_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				
				ch1_duan=3;
				ch1_tong=3;
			
			
				if(ch1_duan>=60000){
					ch1_duan=0;
					ch1_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch1_duan<ch1_tong){
			
			ch1_unmb=(ch1_tong-ch1_duan);
			if(ch1_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch1_duan_kai=30;
				ch1_duan=3;
				ch1_tong=3;
				
				
				if(ch1_tong>=60000){
					ch1_duan=0;
					ch1_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch1_duan>=60000)&&(ch1_tong>=60000))
			
		{
		  ch1_duan=3;
			ch1_tong=3;
			
		}
	
	
	}
	
	
	
		//��1ͨ�����д����ж�
	if(ch2_chexe){
		
		//������·���
		if(ch2_duan>ch2_tong){
			
			ch2_unmb=(ch2_duan-ch2_tong);
			if(ch2_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch2_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				ch2_duan=3;
				ch2_tong=3;
				
				if(ch2_duan>=60000){
					ch2_duan=0;
					ch2_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch2_duan<ch2_tong){
			
			ch2_unmb=(ch2_tong-ch2_duan);
			if(ch2_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch2_duan_kai=30;
				ch2_duan=3;
				ch2_tong=3;
				
				if(ch2_tong>=60000){
					ch2_duan=0;
					ch2_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch2_duan>=60000)&&(ch2_tong>=60000))
			
		{
		  ch2_duan=3;
			ch2_tong=3;
			
		}
	
	
	}
	
	
		//��1ͨ�����д����ж�
	if(ch3_chexe){
		
		//������·���
		if(ch3_duan>ch3_tong){
			
			ch3_unmb=(ch3_duan-ch3_tong);
			if(ch3_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch3_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				ch3_duan=3;
				ch3_tong=3;
				
				
				if(ch3_duan>=60000){
					ch3_duan=0;
					ch3_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch3_duan<ch3_tong){
			
			ch3_unmb=(ch3_tong-ch3_duan);
			if(ch3_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch3_duan_kai=30;
				ch3_duan=3;
				ch3_tong=3;
				
				if(ch3_tong>=60000){
					ch3_duan=0;
					ch3_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch3_duan>=60000)&&(ch3_tong>=60000))
			
		{
		  ch3_duan=3;
			ch3_tong=3;
			
		}
	
	
	}
	
	
		//��1ͨ�����д����ж�
	if(ch4_chexe){
		
		//������·���
		if(ch4_duan>ch4_tong){
			
			ch4_unmb=(ch4_duan-ch4_tong);
			if(ch4_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch4_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				ch4_duan=3;
				ch4_tong=3;
				
				
				if(ch4_duan>=60000){
					ch4_duan=0;
					ch4_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch4_duan<ch4_tong){
			
			ch4_unmb=(ch4_tong-ch4_duan);
			if(ch4_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch4_duan_kai=30;
				ch4_duan=3;
				ch4_tong=3;
				
				if(ch4_tong>=60000){
					ch4_duan=0;
					ch4_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch4_duan>=60000)&&(ch4_tong>=60000))
			
		{
		  ch4_duan=3;
			ch4_tong=3;
			
		}
	
	
	}
	
	
	
		//��1ͨ�����д����ж�
	if(ch5_chexe){
		
		//������·���
		if(ch5_duan>ch5_tong){
			
			ch5_unmb=(ch5_duan-ch5_tong);
			if(ch5_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch5_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				ch5_duan=3;
				ch5_tong=3;
				
				
				if(ch5_duan>=60000){
					ch5_duan=0;
					ch5_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch5_duan<ch5_tong){
			
			ch5_unmb=(ch5_tong-ch5_duan);
			if(ch5_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����
				
				ch5_duan_kai=30;
				ch5_duan=3;
				ch5_tong=3;
				
				if(ch5_tong>=60000){
					ch5_duan=0;
					ch5_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch5_duan>=60000)&&(ch5_tong>=60000))
			
		{
		  ch5_duan=3;
			ch5_tong=3;
			
		}
	
	
	}
	
	
	
		//��1ͨ�����д����ж�
	if(ch6_chexe){
		
		//������·���
		if(ch6_duan>ch6_tong){
			
			ch6_unmb=(ch6_duan-ch6_tong);
			if(ch6_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ����  ��һ���� 30  
				
				ch6_duan_kai=30;  //16λ�����ݴ��������Ϊ65536
				
				ch6_duan=3;
				ch6_tong=3;
				
				if(ch6_duan>=60000){
					ch6_duan=0;
					ch6_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
		}
		//������·���
			if(ch6_duan<ch6_tong){
			
			ch6_unmb=(ch6_tong-ch6_duan);
			if(ch6_unmb>=300){  //30����������������ȿ��أ������ߵ��жϴ˴�����Ļ���������ʾ���� ��һ���� 30  
				
				ch6_duan_kai=30;
				ch6_duan=3;
				ch6_tong=3;
				
				if(ch6_tong>=60000){
					ch6_duan=0;
					ch6_tong=0; //��ô����Ŀ�������ڴ��ڶ�·״̬����ô�´ν��л����ж�Ϊ��·״̬��
					
				}
				
				
			}
			
		}
		
		//û�з�����·��Ҳû�з�����·��ͬʱ�ԱȽ����ݽ�������
		if((ch6_duan>=60000)&&(ch6_tong>=60000))
			
		{
		  ch6_duan=3;
			ch6_tong=3;
			
		}
	
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
}







 
