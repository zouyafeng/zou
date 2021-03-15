#include "led.h"
#include "delay.h"
#include "key.h"
#include "adc.h"
 #include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

extern u16 ch1_time,ch2_time,ch3_time,ch4_time,ch5_time,ch6_time;      //��ʱ������ı�����¼�Ǹ�ͨ��msֵ��ʹ���ÿһ��ͨ��ֵ��1ms�ڲ����ظ����
extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //����Ĭ������10ms  

u16 ch2_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch2_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch2_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch2_unm_E2_x1;  //ͨ��2��Ħ��E2������Ʊ���



u16 ch6_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch6_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch6_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch6_unm_E2_x1;  //ͨ��2��Ħ��E2������Ʊ���


u16 ch5_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch5_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch5_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch5_unm_E2_x1;  //ͨ��2��Ħ��E2������Ʊ���

u16 ch4_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch4_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch4_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch4_unm_E2_x1=0;  //ͨ��2��Ħ��E2������Ʊ���

u16 ch3_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch3_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch3_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch3_unm_E2_x1=0;  //ͨ��2��Ħ��E2������Ʊ���

u16 ch1_unm_E1_t=560; //ͨ��2��Ħ��E1 �Զ��仯���� 
u16 ch1_unm_E3_t=100; //ͨ��2��Ħ��E3 �Զ��仯���� 
u8  ch1_unm_E3_x1=0;  //ͨ��2��Ħ��E3������Ʊ���

u16 ch1_unm_E2_x1;  //ͨ��2��Ħ��E2������Ʊ���


u8 send_guzhang=0;  //���ͽӴ����������Ʊ�־




u8 clos_bis_unm=1;

//���ܲ�������Ʊ���
u16 ch1_shu_mi=0,ch2_shu_mi=0,ch3_shu_mi=0,ch4_shu_mi=0,ch5_shu_mi=0,ch6_shu_mi=0; 

u8  ch1_bool=0,ch2_bool=0,ch3_bool=0,ch4_bool=0,ch5_bool=0,ch6_bool=0;

u8 GZE_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x48,0x00,0x00};  //��·��·������ʾ����



u8 set_ch1_bo=1;  //����Ĭ�������������
u8 set_ch2_bo=1;  //����Ĭ�������������
u8 set_ch3_bo=1;  //����Ĭ�������������
u8 set_ch4_bo=1;  //����Ĭ�������������
u8 set_ch5_bo=1;  //����Ĭ�������������
u8 set_ch6_bo=1;  //����Ĭ�������������


u8 text_ch1=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·
u8 text_ch2=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·
u8 text_ch3=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·
u8 text_ch4=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·
u8 text_ch5=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·
u8 text_ch6=0;   //�������·��·Ƶ�� ��ʱ��Ϊû100��������һ�ζ�����·

extern u8 ch1_duan_kai;  //ͨ��1��·��·����
extern u8 ch2_duan_kai;  //ͨ��2��·��·����
extern u8 ch3_duan_kai;  //ͨ��3��·��·����
extern u8 ch4_duan_kai;  //ͨ��4��·��·����
extern u8 ch5_duan_kai;  //ͨ��5��·��·����
extern u8 ch6_duan_kai;  //ͨ��6��·��·����



int delay_long_lang=130; 

int delay_long_lang_1=260; 

int delay_long=148;  //��������ʱ�䶨�� ��λΪus��   ��ǰ��200��  135   148
int delay_long_1=60;  //��������ʱ�䶨�� ��λΪus��  ��ǰ��100    55     60


u16 sumi_x1,sumi_x2,sumi_x3,sumi_x4,sumi_x5,sumi_x6; //���ܲ��Ŀ��Ʊ��� �ߵ�ƽ ���״̬
u16 sumi_y1,sumi_y2,sumi_y3,sumi_y4,sumi_y5,sumi_y6; //���ܲ��Ŀ��Ʊ��� �͵�ƽ ���״̬

u16 qinchui_x1,qingchui_x1;  //�ᴷ������ߵ�ƽ
u16 qinchui_y1,qinchui_4T;   //�ᴷ������͵�ƽ


u16 sumi_4T;          //4�����ʱ����ֹͣһ��

bool ch_lang=FALSE;

extern  u8 tan_xue_boo ; //̽Ѩ��⵽���庯�� ����⵽ѨλʱΪ2�����Ѩλ��Ӧ��Ϊ0��ӦΪ���𶯵�·������Ѩλ�źţ�����ÿ����Ӧ�궼��Ϊ0 .

extern u8 system_start_EL;  //��·��·ֹͣ����������ʶ

 extern u8 bisuo_boo;  //��

extern u8 zhi_unm_date;





void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	

	//���ȹܽ�
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);		
	

 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7| 
	                              GPIO_Pin_8|GPIO_Pin_9;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	
 GPIO_SetBits(GPIOG,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7| 
	                              GPIO_Pin_8|GPIO_Pin_9);	
	
	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;				 //LED0-->PB.5 �˿�����  HT-2�� LED���ƹܽ� 
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //��©������   
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz  
// GPIO_Init(GPIOE, &GPIO_InitStructure);	
//	
// GPIO_SetBits(GPIOE,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	
	


 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);    //�������PB3��jiack�ڣ��ر�һ���������

	
	////ͨ��5��6 �������������  HT-3   ���ȹܽ� PD 12  13    14   15 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //MCU_O6-PD6  MCU_O5-PD7 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOD,GPIO_Pin_6|GPIO_Pin_7);						 //PB.5 �����
 GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
 
	////ͨ��2��6 �������������  HT-3
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9;			//MCU_O1-PB9  MCU_O2-PB8   MCU_O3-PB4  MCU_O4-PB3
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9);						 //PB.5 �����
	
	
	
 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
}

