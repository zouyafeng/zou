#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "includes.h"
#include "usart3.h"
#include "BC26.h"
#include "rtc.h" 
#include "ds18b20.h" 
#include "timer.h"
#include "key.h"
#include "adc.h"
#include "exti.h"
#include "24cxx.h"
#include "w25qxx.h"	


////////////////////////�汾�����/////////////////////////////////////
/////////////////////VS 1.00.12.J0////////////////////////////////////////////////


u8 bisuo_boo=1;     //�����رյ���0��������1    �رպʹ򿪱����������
u8 jh_boot=0;
u8 zd_che_net=0;
u8 time_t=0;

u16 feng_time=0; //��ʱһ���ӱ���  ʵ����200ms��ʱ�� ���Զ�ʱ1����Ϊ300
u16 elect_time=36; //��ʱ������Ļ�ϵ�������ʾͼ��״̬

//�԰�ĦE1����û�೤ʱ��ı�һ�ν��ж��壬�����趨Ϊ1.2��ı�һ�Σ������Ϳ����ڸı�һ�����ڵ�ʱ�򿴵�һ��0.56S�Ĳ���
u8 ch1_time_s=0,ch2_time_s=0,ch3_time_s=0,ch4_time_s=0,ch5_time_s=0,ch6_time_s=0;


extern u8 clos_bis_unm;//��ӳ���̽Ѩ�����رձ�����������  


extern u8 set_ch1_bo;  //����Ĭ�������������
extern u8 set_ch2_bo;  //����Ĭ�������������
extern u8 set_ch3_bo;  //����Ĭ�������������
extern u8 set_ch4_bo;  //����Ĭ�������������
extern u8 set_ch5_bo;  //����Ĭ�������������
extern u8 set_ch6_bo;  //����Ĭ�������������

extern u16 ch2_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 
extern u16 ch6_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 
extern u16 ch5_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 
extern u16 ch4_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 
extern u16 ch3_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 
extern u16 ch1_unm_E1_t ; //ͨ��2��Ħ��E1 �Զ��仯���� 




u8 zh_send_A2=0;
extern u8  seve_biaoding;

u16 work_REST=0;
u16 LED_PWOR=150; //����Ƶ����ȳ�ʼ����ʱ��50%������

u8 wendu[9]={0xA5,0x5A,0x05,0x82,0x00,0x33,0x00,0x00};  //�����¶ȵı�������
u8 hour_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x22,0x00,0x00}; //����Сʱ�ı�������
u8 feng_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x24,0x00,0x00}; //���ͷ��ӵı�������
u8 miao_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x26,0x00,0x00}; //�������ӵı�������

u8 xian_buf[6]={0xA5,0x5A,0x03,0x80,0x01,0x20};
extern u16 ch1_time,ch2_time,ch3_time,ch4_time,ch5_time,ch6_time;      //��ʱ������ı�����¼�Ǹ�ͨ��msֵ��ʹ���ÿһ��ͨ��ֵ��1ms�ڲ����ظ����

extern u8 LED1_EN, LED2_EN, LED3_EN, LED4_EN;

u8 time_num_low=1;
u8 jb_time_num=1;
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define TASK1_TASK_PRIO		4
//�����ջ��С	
#define TASK1_STK_SIZE 		2048
//������ƿ�
OS_TCB Task1_TaskTCB;
//�����ջ	
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//�������ȼ�
#define TASK2_TASK_PRIO		5
//�����ջ��С	
#define TASK2_STK_SIZE 		128
//������ƿ�
OS_TCB Task2_TaskTCB;
//�����ջ	
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
//������
void task2_task(void *p_arg);

extern u8 send_biaodin_date[] ;


OS_TMR 	tmr1;		//��ʱ��1
OS_TMR	tmr2;		//��ʱ��2
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����
u8 system_int=0;
extern u8 send_zhouqi_date[]; //�����ϴ���������
extern u8 zhouqi_bool;
extern  u8 xin_tiao_time[];  //���������ϴ�����
extern u8 sever_time ;   //�Ƿ��յ�ʱ����� 

short temperature;

extern u8 bool_SHU ;

u16 num_set=0;

u8 system_start=0;
u8 system_start_EL=1;  //����0�������������ܣ� ����1ʱ�رձ�������

extern u16 mang_1,mang_2;


