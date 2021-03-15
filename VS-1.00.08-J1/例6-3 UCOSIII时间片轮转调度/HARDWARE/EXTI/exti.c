#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"

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
//外部中断0服务程序

u8 tan_xue_boo=0; //探穴检测到定义函数 当检测到穴位时为2，检测穴位相应完为0，应为是震动电路产生的穴位信号，所以每次相应完都置为0 .

extern bool tang_bool;  //探穴标志

//短路开路计数标识，变量chX_tong 检查短路变量标志，chX_duan短路变量标志
u16 ch1_tong,ch1_duan,ch2_tong,ch2_duan,ch3_tong,ch3_duan,ch4_tong,ch4_duan,ch5_tong,ch5_duan,ch6_tong,ch6_duan;
//开启短路开路检查标志变量，
u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;

extern u8 ch1_duan_kai;  //通道1短路开路变量
extern u8 ch2_duan_kai;  //通道2短路开路变量
extern u8 ch3_duan_kai;  //通道3短路开路变量
extern u8 ch4_duan_kai;  //通道4短路开路变量
extern u8 ch5_duan_kai;  //通道5短路开路变量
extern u8 ch6_duan_kai;  //通道6短路开路变量




void EXTIX_Init(void)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);	//使能复用功能时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	
	
	 //初始化探穴输入口中断口 PD10
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;			     // 推挽输出
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		  
	  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.0
	
    //GPIOD.10 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //GPIOE.4	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 //GPIOE.5	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //GPIOE.7  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  //GPIOE.8	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器



  //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource9);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器




  //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource14);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
 //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource15);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;					//子优先级4
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 


    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 



}


//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
  OSIntEnter();
	
	 if(KEY0_PE2==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
		//printf("zhong duan 2\r\n");
		 
		 //1通道开路检测端
		 
		 if(ch2_chexe){			 
			ch2_duan++; 
			 
		 }
		 
		 
		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line2); //清除LINE0上的中断标志位  
	
	OSIntExit(); 
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
OSIntEnter();
	
	 if(KEY0_PE3==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
		//printf("zhong duan 3\r\n");
		 
		  //1通道短路检测段
		  if(ch2_chexe){			 
			ch2_tong++; 	 
		 }
		 
		 
		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE0上的中断标志位  
	
	OSIntExit();  
}



//外部中断0服务程序 
void EXTI4_IRQHandler(void)
{
	OSIntEnter();
	
	 if(KEY0_PE4==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
	//	printf("zhong duan 4\r\n");
		 
		 
		 //1通道开路检测端
		 if(ch1_chexe){			 
			ch1_duan++; 
			 
		 }
		 

		 
	 }

	
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE0上的中断标志位  
	
	OSIntExit();
	
}
 