//��ǰ������������ܹ������ĳ��˼��缫������ó�ԭLED1����LED6�����÷�ʽֻ��4051�Ŀ��ƹܽ� 
void off_4051_int(void)   
{
	
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��	

	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				     //������LEEP��ָʾ�ܽ�     
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);	                   //�������Ŀ��ƹܽ�
	
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //4051����ʹ�ܹܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©������ 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);    //INT1 INT2 �ø߾͹ر���
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;				 //4051����ʹ�ܹܽ� A=PC10  B=PC11  C=PC12  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©������ 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);		
 GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);    // 4051��ѡ��ʹ�� A B  C  ������ƽ�   
 

	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;				     //ϵͳ�ź�ָʾ�� LED_G�Ŀ��ƹܽ�   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);	
 GPIO_SetBits(GPIOD,GPIO_Pin_8);	
 GPIO_ResetBits(GPIOD,GPIO_Pin_11);


 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 //��ƽת��ʹ�ܿ������ţ����óɿ�©���
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©������ 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);           //OE
	



	
}


//�򿪷��������Ƿ�������������
void on_leep(void)
{
	u8 i,j;
	
  if(bisuo_boo){   //�رպʹ򿪱������ܲ���
	
	if((ch1_duan_kai>=26)||(ch2_duan_kai>=26)||(ch3_duan_kai>=26)||(ch4_duan_kai>=26)||(ch5_duan_kai>=26)||(ch6_duan_kai>=26))
  {
	 system_start_EL=0;  //�رյ��������  
	
		send_guzhang=2;   //�򿪷��ͱ��ʶ
		
		GZE_LED[7]=0x00;
		
				for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,GZE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
		
  }
	
	//�رձ����ָ�������ָ��ֻ����һ�Σ���С��ʾ��ѹ��
	if(send_guzhang==2){
		
		if(system_start_EL==1){
			
			GZE_LED[7]=0x01;
			send_guzhang=0;   //�رշ��ͱ��ʶ
			
							for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,GZE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
			
			
		}
		
		
		
	}
	
	
	
	
	if(!system_start_EL) //��·��·��������ʾ
	{
	LEEP=1;
		
	}else{
		
	LEEP=0;
	}
	
	
}
	
	//LEEP=~LEEP;
	
	if((ch1_duan_kai>=26)||(ch2_duan_kai>=26)||(ch3_duan_kai>=26)||(ch4_duan_kai>=26)||(ch5_duan_kai>=26)||(ch6_duan_kai>=26)||(tan_xue_boo==2)||(zhi_unm_date>=2))
	{    //��������⵽���ڵ���6�ζ�·��·�������������ʾ��
		
  LEEP=1;

	tan_xue_boo=0;
	ch1_duan_kai=0;  //�����ʾ����־
	ch2_duan_kai=0;	
	ch3_duan_kai=0;  //�����ʾ����־
	ch4_duan_kai=0;	
  ch5_duan_kai=0;  //�����ʾ����־
	ch6_duan_kai=0;			
	zhi_unm_date=0;	
		
	}else{
	LEEP=0;
}
	
	
	
	
	if((tan_xue_boo==2)||(zhi_unm_date>=2))
	{    
		
 // LEEP=1;  //������������ʾ��

	tan_xue_boo=0;   //��λ̽Ѩ��־
	zhi_unm_date=0;	
		
	}else{
//	LEEP=0;
}
	






	
	
	
	
}





void off_NO1_4051(void){    //��ͨ��1���    
	
	
	
GPIO_SetBits(GPIOD,GPIO_Pin_0);
GPIO_ResetBits(GPIOD,GPIO_Pin_1);		
	
GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_12);
GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	
}


void close_4051(void){    //  
	
	
	
GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);    //INT1 INT2 �ø߾͹ر���
DC4051_C=1;DC4051_B=1;DC4051_A=1;
	
	
}


void texi_ch2(void){
	
	off_NO1_4051();
	
//GPIO_SetBits(GPIOD,GPIO_Pin_1);
//GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
//delay_us(1);
//GPIO_SetBits(GPIOD,GPIO_Pin_5|GPIO_Pin_3);
//GPIO_ResetBits(GPIOD,GPIO_Pin_4);	
//	
	
	MCU_O2=0;
	delay_us(delay_long);
	MCU_O2=1;
	close_4051();
	
}



//�������˵��  pinΪ����һƬ4051оƬ��ʹ�� 1Ϊ�򿪵�һ��Ƭ4051ѡ������2Ϊ�򿪵ڶ�Ƭ4051ѡ������
//xinΪ����һ��ѡ��˿ڽ�MIֵ�����X1~X8�Ķ˿� 
void star_4051(u8 pin,u8 xin)
{
	if(pin==1){ INH1=0; INH2=1;}
	if(pin==2){ INH1=1; INH2=0;}
	if(xin==0){DC4051_C=0;DC4051_B=0;DC4051_A=0;}  //��0ͨ��
	if(xin==1){DC4051_C=0;DC4051_B=0;DC4051_A=1;}  //��0ͨ��
	if(xin==2){DC4051_C=0;DC4051_B=1;DC4051_A=0;}  //��0ͨ��
	if(xin==3){DC4051_C=0;DC4051_B=1;DC4051_A=1;}  //��0ͨ��
	if(xin==4){DC4051_C=1;DC4051_B=0;DC4051_A=0;}  //��0ͨ��
	if(xin==5){DC4051_C=1;DC4051_B=0;DC4051_A=1;}  //��0ͨ��
	if(xin==6){DC4051_C=1;DC4051_B=1;DC4051_A=0;}  //��0ͨ��
	if(xin==7){DC4051_C=1;DC4051_B=1;DC4051_A=1;}  //��0ͨ��
}