//Ҫд�뵽W25Q64���ַ�������
const u8 TEXT_Buffer[]={"FFFFFFFFFFFFSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)


//������
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u8 rtc_num=0;
	u8 i=0,y=0,i_1,i_2,i_3;
	int k;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE; 
	
  delay_init(); 
	//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	off_4051_int();
	
	uart_init(115200); 	//���ڳ�ʼ��
	usart3_init(115200);
	usart2_init(115200);
  LED_Init();

	//LCD_Init();			//LCD��ʼ��	
	//read_apn_power(); //��ȡ�ϴδ洢��APN ���� ����  
	//BC20_init();
	//clean_rebuff1();
	//rtc_num=RTC_Init();	  			//RTC��ʼ��
	
	TIM3_Int_Init(18,3600);    // ��ʱ������1ms����һ���жϣ���������ͨ��1��ͨ��6�й��ɵ����������Ϣ   
	
	KEY_Init();
	

	
	EXTIX_Init(); //�����ⲿ̽Ѩ���� 
	Adc_Init();   //����ADC��鿪·��·���ܳ�ʼ������
	
	W25QXX_Init();			//W25QXX��ʼ��
	
	if(W25QXX_ReadID()==W25Q80)
	{
		printf("ϵͳ��ʼ��AT24C20:�ɹ�\r\n");
	}else{
		
		printf("ϵͳ��ʼ��AT24C20:ʧ��\r\n");
		
	}
	
	
	
//	W25QXX_Write((u8*)TEXT_Buffer,100,SIZE);			//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
//	delay_ms(100);
//	W25QXX_Read(datatemp,100,SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
//	
//	printf("ϵͳ��ʼ��AT24C20��ȡ:%s\r\n",datatemp);
//	

//  AT24CXX_Init();			  //IIC��ʼ�� 
//	rtc_num=AT24CXX_Check();
//	
//	//if(rtc_num==1) { printf("ϵͳ��ʼ��AT24C20:ʧ��\r\n");}
// while(AT24CXX_Check()){
//	 printf("ϵͳ��ʼ��AT24C20:ʧ��\r\n");
//	 delay_ms(500);
//	 
// }


	
//	AT24CXX_WriteOneByte(0,25);
//	AT24CXX_WriteOneByte(1,24);
//	AT24CXX_WriteOneByte(2,23);
//	AT24CXX_WriteOneByte(3,22);
//	i=AT24CXX_ReadOneByte(0);
//	i_1=AT24CXX_ReadOneByte(1);
//	i_2=AT24CXX_ReadOneByte(2);
	
	
	
	delay_ms(100);	
	W25QXX_Read(datatemp,100,19);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
	
	i=datatemp[18];
	
	if(i==33){ read_ch1_6_at2402();}  //���洢���������ݶ�ȡ��ϵͳ�
	
	
//	i_1=AT24CXX_ReadOneByte(17);
	
	printf("W25qxx����������Ϊ:%d,%d\r\n",i,i_1);
	
	
	
	//TIM3_PWM_Init(899,270);	//����Ƶ��PWMƵ��=72000000/900=80Khz  899   (899999,4)
	
	printf("ϵͳ��ʼ������");
	delay_ms(2000);
  delay_ms(2000);
	
	for(y=0;y<7;y++){
	for(i=0;i<7;i++)
	{					
			USART_SendData(USART2,xian_buf[i]);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);				
	}
	
}
	

//���ϴα���ĵ������Ʋ�����Ϣ���͵���ʾ����ȥ
 set_ch1_6_to_display();
 set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ







//������λ����Ϊ�ر�״̬ ��Ȼ���������  	
	
while(Start_KEY_Scan_dian()){
	
	LEEP=1;

	
}

LEEP=0;

OE_1=0;
POWER_1=1;
	




	
	OSInit(&err);		     //��ʼ��UCOSIII
	OS_CRITICAL_ENTER(); //�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
								 
}


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )1,			//20*10=200ms
                (OS_TICK	 )20,          //100*10=1000ms  1��10=20msһ��
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����
		
								
	OS_CRITICAL_ENTER();	//�����ٽ���
								
	//����TASK1����
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  //2��ʱ��Ƭ����2*5=10ms					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//����TASK2����
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				         (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,	//2��ʱ��Ƭ����2*5=10ms					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
								 
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
								 
								 
}




//����3�յ���λ����ִ��������в���  ���� ��ѯ �ػ� ��������    ��ѯ�ź�ǿ�� �������ݴ���  ÿ�������Ҫ���лظ�����λ�� 
//


