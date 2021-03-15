 #include "adc.h"
 #include "delay.h"
 #include "usart.h"	
 #include "stm32f10x_adc.h"
 
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3							
extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //����Ĭ������10ms  

u8 ch1_duan_kai=0;  //ͨ��1��·��·����
u8 ch2_duan_kai=0;  //ͨ��2��·��·����
u8 ch3_duan_kai=0;  //ͨ��3��·��·����
u8 ch4_duan_kai=0;  //ͨ��4��·��·����
u8 ch5_duan_kai=0;  //ͨ��5��·��·����
u8 ch6_duan_kai=0;  //ͨ��6��·��·����

u16 mang_1,mang_2;

extern u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	
	//PB1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//PC1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	
	
	
	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת�������ڵ�ͨ��ģʽ  befor DISABLE      
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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


/////////�Ե�һͨ�����ж�·��·ɨ����///////////////////
void Get_ch1(void)
{
	u16 adcx;


	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ  
	
	if(adcx>=400){
		ch1_chexe=2;
		
	}else ch1_chexe=0;




}



/////////�Ե�2ͨ�����ж�·��·ɨ����///////////////////
void Get_ch2(void)
{
	u16 adcx;
	

	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ 
     

	if(adcx>=400){
		ch2_chexe=2;
		
	}else ch2_chexe=0;




}


/////////�Ե�3ͨ�����ж�·��·ɨ����///////////////////
void Get_ch3(void)
{
	u16 adcx;
	 

	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ  
	

	if(adcx>=400){
		ch3_chexe=2;
		
	}else ch3_chexe=0;

	
	
}


/////////�Ե�4ͨ�����ж�·��·ɨ����///////////////////
void Get_ch4(void)
{
	u16 adcx;

	 
	
	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ  
	
	if(adcx>=400){
		ch4_chexe=2;
		
	}else ch4_chexe=0;


	
}

/////////�Ե�4ͨ�����ж�·��·ɨ����///////////////////
void Get_ch5(void)
{
	u16 adcx;
	 
	
	adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ 
	
	
	if(adcx>=400){
		ch5_chexe=2;
		
	}else ch5_chexe=0;


		
}


/////////�Ե�4ͨ�����ж�·��·ɨ����///////////////////
void Get_ch6(void)
{
	u16 adcx,adcx_1,sket_num;
	float temp,temp_1;
	float xi_numb_1;
	u8 y,h,j;
	 
	

 adcx=Get_Adc(ADC_Channel_9);  // MI_ADC ������ͨ��ֵ 
	
	
	if(adcx>=400){
		ch6_chexe=2;
		
	}else ch6_chexe=0;

	

	
}




