//������һ��ͨ�����ʲô���� 
//������һ��ͨ������������źŲ��� 1Ϊ1ͨ����� 2Ϊ2ͨ����� ������������������
// h Ϊ1ʱ������� hΪ2ʱ������� 
void ch_X_set_ms(u8 ch, u8 h)
{
	
	
	if(ch==1){
	if(h==1){  //���������   
	star_4051(1,2);
	MCU_O1=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch1();
		
	}
  delay_us(delay_long_1);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
	star_4051(1,3); //��1Ƭ4051��2ͨ��    
	MCU_O1=0;
	delay_us(delay_long);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==3){  //���������   300ms���
	star_4051(1,2);
	MCU_O1=0;
	delay_us(delay_long_lang);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch1();
	}
		
  delay_us(delay_long_lang);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	if(h==4){  //���������
	star_4051(1,3); //��1Ƭ4051��2ͨ��    
	MCU_O1=0;
	delay_us(delay_long_lang_1);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
	
}
	

	if(ch==2){
	if(h==1){  //���������    
	star_4051(1,5);
	MCU_O2=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch2();
	}
	
  delay_us(delay_long_1);		
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
  star_4051(1,1);	 //��һ���ǣ�1��7���ڶ��棨1��1��
	MCU_O2=0;
	delay_us(delay_long);
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //���������    
	star_4051(1,5);
	MCU_O2=0;
	delay_us(delay_long_lang);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���		
	Get_ch2();
	}
	
  delay_us(delay_long_lang);		
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==4){  //���������
  star_4051(1,1);	 //��һ���ǣ�1��7���ڶ��棨1��1��
	MCU_O2=0;
	delay_us(delay_long_lang_1);
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
}
	
	if(ch==3){ 
	if(h==1){  //���������     
	star_4051(1,6);
	MCU_O3=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch3();
	}
		
  delay_us(delay_long_1);	
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
  star_4051(1,4);	 //��1Ƭ4051��2ͨ��    
	MCU_O3=0;
	delay_us(delay_long);
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //���������     
	star_4051(1,6);
	MCU_O3=0;
	delay_us(delay_long_lang);
	
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���		
	Get_ch3();
		
	}
  delay_us(delay_long_lang);	
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==4){  //���������
  star_4051(1,4);	 //��1Ƭ4051��2ͨ��    
	MCU_O3=0;
	delay_us(delay_long_lang_1);
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
}
	
	if(ch==4){
	if(h==1){  //���������    
	star_4051(2,2);
	MCU_O4=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���		
	Get_ch4();
	}
  delay_us(delay_long_1);	
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
	star_4051(2,3); //��1Ƭ4051��2ͨ��   
	MCU_O4=0;
	delay_us(delay_long);
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //���������    
	star_4051(2,2);
	MCU_O4=0;
	delay_us(delay_long_lang);
			
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch4();
	}
  delay_us(delay_long_lang);	
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==4){  //���������
	star_4051(2,3); //��1Ƭ4051��2ͨ��   
	MCU_O4=0;
	delay_us(delay_long_lang_1);
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
}
	



	if(ch==5){
	if(h==1){  //���������   
	star_4051(2,5);
	MCU_O5=0;
	delay_us(delay_long_1);
	
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch5();
		
	}
	
  delay_us(delay_long_1);	
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
	star_4051(2,1);	 //��һ���ǣ�2.7�� �ڶ����ǣ�2��1��
	MCU_O5=0;
	delay_us(delay_long);
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //���������   
	star_4051(2,5);
	MCU_O5=0;
	delay_us(delay_long_lang);
			
	if(clos_bis_unm){  //�رն�·��·�����Ʊ���	
	Get_ch5();
	}
  delay_us(delay_long_lang);	
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==4){  //���������
	star_4051(2,1);	 //��һ���ǣ�2.7�� �ڶ����ǣ�2��1��
	MCU_O5=0;
	delay_us(delay_long_lang_1);
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
}
	
	if(ch==6){
	if(h==1){  //���������
  star_4051(2,6);	
	MCU_O6=0;
	delay_us(delay_long_1);
		
		
	
	if(clos_bis_unm==1){  //�رն�·��·�����Ʊ���	
	Get_ch6();
	}
	
	delay_us(delay_long_1);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //���������
	star_4051(2,4);
	MCU_O6=0;
	delay_us(delay_long);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
	
 	if(h==3){  //���������
  star_4051(2,6);	
	MCU_O6=0;
	delay_us(delay_long_lang);
		
		

	if(clos_bis_unm==1){  //�رն�·��·�����Ʊ���	
	Get_ch6();
	
	}
	
	delay_us(delay_long_lang);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
	if(h==4){  //���������
	star_4051(2,4);
	MCU_O6=0;
	delay_us(delay_long_lang_1);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
}
	
	

}





/////////////ͨ��2 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch2_pin(void)
{
	if(set_ch2_bo==1)   //����Ϊ������
	{
	 if(ch2_time==2){
		ch_X_set_ms(2,3);  //2ͨ����������� 
	 }
	if(ch2_time>=ch2_unm) ch2_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch2_bo==2)
 { 
	   
	 
	 	 sumi_x1=(ch2_unm-8);
		 sumi_x2=(sumi_x1+ch2_unm);

     sumi_y1=(ch2_unm+2);
		 sumi_y2=(sumi_y1+ch2_unm);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3ͨ����������� 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch2_time>=sumi_y2){
			 ch2_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch2_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch2_shu_mi<40){
		
				 ch2_unm=ch2_unm;
	
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch2_shu_mi==40){
				
				 ch2_unm=(ch2_unm*4);
				
			 }
			 
			 if(ch2_shu_mi>=80){
				 ch2_shu_mi=0;
			//	 set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
			 set_ch2_sinc();	 
				 
			 }	 
		 }
 
	 
	 
	 
	 
	 
	 
//	   sumi_x1=(ch2_unm+22); //�ߵ�ƽ���״̬ʱ�� ��2������� 
//	   sumi_x2=((ch2_unm*2)+42);
//	   sumi_x3=((ch2_unm*3)+62); 
//     
//	 
//	   sumi_y1=(ch2_unm+32); //�͵�ƽ���״̬ʱ�� ��2�������ʱ���λ
//	   sumi_y2=(sumi_x2+10);
//	   sumi_y3=(sumi_x3+10); 
//     
//	   sumi_4T=((ch2_unm*7)+80);
//	 
//     if((ch2_time==2)||(ch2_time==sumi_x1)||(ch2_time==sumi_x2) ||(ch2_time==sumi_x3)  )	 
//		 {
//			 ch_X_set_ms(2,1);  //2ͨ����������� 
//			 
//		 }
//	   if((ch2_time==12)||(ch2_time==sumi_y1)||(ch2_time==sumi_y2) ||(ch2_time==sumi_y3)    )
//		{
//			 
//			 ch_X_set_ms(2,2);  //2ͨ����������� 	 
//			 
//		 }
//		 
//		if(ch2_time>=sumi_4T) ch2_time=0;  //�����������T��ֵ   
//		 
		 
		 
		 		
	}
	
	if(set_ch2_bo==3)    //�ᴷ�����
	{
		
		
		sumi_x1=(ch2_unm-8);
		 sumi_x2=(sumi_x1+ch2_unm);

     sumi_y1=(ch2_unm+2);
		 sumi_y2=(sumi_y1+ch2_unm);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3ͨ����������� 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch2_time>=sumi_y2){
			 ch2_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			// set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
	 
			 set_ch2_sinc();
			 
			 }	
		
		
		
		
		
//		qinchui_x1=2; //��һ������ᴷ��������
//		qinchui_y1=(ch2_unm+10+2); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch2_unm*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //�����������T��ֵ  
//		
		
		
		
	}
	
	if(set_ch2_bo==4)  //����Ϊ��Ħ��E1�������  �����������Ϊ0.01��0.56s�仯���ᴷ��  
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch2_unm_E1_t-8);
		 sumi_x2=(sumi_x1+ch2_unm_E1_t);

     sumi_y1=(ch2_unm_E1_t+2);
		 sumi_y2=(sumi_y1+ch2_unm_E1_t);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3ͨ����������� 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch2_time>=sumi_y2){
			 ch2_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			// set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
	 
			 }	
		
		
		