void EXTI9_5_IRQHandler(void)
{
	//delay_ms(10);//消抖
	OSIntEnter();
	
	if(EXTI_GetITStatus(EXTI_Line5)==1){
		
   if(KEY0_PE5==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
		//printf("zhong duan 5\r\n");
		 
		 //1通道短路检测段
		  if(ch1_chexe){			 
			ch1_tong++; 	 
		 }
		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE4上的中断标志位  
		
	}	
	
	
	
	
	if(EXTI_GetITStatus(EXTI_Line7)==1){
		
   if(KEY0_PE7==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
		//printf("zhong duan 7\r\n");
		 
		 	//3通道开路检测端
		 if(ch3_chexe){			 
			ch3_duan++; 
			 
		 }
		 

		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE4上的中断标志位  
		
	}		
	
	
if(EXTI_GetITStatus(EXTI_Line8)==1){
		
   if(KEY0_PE8==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
		//printf("zhong duan 8\r\n");
		 
		 //3通道短路检测段
		  if(ch3_chexe){			 
			ch3_tong++; 	 
		 }
		  
		 
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除LINE4上的中断标志位  
		
	}		
	
	
	
	

	if(EXTI_GetITStatus(EXTI_Line9)==1){
   if(KEY0_PE9==0){
	
		 //1通道开路检测端
		 if(ch6_chexe){			 
			ch6_duan++; 
			 
		 }
		 
		 
	 }
	EXTI_ClearITPendingBit(EXTI_Line9);  //清除LINE4上的中断标志位  
		
	}		

	

	
	
	OSIntExit();
}






 void EXTI15_10_IRQHandler(void)
{
	
	
 OSIntEnter();
	
	//delay_ms(10);//消抖
	
	if(EXTI_GetITStatus(EXTI_Line10)==1){
     
		delay_ms(20);
		
		if(KEY0_PD10==1){
		
		delay_ms(20);
		
  
		 if(KEY0_PD10==1){	
		 delay_ms(10);
		 
		if(KEY0_PD10==1){	
			
		if(tang_bool){
		tan_xue_boo=2;     //将探穴信号置为2 
		}		
	}
		
  }
	
	
		
	 }
		EXTI_ClearITPendingBit(EXTI_Line10);  //清除LINE4上的中断标志位  
	}		

	
	
	if(EXTI_GetITStatus(EXTI_Line11)==1){
   if(KEY0_PE11==0){

	
			//6通道短路检测段
		  if(ch6_chexe){			 
			ch6_tong++; 	 
		 } 
		 
		 
	 }
		EXTI_ClearITPendingBit(EXTI_Line11);  //清除LINE4上的中断标志位  
		
	}		

	
	
	
	
	
	
  if(EXTI_GetITStatus(EXTI_Line12)==1){


		if(KEY0_PE12==0){
			//	tan_xue_boo=2;  //将探穴信号置为2 
	//	printf("zhong duan 12\r\n");
			
				 //1通道开路检测端
		 if(ch5_chexe){			 
			ch5_duan++; 
			 
		 }
		 
			
			
		}
		EXTI_ClearITPendingBit(EXTI_Line12);  //清除LINE4上的中断标志位  
		
	}		


	
	if(EXTI_GetITStatus(EXTI_Line13)==1){
		
  if(KEY0_PE13==0){
	//	tan_xue_boo=2;  //将探穴信号置为2 
	//	printf("zhong duan 13\r\n");
		
			//4通道短路检测段
		  if(ch5_chexe){			 
			ch5_tong++; 	 
		 } 
		 
		
		
	}
		EXTI_ClearITPendingBit(EXTI_Line13);  //清除LINE4上的中断标志位  
		
	}		
	
	
	if(EXTI_GetITStatus(EXTI_Line14)==1){
   if(KEY0_PE14==0){
		//tan_xue_boo=2;  //将探穴信号置为2 
	//	printf("zhong duan 14\r\n");
		 
		 	 //1通道开路检测端
		 if(ch4_chexe){			 
			ch4_duan++; 
			 
		 }
		 

		 
		 
		 
  }
		EXTI_ClearITPendingBit(EXTI_Line14);  //清除LINE4上的中断标志位  
		
	}		


	
	if(EXTI_GetITStatus(EXTI_Line15)==1){
		
   if(KEY0_PE15==0){
	//	tan_xue_boo=2;  //将探穴信号置为2 
	//	printf("zhong duan 15\r\n");
		 
			//4通道短路检测段
		  if(ch4_chexe){			 
			ch4_tong++; 	 
		 } 
		 
		 
		 
	 }
		EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE4上的中断标志位  
		
	}		
	


	
	OSIntExit();
}



//第二版本短路开路检测程序
void check_ch1x6_dk(void)
{
	u16 ch1_unmb,ch2_unmb,ch3_unmb,ch4_unmb,ch5_unmb,ch6_unmb;
	
	
	
	//对1通道进行处理判断
	if(ch1_chexe){
		
		//发生短路情况
		if(ch1_duan>ch1_tong){
			
			ch1_unmb=(ch1_duan-ch1_tong);
			if(ch1_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch1_duan_kai=30;  //16位的数据存最大数据为65536
				
				ch1_duan=3;
				ch1_tong=3;
			
			
				if(ch1_duan>=60000){
					ch1_duan=0;
					ch1_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch1_duan<ch1_tong){
			
			ch1_unmb=(ch1_tong-ch1_duan);
			if(ch1_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch1_duan_kai=30;
				ch1_duan=3;
				ch1_tong=3;
				
				
				if(ch1_tong>=60000){
					ch1_duan=0;
					ch1_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch1_duan>=60000)&&(ch1_tong>=60000))
			
		{
		  ch1_duan=3;
			ch1_tong=3;
			
		}
	
	
	}
	
	
	
		//对1通道进行处理判断
	if(ch2_chexe){
		
		//发生短路情况
		if(ch2_duan>ch2_tong){
			
			ch2_unmb=(ch2_duan-ch2_tong);
			if(ch2_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch2_duan_kai=30;  //16位的数据存最大数据为65536
				ch2_duan=3;
				ch2_tong=3;
				
				if(ch2_duan>=60000){
					ch2_duan=0;
					ch2_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch2_duan<ch2_tong){
			
			ch2_unmb=(ch2_tong-ch2_duan);
			if(ch2_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch2_duan_kai=30;
				ch2_duan=3;
				ch2_tong=3;
				
				if(ch2_tong>=60000){
					ch2_duan=0;
					ch2_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch2_duan>=60000)&&(ch2_tong>=60000))
			
		{
		  ch2_duan=3;
			ch2_tong=3;
			
		}
	
	
	}
	
	
		//对1通道进行处理判断
	if(ch3_chexe){
		
		//发生短路情况
		if(ch3_duan>ch3_tong){
			
			ch3_unmb=(ch3_duan-ch3_tong);
			if(ch3_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch3_duan_kai=30;  //16位的数据存最大数据为65536
				ch3_duan=3;
				ch3_tong=3;
				
				
				if(ch3_duan>=60000){
					ch3_duan=0;
					ch3_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch3_duan<ch3_tong){
			
			ch3_unmb=(ch3_tong-ch3_duan);
			if(ch3_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch3_duan_kai=30;
				ch3_duan=3;
				ch3_tong=3;
				
				if(ch3_tong>=60000){
					ch3_duan=0;
					ch3_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch3_duan>=60000)&&(ch3_tong>=60000))
			
		{
		  ch3_duan=3;
			ch3_tong=3;
			
		}
	
	
	}
	
	
		//对1通道进行处理判断
	if(ch4_chexe){
		
		//发生短路情况
		if(ch4_duan>ch4_tong){
			
			ch4_unmb=(ch4_duan-ch4_tong);
			if(ch4_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch4_duan_kai=30;  //16位的数据存最大数据为65536
				ch4_duan=3;
				ch4_tong=3;
				
				
				if(ch4_duan>=60000){
					ch4_duan=0;
					ch4_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch4_duan<ch4_tong){
			
			ch4_unmb=(ch4_tong-ch4_duan);
			if(ch4_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch4_duan_kai=30;
				ch4_duan=3;
				ch4_tong=3;
				
				if(ch4_tong>=60000){
					ch4_duan=0;
					ch4_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch4_duan>=60000)&&(ch4_tong>=60000))
			
		{
		  ch4_duan=3;
			ch4_tong=3;
			
		}
	
	
	}
	
	
	
		//对1通道进行处理判断
	if(ch5_chexe){
		
		//发生短路情况
		if(ch5_duan>ch5_tong){
			
			ch5_unmb=(ch5_duan-ch5_tong);
			if(ch5_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch5_duan_kai=30;  //16位的数据存最大数据为65536
				ch5_duan=3;
				ch5_tong=3;
				
				
				if(ch5_duan>=60000){
					ch5_duan=0;
					ch5_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch5_duan<ch5_tong){
			
			ch5_unmb=(ch5_tong-ch5_duan);
			if(ch5_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能
				
				ch5_duan_kai=30;
				ch5_duan=3;
				ch5_tong=3;
				
				if(ch5_tong>=60000){
					ch5_duan=0;
					ch5_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch5_duan>=60000)&&(ch5_tong>=60000))
			
		{
		  ch5_duan=3;
			ch5_tong=3;
			
		}
	
	
	}
	
	
	
		//对1通道进行处理判断
	if(ch6_chexe){
		
		//发生短路情况
		if(ch6_duan>ch6_tong){
			
			ch6_unmb=(ch6_duan-ch6_tong);
			if(ch6_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能  第一版是 30  
				
				ch6_duan_kai=30;  //16位的数据存最大数据为65536
				
				ch6_duan=3;
				ch6_tong=3;
				
				if(ch6_duan>=60000){
					ch6_duan=0;
					ch6_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
		}
		//发生开路情况
			if(ch6_duan<ch6_tong){
			
			ch6_unmb=(ch6_tong-ch6_duan);
			if(ch6_unmb>=300){  //30代表启动检测灵敏度开关，当两边的中断此次相差大的话就启动提示功能 第一版是 30  
				
				ch6_duan_kai=30;
				ch6_duan=3;
				ch6_tong=3;
				
				if(ch6_tong>=60000){
					ch6_duan=0;
					ch6_tong=0; //这么做的目的是现在处于短路状态，那么下次进行还会判断为短路状态，
					
				}
				
				
			}
			
		}
		
		//没有发生短路，也没有发生开路，同时对比较数据进行清零
		if((ch6_duan>=60000)&&(ch6_tong>=60000))
			
		{
		  ch6_duan=3;
			ch6_tong=3;
			
		}
	
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
}







 
