 #include "adc.h"
 #include "delay.h"
 #include "usart.h"	
 #include "stm32f10x_adc.h"
 
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3							
extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //开机默认周期10ms  

u8 ch1_duan_kai=0;  //通道1短路开路变量
u8 ch2_duan_kai=0;  //通道2短路开路变量
u8 ch3_duan_kai=0;  //通道3短路开路变量
u8 ch4_duan_kai=0;  //通道4短路开路变量
u8 ch5_duan_kai=0;  //通道5短路开路变量
u8 ch6_duan_kai=0;  //通道6短路开路变量

u16 mang_1,mang_2;

extern u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	
	//PB1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//PC1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	
	
	
	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式  befor DISABLE      
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


/////////对第一通道进行短路开路扫描检测///////////////////
void Get_ch1(void)
{
	u16 adcx;


	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值  
	
	if(adcx>=400){
		ch1_chexe=2;
		
	}else ch1_chexe=0;




}



/////////对第2通道进行短路开路扫描检测///////////////////
void Get_ch2(void)
{
	u16 adcx;
	

	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值 
     

	if(adcx>=400){
		ch2_chexe=2;
		
	}else ch2_chexe=0;




}


/////////对第3通道进行短路开路扫描检测///////////////////
void Get_ch3(void)
{
	u16 adcx;
	 

	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值  
	

	if(adcx>=400){
		ch3_chexe=2;
		
	}else ch3_chexe=0;

	
	
}


/////////对第4通道进行短路开路扫描检测///////////////////
void Get_ch4(void)
{
	u16 adcx;

	 
	
	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值  
	
	if(adcx>=400){
		ch4_chexe=2;
		
	}else ch4_chexe=0;


	
}

/////////对第4通道进行短路开路扫描检测///////////////////
void Get_ch5(void)
{
	u16 adcx;
	 
	
	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值 
	
	
	if(adcx>=400){
		ch5_chexe=2;
		
	}else ch5_chexe=0;


		
}


/////////对第4通道进行短路开路扫描检测///////////////////
void Get_ch6(void)
{
	u16 adcx,adcx_1,sket_num;
	float temp,temp_1;
	float xi_numb_1;
	u8 y,h,j;
	 
	

 adcx=Get_Adc(ADC_Channel_9);  // MI_ADC 公共端通道值 
	
	
	if(adcx>=400){
		ch6_chexe=2;
		
	}else ch6_chexe=0;

	

	
}




