//		qinchui_x1=2; //��һ������ᴷ��������
//		qinchui_y1=(ch2_unm_E1_t+10+2); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch2_unm_E1_t*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //�����������T��ֵ  
//		
		
		
	}
	
	if(set_ch2_bo==5) //��Ħ��E2���������
	{
				if(ch2_unm_E2_x1<=1200){
				 if(ch2_time==2){
				 ch_X_set_ms(2,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch2_unm_E2_x1>1200){
					
					if(ch2_time==2){
				 ch_X_set_ms(2,4);  //2ͨ����������� 
				 
			 }}
				
			ch2_unm_E2_x1++;
			if(ch2_unm_E2_x1>=2401) ch2_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
			if(ch2_time>=20) ch2_time=0;  //�����������T��ֵ   

	}
	
	if(set_ch2_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		 sumi_x1=(ch2_unm_E3_t-8);
		 sumi_x2=(sumi_x1+ch2_unm_E3_t);

     sumi_y1=(ch2_unm_E3_t+2);
		 sumi_y2=(sumi_y1+ch2_unm_E3_t);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3ͨ����������� 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch2_time>=sumi_y2){
			 ch2_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch2_unm_E3_x1++; //�����������һ������Ҳһ������Ϊһ��
	 
			 }	
		 
		if(ch2_unm_E3_x1>=5) { 
		ch2_unm_E3_t=(ch2_unm_E3_t-10);
    if(	ch2_unm_E3_t<=10){
    ch2_unm_E3_t=100;
		}			

		ch2_unm_E3_x1=0;   }
		
		
		
		
		
		
		
		
		
		
		
//	  qinchui_x1=2; //��һ������ᴷ��������
//		qinchui_y1=(ch2_unm_E3_t+10+2); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch2_unm_E3_t*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2ͨ����������� 	
//		ch2_unm_E3_x1++; //�����������һ������Ҳһ������Ϊһ��
//    			
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //�����������T��ֵ  
//		if(ch2_unm_E3_x1>=10) { 
//		ch2_unm_E3_t=(ch2_unm_E3_t-10);
//    if(	ch2_unm_E3_t<=10){
//    ch2_unm_E3_t=100;
//		}			

//		ch2_unm_E3_x1=0;   }
//		
		
		
		
		
		
		
	}
	
	
	
}