//task1������
void task1_task(void *p_arg)
{
	u8 i,j,task1_num=0;
	u8 key=0; 
	OS_ERR err;
	p_arg = p_arg;

	OSTmrStart(&tmr1,&err);	//������ʱ��1
	
  
	
	while(1)
	{
		
	
  system_start=2;    //��ʱ����������п���
		
//	system_start_EL=1; //���԰汾�رձ����汾	
		
	//USART2_GET();
	

   key_set();
		
//    MCU_O2=0;
		
//		LED_3=0;
//		LED_4=0;
//		MCU_MUS=0;
//		
//		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
//		LED_3=1;
//		LED_4=1;
//	  MCU_MUS=1;
		
		

	 OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s	
		
		
	}
	
	
}

//task2������
void task2_task(void *p_arg)
{
	u8 i,j;
	OS_ERR err;
	u16 temp=0;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	

	while(1)
	{


	//	ch3_time
		
		
	
   //printf("1:%d\r\n",mang_1);
   //printf("2:%d\r\n",mang_2);
		
//	 printf("�ڶ�·��ֵ:%d\r\n",ch2_time);
//   printf("����·��ֵ:%d\r\n",ch3_time);


		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s

		

		
		
	}
	
}




//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	u8 i,t,a,b,c;
	p_arg = p_arg;
  
	//200ms����һ�ζ�ʱ�� 
//	set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
	   
	on_leep();        //�򿪷������������������������� 
	//�°汾��·��·�����򣬴�����
  check_ch1x6_dk();
	
  LED_G= ~LED_G;
	
 

	//ͨ��2��Ħ�������Զ��仯
	if(set_ch2_bo==4){
		
		ch2_time_s++;
		
		if(ch2_time_s>=6){
		ch2_unm_E1_t=(ch2_unm_E1_t-10);
		if(ch2_unm_E1_t<=12){
			
		ch2_unm_E1_t=560;
		}
		ch2_time_s=0;
	}
		
	}
	
		//ͨ��2��Ħ�������Զ��仯
	if(set_ch6_bo==4){
		
			ch6_time_s++;
		
		if(ch6_time_s>=6){
		ch6_unm_E1_t=(ch6_unm_E1_t-10);
		if(ch6_unm_E1_t<=12){
			
			ch6_unm_E1_t=560;
		}
	  ch6_time_s=0;
	}
		
	}
	
	
			//ͨ��2��Ħ�������Զ��仯
	if(set_ch5_bo==4){
		
		ch5_time_s++;
		if(ch5_time_s>=6){
		ch5_unm_E1_t=(ch5_unm_E1_t-10);
		if(ch5_unm_E1_t<=12){
			
			ch5_unm_E1_t=560;
		}
		ch5_time_s=0;
		
		}
	
		
		
	}
	
			//ͨ��2��Ħ�������Զ��仯
	if(set_ch4_bo==4){
		
		ch4_time_s++;
		if(ch4_time_s>=6){
		ch4_unm_E1_t=(ch4_unm_E1_t-10);
		if(ch4_unm_E1_t<=12){
			
			ch4_unm_E1_t=560;
		}
			ch4_time_s=0;
		
		}
		
		
		
	}
	
				//ͨ��2��Ħ�������Զ��仯
	if(set_ch3_bo==4){
		
		
		ch3_time_s++;
		if(ch3_time_s>=6){
		ch3_unm_E1_t=(ch3_unm_E1_t-10);
		if(ch3_unm_E1_t<=12){
			
			ch3_unm_E1_t=560;
		}
		ch3_time_s=0;
		
		}
		
	}
	
			//ͨ��2��Ħ�������Զ��仯
	if(set_ch1_bo==4){
		
		ch1_time_s++;
		if(ch1_time_s>=6){	
		ch1_unm_E1_t=(ch1_unm_E1_t-10);
		if(ch1_unm_E1_t<=12){
			
			ch1_unm_E1_t=560;
		}
			ch1_time_s=0;
		
		}
		
		

		
		
	}
	
	
 //��ʱ����ʱ��1���ӣ�����һ��ʱ���¼����������ʱ�䷢������ʵ��
	
	feng_time++;
	if(feng_time>=300){
	feng_time=0;	
	disply_time();	
		
	}

//��ʱ������ʾ���ϵ�����ͼ��״̬
//	elect_time--;
//if(elect_time<=1){
//	elect_time=2;	
//	fa_set_elect();
//		
//	}
	
		//num_set=0;
		
	//	set_ms();
//  ch_X_set_ms(1,2);
 //   ch_X_set_ms(2,1);
//	
//	ch_X_set_ms(3,1);
//	ch_X_set_ms(4,1);
//	ch_X_set_ms(5,1);
//	
// ch_X_set_ms(6,1); 
		
	
//   texi_ch2();
		
//	MCU_O2=~MCU_O2;
		

	
}