/////////////ͨ��6 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch6_pin(void)
{
	if(set_ch6_bo==1)   //����Ϊ������
	{
	 if(ch6_time==7){
		ch_X_set_ms(6,3);  //2ͨ����������� 
	 }
	if(ch6_time>=ch6_unm) ch6_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch6_bo==2)
 { 
	 
	 	 sumi_x1=(ch6_unm-3);
		 sumi_x2=(sumi_x1+ch6_unm);

     sumi_y1=(ch6_unm+7);
		 sumi_y2=(sumi_y1+ch6_unm);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3ͨ����������� 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch6_time>=sumi_y2){
			 ch6_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch6_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch6_shu_mi<40){
			
				 ch6_unm=ch6_unm;		
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch6_shu_mi==40){
				
				 ch6_unm=(ch6_unm*4);
				
			 }
			 
			 if(ch6_shu_mi>=80){
				 ch6_shu_mi=0;
				 //set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				 set_ch6_sinc();
				 
			 }	 
		 }
 
	 
	 
	 
	 
//	   sumi_x1=(ch6_unm+26); //�ߵ�ƽ���״̬ʱ�� ��2������� 
//	   sumi_x2=((ch6_unm*2)+46);
//	   sumi_x3=((ch6_unm*3)+66); 
//     
//	 
//	   sumi_y1=(ch6_unm+36); //�͵�ƽ���״̬ʱ�� ��2�������ʱ���λ
//	   sumi_y2=(sumi_x2+10);
//	   sumi_y3=(sumi_x3+10); 
//     
//	   sumi_4T=((ch6_unm*7)+80);
//	 
//     if((ch6_time==6)||(ch6_time==sumi_x1)||(ch6_time==sumi_x2) ||(ch6_time==sumi_x3)  )	 
//		 {
//			 ch_X_set_ms(6,1);  //2ͨ����������� 
//			 
//		 }
//	   if((ch6_time==16)||(ch6_time==sumi_y1)||(ch6_time==sumi_y2) ||(ch6_time==sumi_y3)    )
//		{
//			 
//			 ch_X_set_ms(6,2);  //2ͨ����������� 	 
//			 
//		 }
//		 
//			 if(ch6_time>=sumi_4T) ch6_time=0;  //�����������T��ֵ   
//		




		 
	}
	
	if(set_ch6_bo==3)    //�ᴷ�����
	{
		
		 sumi_x1=(ch6_unm-3);
		 sumi_x2=(sumi_x1+ch6_unm);

     sumi_y1=(ch6_unm+7);
		 sumi_y2=(sumi_y1+ch6_unm);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3ͨ����������� 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch6_time>=sumi_y2){
			 ch6_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		   //set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
			set_ch6_sinc();	

			 
			 }	
		
		
		
//		qinchui_x1=6; //��һ������ᴷ��������
//		qinchui_y1=(ch6_unm+10+6); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch6_unm*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //�����������T��ֵ  
//		
		
		
		
		
		
	}
	
	if(set_ch6_bo==4)  //����Ϊ��Ħ��E1�������  �仯�����ȷ�����0.01��50���������ٷ�����0.01��50��������
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch6_unm_E1_t-3);
		 sumi_x2=(sumi_x1+ch6_unm_E1_t);

     sumi_y1=(ch6_unm_E1_t+7);
		 sumi_y2=(sumi_y1+ch6_unm_E1_t);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3ͨ����������� 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch6_time>=sumi_y2){
			 ch6_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		  // set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				 
			 }	
		
		
		
		
		
		
//		qinchui_x1=6; //��һ������ᴷ��������
//		qinchui_y1=(ch6_unm_E1_t+10+6); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch6_unm_E1_t*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //�����������T��ֵ  
//		
		
		
	}
	
	if(set_ch6_bo==5) //��Ħ��E2���������
	{
				if(ch6_unm_E2_x1<=1200){
				 if(ch6_time==7){
				 ch_X_set_ms(6,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch6_unm_E2_x1>1200){
					
					if(ch6_time==7){
				 ch_X_set_ms(6,4);  //2ͨ����������� 
				 
			 }}
				
			ch6_unm_E2_x1++;
			if(ch6_unm_E2_x1>=2401) ch6_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
			if(ch6_time>=20) ch6_time=0;  //�����������T��ֵ   

	}
	
	if(set_ch6_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		
		 sumi_x1=(ch6_unm_E3_t-3);
		 sumi_x2=(sumi_x1+ch6_unm_E3_t);

     sumi_y1=(ch6_unm_E3_t+7);
		 sumi_y2=(sumi_y1+ch6_unm_E3_t);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3ͨ����������� 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch6_time>=sumi_y2){
			 ch6_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
       ch6_unm_E3_x1++; //�����������һ������Ҳһ������Ϊһ��
				 
			 }	
		
		if(ch6_unm_E3_x1>=5) { 
		ch6_unm_E3_t=(ch6_unm_E3_t-10);
    if(	ch6_unm_E3_t<=10){
    ch6_unm_E3_t=100;
		}			

		ch6_unm_E3_x1=0;   }
		
		
		
		
		
		
//	  qinchui_x1=6; //��һ������ᴷ��������
//		qinchui_y1=(ch6_unm_E3_t+10+6); //��һ������ᴷ���ĸ���
//		qinchui_4T=((ch6_unm_E3_t*2)+20);//�ᴷ����������Ϊ�����������ڼ�����������������ʱ��
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2ͨ����������� 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2ͨ����������� 	
//		ch6_unm_E3_x1++; //�����������һ������Ҳһ������Ϊһ��
//    			
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //�����������T��ֵ  
//		if(ch6_unm_E3_x1>=10) { 
//		ch6_unm_E3_t=(ch6_unm_E3_t-10);
//    if(	ch6_unm_E3_t<=10){
//    ch6_unm_E3_t=100;
//		}			

//		ch6_unm_E3_x1=0;   }
//		
		
		
		
		
		
	}
	
	
	
}

/////////////ͨ��6 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch5_pin(void)
{
	if(set_ch5_bo==1)   //����Ϊ������
	{
	 if(ch5_time==6){
		ch_X_set_ms(5,3);  //6ͨ����������� 
	 }
	if(ch5_time>=ch5_unm) ch5_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch5_bo==2)
 { 
	 
	 	 sumi_x1=(ch5_unm-4);
		 sumi_x2=(sumi_x1+ch5_unm);

     sumi_y1=(ch5_unm+6);
		 sumi_y2=(sumi_y1+ch5_unm);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3ͨ����������� 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch5_time>=sumi_y2){
			 ch5_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch5_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch5_shu_mi<40){
				 ch5_unm=ch5_unm;
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch5_shu_mi==40){
				
				 ch5_unm=(ch5_unm*4);
				
			 }
			 
			 if(ch5_shu_mi>=80){
				 ch5_shu_mi=0;
				// set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				 set_ch5_sinc();		 
			 }	 
		 }
 
	 
		 
		 
		 
		 
	}
	
	if(set_ch5_bo==3)    //�ᴷ�����
	{
		
		
		 sumi_x1=(ch5_unm-4);
		 sumi_x2=(sumi_x1+ch5_unm);

     sumi_y1=(ch5_unm+6);
		 sumi_y2=(sumi_y1+ch5_unm);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3ͨ����������� 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3ͨ����������� 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		   //set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				set_ch5_sinc();	

				
			 }	 


		

		
		
		
	}
	
	if(set_ch5_bo==4)  //����Ϊ��Ħ��E1�������  �仯�����ȷ�����0.01��50���������ٷ�����0.01��50��������
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch5_unm_E1_t-4);
		 sumi_x2=(sumi_x1+ch5_unm_E1_t);

     sumi_y1=(ch5_unm_E1_t+6);
		 sumi_y2=(sumi_y1+ch5_unm_E1_t);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3ͨ����������� 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3ͨ����������� 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
	//	   set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				 		 
			 }	 
		
	
		
	
		
		
		
	}
	
	if(set_ch5_bo==5) //��Ħ��E2���������
	{
				if(ch5_unm_E2_x1<=1200){
				 if(ch5_time==6){
				 ch_X_set_ms(5,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch5_unm_E2_x1>1200){
					
					if(ch5_time==6){
				 ch_X_set_ms(5,4);  //2ͨ����������� 
				 
			 }}
				
			ch5_unm_E2_x1++;
			if(ch5_unm_E2_x1>=2401) ch5_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
			if(ch5_time>=20) ch5_time=0;  //�����������T��ֵ   

	}
	
	if(set_ch5_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		
		
		 sumi_x1=(ch5_unm_E3_t-4);
		 sumi_x2=(sumi_x1+ch5_unm_E3_t);

     sumi_y1=(ch5_unm_E3_t+6);
		 sumi_y2=(sumi_y1+ch5_unm_E3_t);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3ͨ����������� 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3ͨ����������� 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
	     ch5_unm_E3_x1++; //�����������һ������Ҳһ������Ϊһ��
				 		 
			 }	 
		
		if(ch5_unm_E3_x1>=5) { 
		ch5_unm_E3_t=(ch5_unm_E3_t-10);
    if(	ch5_unm_E3_t<=10){
    ch5_unm_E3_t=100;
		}			

		ch5_unm_E3_x1=0;   }
		
	
		
	}
	
	
	
}


/////////////ͨ��4 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch4_pin(void)
{
	if(set_ch4_bo==1)   //����Ϊ������
	{
	 if(ch4_time==5){
		ch_X_set_ms(4,3);  //6ͨ����������� 
	 }
	if(ch4_time>=ch4_unm) ch4_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch4_bo==2)
 { 
	   
	 	 sumi_x1=(ch4_unm-5);
		 sumi_x2=(sumi_x1+ch4_unm);

     sumi_y1=(ch4_unm+5);
		 sumi_y2=(sumi_y1+ch4_unm);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2ͨ����������� 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch4_time>=sumi_y2){
			 ch4_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch4_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch4_shu_mi<40){
				if(ch4_bool){
				 ch4_unm=ch4_unm;

				}			
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch4_shu_mi==40){
				
				 ch4_unm=(ch4_unm*4);
				
			 }
			 
			 if(ch4_shu_mi>=80){
				 ch4_shu_mi=0;
				 //set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				 set_ch4_sinc();	
				 
			 }	 
		 }
 


		 
	}
	
	if(set_ch4_bo==3)    //�ᴷ�����
	{
		
		
		 sumi_x1=(ch4_unm-5);
		 sumi_x2=(sumi_x1+ch4_unm);

     sumi_y1=(ch4_unm+5);
		 sumi_y2=(sumi_y1+ch4_unm);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2ͨ����������� 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2ͨ����������� 
			 
		 }
		
 	if(ch4_time>=sumi_y2){
			ch4_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		 // set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		
		 set_ch4_sinc();
		
		
		}
		


	}
	
	if(set_ch4_bo==4)  //����Ϊ��Ħ��E1�������  �仯�����ȷ�����0.01��50���������ٷ�����0.01��50��������
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch4_unm_E1_t-5);
		 sumi_x2=(sumi_x1+ch4_unm_E1_t);

     sumi_y1=(ch4_unm_E1_t+5);
		 sumi_y2=(sumi_y1+ch4_unm_E1_t);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2ͨ����������� 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2ͨ����������� 
			 
		 }
		
		
		if(ch4_time>=sumi_y2){
			ch4_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
	//	  set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		
		}
		


		
		
	}
	
	if(set_ch4_bo==5) //��Ħ��E2���������
	{
				if(ch4_unm_E2_x1<=1200){
				 if(ch4_time==5){
				 ch_X_set_ms(4,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch4_unm_E2_x1>1200){
					
					if(ch4_time==5){
				 ch_X_set_ms(4,4);  //2ͨ����������� 
				 
			 }}
				
			ch4_unm_E2_x1++;
			if(ch4_unm_E2_x1>=2401) ch4_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
			if(ch4_time>=20) ch4_time=0;  //�����������T��ֵ   

	}
	
  if(set_ch4_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		
		
		 sumi_x1=(ch4_unm_E3_t-5);
		 sumi_x2=(sumi_x1+ch4_unm_E3_t);

     sumi_y1=(ch4_unm_E3_t+5);
		 sumi_y2=(sumi_y1+ch4_unm_E3_t);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2ͨ����������� 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2ͨ����������� 
			 
		 }
		
		
	if(ch4_time>=sumi_y2){
		ch4_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		ch4_unm_E3_x1++;
				
			
		}
		if(ch4_unm_E3_x1>=5) { 
			
		ch4_unm_E3_t=(ch4_unm_E3_t-10);
			
    if(	ch4_unm_E3_t<=10){
    ch4_unm_E3_t=100;
		}			
		
		ch4_unm_E3_x1=0;   }
		
		
		
		

		
		
	}
	
	
	
}


/////////////ͨ��3 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch3_pin(void)
{
	if(set_ch3_bo==1)   //����Ϊ������
	{
	 if(ch3_time==4){
		ch_X_set_ms(3,3);  //6ͨ����������� 
	 }
	if(ch3_time>=ch3_unm) ch3_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch3_bo==2)
 { 
	 

		 
		 sumi_x1=(ch3_unm-6);
		 sumi_x2=(sumi_x1+ch3_unm);

     sumi_y1=(ch3_unm+4);
		 sumi_y2=(sumi_y1+ch3_unm);

		 
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3ͨ����������� 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
	    ch_X_set_ms(3,2);  //3ͨ����������� 
			 
		 }
		
//��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch3_time>=sumi_y2){
			 ch3_time=2;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch3_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch3_shu_mi<40){
				if(ch3_bool){
				 ch3_unm=ch3_unm;

				}			
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch3_shu_mi==40){
				
				 ch3_unm=(ch3_unm*4);
				
			 }
			 
			 if(ch3_shu_mi>=80){
				 ch3_shu_mi=0;
				// set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				  set_ch3_sinc();
				 
			 }
			 
			 
			 
		 }
		 
		 
		 
		 
		 
		 
		 
	}
	
	if(set_ch3_bo==3)    //�ᴷ�����
	{
		

		
	   sumi_x1=(ch3_unm-6);
		 sumi_x2=(sumi_x1+ch3_unm);

     sumi_y1=(ch3_unm+4);
		 sumi_y2=(sumi_y1+ch3_unm);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3ͨ����������� 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2ͨ����������� 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		//  set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		set_ch3_sinc();
			
			
		}
		
		
		
		
		
	}
	
	if(set_ch3_bo==4)  //����Ϊ��Ħ��E1�������  �仯�����ȷ�����0.01��50���������ٷ�����0.01��50��������
	{
		
		//ch2_unm_E1_t
		

		
		 sumi_x1=(ch3_unm_E1_t-6);
		 sumi_x2=(sumi_x1+ch3_unm_E1_t);

     sumi_y1=(ch3_unm_E1_t+4);
		 sumi_y2=(sumi_y1+ch3_unm_E1_t);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3ͨ����������� 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2ͨ����������� 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		//  set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		
		}
		
		
		
		
	}
	
	if(set_ch3_bo==5) //��Ħ��E2���������
	{
		    ch3_unm_E2_x1++;
	     if(ch3_unm_E2_x1<=1200){
				 if(ch3_time==4){
				 ch_X_set_ms(3,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			  }
		    }
	       if(ch3_unm_E2_x1>1200){
					if(ch3_time==4){
				 ch_X_set_ms(3,4);  //2ͨ����������� 
				 
			 }
			}
				
			
	   if(ch3_unm_E2_x1>=2401) ch3_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
	   if(ch3_time>=20) ch3_time=0;  //�����������T��ֵ   

	}
	
	if(set_ch3_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		
		 sumi_x1=(ch3_unm_E3_t-6);
		 sumi_x2=(sumi_x1+ch3_unm_E3_t);

     sumi_y1=(ch3_unm_E3_t+4);
		 sumi_y2=(sumi_y1+ch3_unm_E3_t);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3ͨ����������� 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2ͨ����������� 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		ch3_unm_E3_x1++;
				
			
		}
		
		
		if(ch3_unm_E3_x1>=5) { 
			
		ch3_unm_E3_t=(ch3_unm_E3_t-10);
			
    if(	ch3_unm_E3_t<=10){
    ch3_unm_E3_t=100;
		}			

		ch3_unm_E3_x1=0;   }
		
		

		
		
		
	}
	
	
	
}


/////////////ͨ��1 ������κ��� /////////////////////
///set_boΪ���õ�ʲô����1��������  2�����ܲ�    3:�ᴷ��   4:��Ħ��E1  5:��Ħ��E2   6:��Ħ��E3  

void layout_ch1_pin(void)
{
	if(set_ch1_bo==1)   //����Ϊ������
	{
	 if(ch1_time==3){
		ch_X_set_ms(1,3);  //1ͨ����������� 
	 }
	if(ch1_time>=ch1_unm) ch1_time=0;  //�����������T��ֵ   
	 	 
	}
	
 //����Ϊ���ܲ�   ������4���ܲ�   һ���ߵ�ƽ10ms���͵�����10ms  ����T  �ٷ�4��    ��4T��ʱ����ŷ��ܲ�4��  һ��T   
	if(set_ch1_bo==2)
 { 


     sumi_x1=(ch1_unm-7);
		 sumi_x2=(sumi_x1+ch1_unm);

     sumi_y1=(ch1_unm+3);
		 sumi_y2=(sumi_y1+ch1_unm);

		 
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2ͨ����������� 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2ͨ����������� 	 
			 
		 }
		
//		 //��ͨ��1���������������������������������ݽ��м�һ��������������������ܲ������貨���ĸ�����
		 if(ch1_time>=sumi_y2){
			 ch1_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
			 ch1_shu_mi++;
			 //���ڵ���ʵ���������
			 if(ch1_shu_mi<40){
				if(ch1_bool){
				 ch1_unm=ch1_unm;

				}
				
				// ch1_bool=0;
				
				
			 }
			 //���ڵ���ʵ�����ڵĵ�4����
			  if(ch1_shu_mi==40){
				
				//if(!ch1_bool){
				 ch1_unm=(ch1_unm*4);
				//	ch1_bool=2;
				//}
				
			 }
			 
			 if(ch1_shu_mi>=80){
				 ch1_shu_mi=0;
				// set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				set_ch1_sinc(); 
				 
			 }
			 
			 
			 
		 }
		 
		 
		 
		 
		 
		 
		 		
	}
	
	if(set_ch1_bo==3)    //�ᴷ�����
	{

		
		 sumi_x1=(ch1_unm-7);
		 sumi_x2=(sumi_x1+ch1_unm);

     sumi_y1=(ch1_unm+3);
		 sumi_y2=(sumi_y1+ch1_unm);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2ͨ����������� 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2ͨ����������� 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
			ch1_time=4;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
	//	  set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		set_ch1_sinc(); 
		}
		
		
		
		
	}
	
	if(set_ch1_bo==4)  //����Ϊ��Ħ��E1�������  ������0.56s��0.02s�����ڱ仯���ᴷ����ɣ�
	{
		
		//ch2_unm_E1_t

		
		 sumi_x1=(ch1_unm_E1_t-7);
		 sumi_x2=(sumi_x1+ch1_unm_E1_t);

     sumi_y1=(ch1_unm_E1_t+3);
		 sumi_y2=(sumi_y1+ch1_unm_E1_t);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2ͨ����������� 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2ͨ����������� 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
			ch1_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		 // set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
		
		}
		
		
		
		
		
		
		
	}
	
	if(set_ch1_bo==5) //��Ħ��E2���������
	{
				if(ch1_unm_E2_x1<=1200){
				 if(ch1_time==3){
				 ch_X_set_ms(1,3);  //2ͨ����������� 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch1_unm_E2_x1>1200){
					
					if(ch1_time==3){
				 ch_X_set_ms(1,4);  //2ͨ����������� 
				 
			 }}
				
			ch1_unm_E2_x1++;
			if(ch1_unm_E2_x1>=2401) ch1_unm_E2_x1=0;  //����������������Ʊ�����Ϊ0��
			if(ch1_time>=20) ch1_time=0;  //�����������T��ֵ   

	}
	
	if(set_ch1_bo==6)   //��Ħ��E3�����ڴ�0.1��С��0.01 ���ᴷ��ÿ10���������ں�������С0.01s
	{    
		  //ch2_unm_E3_t
		
		 sumi_x1=(ch1_unm_E3_t-7);
		 sumi_x2=(sumi_x1+ch1_unm_E3_t);

     sumi_y1=(ch1_unm_E3_t+3);
		 sumi_y2=(sumi_y1+ch1_unm_E3_t);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2ͨ����������� 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2ͨ����������� 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
		
		ch1_unm_E3_x1++;
		ch1_time=0;    //��ʼ�������ݾ����´δ�ʲôʱ��ʼ���������ʱ��
		
		}
		
		if(ch1_unm_E3_x1>=5) { 
			
		ch1_unm_E3_t=(ch1_unm_E3_t-10);
			
    if(	ch1_unm_E3_t<=10){
    ch1_unm_E3_t=100;
		}			

		ch1_unm_E3_x1=0;   }
		
		
		
		

		
		
		
	}
	
	
	
}
















//��4Gģ��ػ�
 void OFF_4G()
	 {
	 MOD_4G=0;
	 delay_ms(200);
	 MOD_4G=1;
	 
 }
 
 //��4Gģ��ػ�
 void ON_4G(){
	 MOD_4G=0;
	 delay_ms(200);
	 MOD_4G=1;
	 
 }
 