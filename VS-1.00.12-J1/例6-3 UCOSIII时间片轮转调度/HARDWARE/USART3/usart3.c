#include "sys.h"
#include "usart3.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "usart.h"
#include "usart.h"
#include "delay.h"
#include "BC26.h"
#include "stm32f10x_flash.h"
#include "stmflash.h"
#include "led.h"
#include "rtc.h"
#include "includes.h"

//#include "timer.h"
//#include "ucos_ii.h"

//#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//����3��ʼ������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/3/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	


//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
//�жϻ��洮������
uint8_t U3_P = 0;
int U3_P_x = 0;
uint8_t U3_P_RC =0;
//unsigned char U3BUF[256]={0};
uint8_t LAST_CHAR=0;
unsigned char U3TEMP[255];
u8 usart3_reved_boo=1;
extern u8 pdp_name[30];
extern u8 pdp_power[30];
extern u8 pdp_apn[20];

extern u8 zd_che_net;  //���������Ĺ���

u8 revert_buf[100];  //�ظ�A1����


u8 work_stay=0x03;    //����״̬  
u8 ietnet_stay=0x04;  //����״̬
extern u8 NL660_ip[4];
u8 jem_p,jem_g,jem_j;
u8 read_jem_p, read_jem_g, read_jem_j;

extern uint8_t U2_P;
extern unsigned char U2BUF[256];

extern u8 Objective_ip_B_1[4];
extern u8 Objective_ip_B_2[4];
extern u8 Objective_ip_B_3[4];
extern u8 Objective_ip_B_4[4];
extern u8 jh_boot; //�����־
extern u8 zh_send_A2;   //�Ƿ��������ظ�A2��������1����������� ��0���õ�IP��ַ�ǹر���������������־��0��

          //           0X08070000
u32 apn_name_ANDD1 =    0X08075000;
u32 apn_name_long_ADD2= 0X08073000;

u32 apn_power_ANDD2 = 0X08077000;
u32 apn_power_long_ANDD2 = 0X08077100;

u32 apn_apn_ANDD2 = 0X08074100;
u32 apn_apn_long_ANDD2 = 0X08077200;


u32 IP_ANDD3 = 0x08083800;
u32 IP_ANDD4 = 0x08086800;

u8 red_pdp_name[30];
u8 red_pdp_power[30];
u8 red_pdp_apn[20];

extern u8 xiaoqu[5];
extern u8 xinhao[8];

u8  qidiao_time=1;  //�رո������ʱ�����
u8	xidiao_time=0;  //�򿪸���ж��ʱ�����	
u8  seve_biaoding=0;

u8 NL660_ip[4]={0x00,0x00,0x00,0x00};//����ip

u8 send_cong_apn_bool=0;

u8 yanzhen[30]={0xFF,0x7E,0x00,0x1E,0x01,0x08,0x31,0x34,0x31,0x35,
0x34,0x33,0x32,0x31,0x08,0x31,0x34,0x31,0x35,0x34,0x33,0x32,0x31,0x05,0x31,0x34,0x31,0x35,0x34,0xAE};
u8 sever_2_buf[]={0xFF,0x7E,0x00,0x1E,0xA2,0x03,0x04,0x05,0x31,0x34,0x31,0x35,0x34,0x05,0x31,0x34,0x31,0x35,0x34,0x05,0x31,
0x34,0x31,0x35,0x34,0x00,0x00,0x00,0x00,0x0A};
u8 sever_3_buf[]={0xFF,0x7E,0x00,0x06,0x03,0x84};
u8 sever_d_buf[]={0xFF,0x7E,0x00,0x07,0xAD,0x63,0x48};
u8 sever_gps_fall_buf[]={0xFF,0x7E,0x00,0x07,0xE9,0x00,0x6F};
u8 sever_gps_turn_buf[27]={0xFF,0x7E,0x00,0x1B,0xE9,0x01,0x13,0x01,0x1B,0x0B,0x06,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0xE8,0xA4,0x3C,0x08,0x95,0x5A,0xBC,0x57};

u8 sever_e8_buf[]={0xFF,0x7E,0x00,0x0A,0xE8,0x10,0xCD,0xD9,0xBA,0xDD};
//�ϴ��������궨���ݻ���������  
u8 send_biaodin_date[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
u8 send_shitime_date[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


	u8 send_zhuce_date[]={0xA5,0x5A,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x33,0xC3,0x3C};
	
		//�����ϴ�����buf
	u8 send_zhouqi_date[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

 u8 xin_tiao_time[]={0xA5,0x5A,0x0F,0x07,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0xCC,0x33,0xC3,0x3C};
	

 
 
//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���

u8 sevr_03_comd=0;

u8 biaoding_bool=0; //�ӵ��궨���ݵı�־,����������Ϸ�һ�α궨��Ϣ
u8 time_data_bool=0; //�ӵ�ʵʱ�����ϴ�����,�����������������һ֡�µ�����
u8 long_time_data_bool=0; //�ӵ����������ϴ�����֡,�����������������һ֡�µ�����
		
u8 zhouqi_bool;  //�����ϴ����Ʊ���  
		
extern u8 sever_time ;   //�Ƿ��յ�ʱ�����  		
		
/////////////////��ϻ�ӱ����洢�궨����////////////////////////////////
u8 shebei_1,shebei_2,shebei_3,shebei_4; 
 
u16 th_num=0; //�������
u16 liju=0; //�������߱���
u16 zo_x=0; //���� X  
u16 zo_y=0; //���� Y
u16 qizhon=0; //���ر۳�
u16 pingheng=0; //ƽ��۳�
u16 tagao=0;  //��ñ��
u16 qibh=0;   //���ر۸�
u16 ssb=0 ;   //��������
u16 gd_AD1=0 ; //�߶ȱ궨
u16 gd_X1 =0 ; //�߶ȱ궨
u16 gd_AD2=0 ; //�߶ȱ궨
u16 gd_X2 =0 ; //�߶ȱ궨

u16 fd_AD1=0 ; //���ȱ궨
u16 fd_X1 =0 ; //���ȱ궨
u16 fd_AD2=0 ; //���ȱ궨
u16 fd_X2 =0 ; //���ȱ궨

u16 hd_AD1=0 ; //��ת�궨
u16 hd_X1 =0 ; //��ת�궨
u16 hd_AD2=0 ; //��ת�궨
u16 hd_X2 =0 ; //��ת�궨

u16 ld_AD1=0 ; //�����궨
u16 ld_X1 =0 ; //�����궨
u16 ld_AD2=0 ; //�����궨
u16 ld_X2 =0 ; //�����궨

u16 fs_bd=0 ; //���ٱ궨У׼ֵ
u16 qx_bd=0 ; //��б�궨У׼ 
u16 gds_bd=0 ; //�߶�����λ
u16 gdx_bd=0;  //�߶�����λ
u16 fdjx_bd=0; //���Ƚ�����λ
u16 fdyx_bd=0; //����Զ����λ

u16 hzz_bd=0 ; //��ת����λ
u16 hzy_bd=0 ; //��ת����λ
u16 hpb_jl=0 ; //ˮƽ��������
u16 czb_jl=0 ; //��ֱ���۾���
u16 zlbj_bf=0 ; //���������ٷֱ�
u16 fsba_jz=0 ; //���ٱ���ֵ
u16 qxb_jz=0 ; //��б����ֵ
u16 spy_jl=0 ; //ˮƽԤ������ 
u16 czyj_jl=0 ; //��ֱԤ������ 
u16 zlyj_bf=0 ; //����Ԥ���ٷֱ� 
u16 fsy_jz=0  ; //����Ԥ��ֵ
u16 qxy_jz=0 ;  //��бԤ��ֵ 
u16 pzzd_yx=0 ; //��ײ�ƶ�����
u16 sfrz_qr=0 ; //�����֤�Ƿ�ȷ��
u16 gprs_sc=0 ; //GPRS ����   

//////////////////////////ʵʱ�ϴ�����֡�����洢//////////////////////////
u16 higt_unm=0;  //�߶� 
u16 fdu_unm=0;  //����
u16 hzu_unm=0;  //��ת
u16 zhong_unm=0 ; //����
u16 dqyxzd_unm=0 ; //��ǰ�����������
u16 zzbf_unm=0  ;  //���ذٷֱ�
u16 fengs_unm=0 ;  //����
u16 qxie_unm=0  ;  //��б
u16 gdxwz_unm=0 ;  //�߶���λֵ
u16 gdhzx_unm=0 ;  //�߶Ȼ�ת��λ
u16 zhxzz_unm=0 ;  //��������ֵ
u16 qxfsxz_unm=0 ; //��б��������
u16 gssxxw_unm=0 ; //����������λ
u16 gyqhxw_unm=0 ; //����ǰ����λ
u16 gyzyxw_unm=0 ; //����������λ
u16 pzsxxw_unm=0 ; //��ײ������λ
u16 pzqhxw_unm=0 ; //��ײǰ����λ
u16 pzzyxw_unm=0 ; //��ײ������λ
u16 jdqzt_unm=0  ; //�̵���״̬
u16 gzzt_unm=0   ; //����״̬
u16 qgqljzt_unm=0 ; // ����������״̬ 

////////////////////������������֡���ݴ洢////////////////////////////////
u16 qdsj_unm=0;   //���ʱ�� 
u16 qddgd_unm=0 ; //�����߶�
u16 qddfd_unm=0 ; //��������
u16 qddhz_unm=0 ; //������ת
u16 ydsj_unm=0  ; //ж��ʱ�� 
u16 xdgd_unm=0  ; //ж���߶�
u16 xdfd_unm=0  ; //ж������
u16 xdhz_unm=0  ; //ж����ת
u16 bcdz_unm=0  ; //���ε���  
u16 zdfh_unm=0  ; //��󸺺� 

vu16 USART3_RX_STA=0;   	 
void USART3_IRQHandler(void)
{
	u8 res;	    
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
//		USART_ClearFlag(USART3,USART_FLAG_RXNE);
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		 res=USART_ReceiveData(USART3);
		
	//	if((U3_P_x<255)&&(usart3_reved_boo))//�������յ���������ֵ�����
		if(U3_P_x<255)
		{

			U3TEMP[U3_P_x] = res;
	  USART_SendData(USART3,U3TEMP[U3_P_x]);
//    LAST_CHAR=U3TEMP[U3_P_x];
			U3_P_x++;	
			
		}
	}

#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
#endif
		 											 
}   

//��ȡ���յ������ݺͳ���
unsigned char *get_rebuff3(uint8_t *len)
{
    *len = U3_P_x;
    return (unsigned char *)&U3TEMP;
	
}

//��ȡ��������
uint8_t Type_Get()
{
	
	uint8_t lens[1];
	u16 sever=0x0000,key=0;
	
	unsigned char *addr;
	//addr=get_rebuff3(lens);
	addr=get_rebuff2(lens);
	key=addr[4]<<8;
	sever= key+addr[5];
	printf("�յ������ݼĴ�����ַΪ��%x",sever);
	switch(sever)
	{
	  case 0x1030:   //�������ˮ
		return 1;
		break;	
		
		case 0x1015:   //�������ˮ
		return 4;
		break;	
			
		case 0x1017:   //�����Ӫ��Һ 

		return 5;
		break;
		
		case 0x101A:   //�����Ӫ��Һ 
		return 6;
		break;
		
		case 0x0010:   //����ʱ�ӵ�Сʱָ�� 
		return 7;
		break;
		
		case 0x0012:   //����ʱ�ӵķ���ָ�� 
		return 8;
		break;
		
		case 0x0014:   //����ʱ�ӵ���ָ�� 
		return 9;
		break;
		
		case 0x0180:   //�������������ȵ��� 
		return 10;
		break;
				
				
		case 0x0122:   //���չ��ָ��
	  time_data_bool=1;
		return 2;
		break;
		
		case 0x0124:   //�򿪷���ָ��  
	  //long_time_data_bool=1;
		return 3;
		break;
		
	}
	
	
	
	
	return 0;
	
}




//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart3_init(u32 bound)
{  	 
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����3
		 //USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������	3
  

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
//	TIM7_Int_Init(99,7199);		//10ms�ж�
	USART3_RX_STA=0;		//����
//	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	u8 *pbuf;
	va_list ap;
//	pbuf=mymalloc(SRAMIN,USART3_MAX_SEND_LEN);	//�����ڴ�
	if(!pbuf)									//�ڴ�����ʧ��
	{
		printf("u3 malloc error\r\n");
		return ;
	}
	va_start(ap,fmt);
	vsprintf((char*)pbuf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)pbuf);				//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������      
		USART_SendData(USART3,pbuf[j]); 
	}
//	myfree(SRAMIN,pbuf);						//�ͷ��ڴ�
}


/****************************************************************************
* ��	�ƣ�void USART3_SendByte(u8 Data)
* ��	�ܣ����ַ�����
* ��ڲ�����Data 	���͵��ַ�����
* ���ڲ�������
* ˵	������				   
****************************************************************************/
void USART3_SendByte(u8 Data)		   //���ַ��������
{
	USART_SendData(USART3, Data);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* ��	�ƣ�void USART3_SendString(u8* Data,u32 Len)
* ��	�ܣ����ַ����
* ��ڲ�����Data 	����ĵ��ַ�����
			Len		�ַ�����
* ���ڲ�������
* ˵	������					 
****************************************************************************/
void USART3_SendString(u8* Data,u32 Len)		   //���ַ����
{
	u32 i=0;
	for(i=0;i<Len;i++)
    {    
		USART_SendData(USART3, Data[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

void USART3_DMAS(u8* Data)
{
	while(*Data)
	{
		USART3_SendByte(*Data++);
	}
}

void clean_rebuff3(void)
{
	uint8_t i;
	for(i=0;i<255;i++){
		U3TEMP[i]=0x00;
	}
	U3TEMP[255]=0x00;
  U3_P_x = 0;
}
//�ж�U1_P�Ƿ�������ӣ������жϴ��ڷ����Ƿ����
uint8_t CK_U3P()
{
	if(U3_P_RC!=U3_P_x){
		
		U3_P_RC=U3_P_x;
		return 0;
		
	}
	
	U3_P_RC=0;
	return 1;
}

uint8_t Check_HD(unsigned char *addr)
{

	if(addr[0]==0xA5 && addr[1]==0x5A ){
		return 0x01;
	}
	//clean_rebuff3();
	return 0x00;
}
//����У�飺��ȷ����1�����󷵻�0
uint8_t Check_CK(unsigned char *addr)
{
	uint8_t i,j,k;
	unsigned char s=0x00;
	unsigned char checksum =addr[3];
	j=addr[2];//����

	 k=ASCII_u16(addr[3])-1; //��������λ��
	 s=Q24_CheckSum(addr,k);
	// printf("du check_CK  is:%x\r\n",s);
	
	if(s==addr[3])
	{
		return 0x02;//У����ȷ
	}
	
	return 0x00;
}


uint8_t BLE_Check(void)
{
	uint8_t lens[1],k,s;
	unsigned char *addr;
	uint16_t i,j;

	//addr=get_rebuff3(lens);  //��������ָ����������ʱָ���������
	

	addr=get_rebuff2(lens);
	
	if(Check_HD(addr)==0x01)  //�ж�������ͷ�Ĵ��� 0xff 0x7e 
		{	
			
//			// k=ASCII_u16(addr[3])-1; //��������λ��
//			 k=addr[3]-1; //��������λ��
//			 s=Q24_CheckSum(addr,k);
	//	   printf("du check_CK  is:%x\r\n",s);
	//		 printf("du check_CK  is:%x\r\n",addr[k]);
//			if(s==addr[k])
//			{
	//		printf("jiao yan cheng gong ",s);
				
			return 0x02;//У����ȷ
			
			
		//	}
			
			
		}
		addr=NULL;
		 return 0x00;//��ͷ��Ч���������μ�⣬��մ��ڻ��嵥Ԫ
		
	
	  

	

}

u8 send_server_data(void)
{
	u8 i,y,ret_x=0;
	uint32_t  ret=0;
	OS_ERR err;
	
	  for(y=0;y<20;y++)
	{
		ret_x=0;
		ret=0;
		for(i=0; i<10; i++)
		{
			ret += send_zhuce_date[i] ;
		}
		//USART2_printf( USART2, "\r\n У�� read is  %d \r\n", ret );
    ret_x =(ret&0xff);		
   // USART2_printf( USART2, "\r\n У�� read is  %d \r\n", ret_x );		
		send_zhuce_date[10]=0x00;
		send_zhuce_date[10]=ret_x;	

		// send_zhuce_date[10]=TX_CheckSum(send_zhuce_date,10);
	
		for(i=0;i<15;i++)
		{			
		 USART2_SendByte(send_zhuce_date[i]);					
		}	
		
		 OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			
			if(sever_time){ return 1; y=20; } ; //������ 3�Σ���ûһ��Ϊ��� ���жϴ���2�յ���û���յ���������Ӧ��ظ���û�о������豸
			
			
			
		}
		if(!sever_time) return 0;

	
}

uint8_t USART3_GET(void)
{
	//��������3���ܵ����ͨ������2������Ӧ��ATָ���������ģ��  

	u8 k,s,b;
	u8 h;
	uint16_t j,i,jeep;
	uint8_t lens[1];
	uint16_t net_port;
	u8 CSQ;
	uint8_t shu_a=0,shu_b=0,shu_g=0;
	uint32_t j_du;
	uint32_t w_du;
	uint32_t du_a,du_b,du_c;

	uint32_t fs_du;   //���ٱ�ʾ
	uint16_t fx_du;   //�����ʾ
	
	u8 w_s=0;        //����һ������ 
	u8 weizhang_bool=0;
	

	 u8 ret_x = 0;
   uint32_t ret=0;

	
	
	
	
	
	uint16_t read_remj;
	
	unsigned char *add_ut1;
	unsigned char *addr;
	
	if(U3_P_x>0){

	  if(CK_U3P())  //���������û�н�������
	{	
    		
			if(BLE_Check()==0x02)	 //���BLE������,0x02Ϊ��鵽��������ȷ����
			{
				
				USART2_DMAS("zou ya feng ffffffffffffffffffff\r\n");
				
        s=Type_Get();
			//	USART2_printf( USART2, "\r\n read is  %d \r\n", s );
		//		USART3_SendByte(s);
			switch(s)
						{
						//�õ���ˮ��ָ���
						case 0x01 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ��ˮ�ò����ֶ�ָ�� %d \r\n",th_num);	
					
            j=th_num&0x01;
						if(j){ PWOR_YANG=0; } else PWOR_YANG=1;
						
						}		 
					  break ;
						
						case 0x04 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n �򿪳���ˮ��Ӫ��ҺͰ��ָ�� %d \r\n",th_num);	
            j=th_num&0x01;
						if(j){ PWOR_SHU=0; } else PWOR_SHU=1;
						
						}		 
					  break ;	
						
						case 0x05 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ��ˮ�ò����ֶ�ָ�� %d \r\n",th_num);	
            j=th_num&0x01;
						if(j){ PWOR_YINGYANG=0; } else PWOR_YINGYANG=1;
						
						}		 
					  break ;	
						
						
		       //�õ�ʵʱ����֡���ݣ���ʼ����  			
						case  2 :
					{
					  addr=get_rebuff3(lens);
		 				th_num=addr[8];  //�������      
						USART2_printf( USART2, "\r\n �չ�Ʋ����ֶ�ָ�� %d \r\n",th_num);	 
						j=th_num&0x08;
					//	if(j){ LED_1=0; } else LED_1=1;
            j=th_num&0x04;
						if(j){ LED_2=0; } else LED_2=1;						
						j=th_num&0x02;
						if(j){ LED_3=0; } else LED_3=1;
            j=th_num&0x01;
						if(j){ LED_4=0; } else LED_4=1;
							
					}
					  break;
					case  3 :
					{
					  addr=get_rebuff3(lens);
		 				th_num=addr[8];  //�������      
						USART2_printf( USART2, "\r\n ���Ȳ����ֶ�ָ�� %d \r\n",th_num);	
						
						j=th_num&0x08;
						if(j){ feng_1=0; } else feng_1=1;
            j=th_num&0x04;
						if(j){ feng_2=0; } else feng_2=1;						
						j=th_num&0x02;
						if(j){ feng_3=0; } else feng_3=1;
            j=th_num&0x01;
						if(j){ feng_4=0; } else feng_4=1;
				
							
					}
					  break;
					}
				}

       
						
			clean_rebuff3();	//���
				
			}
			 }

	}

	
	
	//������һ���ַ�������ڼ���che������Ϊλ��ֵ
	//che:�ָ����ʮ�����Ʊ�ʾ,����λ�150���ֽ�  
	//k���������һ���ָ�����濪ʼȡ����
	//���أ���������ĵڼ�λ���ֿ�ʼȥ����
	int chenk_date(u8 *str, u8 che , int k )
	{
		 u8 *s;
		int i,j,h,y;
     j=0;
		 h=0;
		 y=k;
		s=str;
	//	printf("du �� �ַ��� is:%s\r\n",s);
		
		for(i=0;i<70;i++){
			 h++;
			if(s[i]==che)
				{
				   j++;
					
				//	printf("du �� jss is:%d\r\n",j);
					 if(j==y){
						 return h;
						 
						 
					 }
				}
			
		}
		//h=0;
	
		
	}
	


unsigned char Q24_CheckSum(unsigned char str[], int n)
{
   unsigned char checksum =str[0];
	 int i;
     for(i=1;i<n;i++)
          checksum ^= str[i];
     return checksum;
}




/*************************************************
����: Flash_write_buf(u32 addr,u8 *p,u16 n)
����: д���ݵ�Ƭ��flash
����: addr:��ַ
			*p:д�������
			n:����
����: ��
**************************************************/
void Flash_write_buf(u32 addr,u8 *p,u16 n)
{
	int i;
	FLASH_Unlock();//flash����
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//�����־
	FLASH_ErasePage(addr);
	while(n--)
	{
		i=*p;
		FLASH_ProgramWord(addr, i);
		p++;
		addr+=4;
	}
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//�����־
	FLASH_Lock();//flash����
}
/*************************************************
����: Flash_read_buf(void)
����: ��Ƭ��flash
����: ��
����: ��
**************************************************/
void Flash_read_buf(u32 addr,u8 *p,u16 n)
{
	while(n--)
	{
		*p = *(int*)addr;
		p++;
		addr+=4;
	}
}

void read_apn_power(void)
	{
				read_jem_p=STMFLASH_ReadHalfWord(apn_name_long_ADD2);
			//	printf("du read_jem_p is:%d \r\n",read_jem_p);
				STMFLASH_Read(apn_name_ANDD1,(u16*)red_pdp_name,(u16)read_jem_p);
		//		printf("du red_pdp_name is:%s \r\n",red_pdp_name);
					
				read_jem_g=STMFLASH_ReadHalfWord(apn_power_long_ANDD2);
			//	printf("du read_jem_g is:%d \r\n",read_jem_g);
				STMFLASH_Read(apn_power_ANDD2,(u16*)red_pdp_power,(u16)read_jem_g);
			//	printf("du red_pdp_power is:%s \r\n",red_pdp_power);
				
				read_jem_j=STMFLASH_ReadHalfWord(apn_apn_long_ANDD2);
		//		printf("du read_jem_j is:%d \r\n",read_jem_j);
				STMFLASH_Read(apn_apn_ANDD2,(u16*)red_pdp_apn,(u16)read_jem_j);
			//	printf("du red_pdp_apn is:%s \r\n",red_pdp_apn);

}


int zh_send_a2(void)
	{
	u8 i,j,k,s;
	s=0;
		        s=get_ip();
		      
		        if(s){
						work_stay=0x03;
						ietnet_stay=0x01;
		        sever_2_buf[5]=work_stay;	//
						sever_2_buf[6]=ietnet_stay;}
						else {
							work_stay=0x03;
						  ietnet_stay=0x02;
							sever_2_buf[5]=work_stay;			
						  sever_2_buf[6]=ietnet_stay;
						}
			
									sever_2_buf[7]=jem_p;
									//ȡpdp���û���
									for(i=0;i<jem_p;i++)
									{
									sever_2_buf[8+i]=pdp_name[i];
									
									}
									j=8+jem_p;
									//ȡ����
									sever_2_buf[j]=jem_g;
									for(i=0;i<jem_g+1;i++){
									 sever_2_buf[j+1+i]=pdp_power[i];
									}
									j=j+jem_g+1;
									//ȥAPN��
									sever_2_buf[j]=jem_j;
									for(i=0;i<jem_j+1;i++){
										sever_2_buf[j+1+i]=pdp_apn[i];
									}
									j=j+jem_j+1;
									
				
						if(s){
							for(i=0;i<5;i++){
							sever_2_buf[j+i]=NL660_ip[i];
							zh_send_A2=0;     //�ر������ϱ�����״̬����
						  }
        	    }else{						
								//ȡip��ַ����������������
								for(i=0;i<5;i++){
									sever_2_buf[j+i]=0x00;
								}
					}
						
						j=j+5;
						sever_2_buf[j-1]=Q24_CheckSum(sever_2_buf,j-1);
						
						sever_2_buf[3]=j; //��ֵ֡������ 
						
						for(i=0;i<j;i++){
							
							USART3_SendByte(sever_2_buf[i]);
							
						}
						
				return s;
					
}
	
int zh_send_a2_sidix(void)
	{
	u8 i,j,k,s;
	s=0;
		 clean_rebuff2();
		USART2_DMASS("AT+CREG?\r\n","OK",1000,500);
		if((strstr(U2BUF,"+CREG: 2,0")!=NULL)||(strstr(U2BUF,"+CREG: 2,2")!=NULL)
			||(strstr(U2BUF,"+CREG: 2,3")!=NULL)||(strstr(U2BUF,"+CREG: 2,4")!=NULL))//�жϻظ�����
		{
     s=1;
			ietnet_stay=0x04;
			jh_boot=0;
						LED_G=1;
						LED_R=0;
		}else {
			s=0;
		}
			      
						
		        sever_2_buf[5]=work_stay;	// ����״ֵ̬���ı�
						sever_2_buf[6]=ietnet_stay;
		
				if(send_cong_apn_bool){  
									sever_2_buf[7]=jem_p;
									//ȡpdp���û���
									for(i=0;i<jem_p;i++)
									{
									sever_2_buf[8+i]=pdp_name[i];
									
									}
									j=8+jem_p;
									//ȡ����
									sever_2_buf[j]=jem_g;
									for(i=0;i<jem_g+1;i++){
									 sever_2_buf[j+1+i]=pdp_power[i];
									}
									j=j+jem_g+1;
									//ȥAPN��
									sever_2_buf[j]=jem_j;
									for(i=0;i<jem_j+1;i++){
										sever_2_buf[j+1+i]=pdp_apn[i];
									}
									j=j+jem_j+1;
									
					}else{
						
						      sever_2_buf[7]=read_jem_p;
									//ȡpdp���û���
									for(i=0;i<read_jem_p;i++)
									{
									sever_2_buf[8+i]=red_pdp_name[i];
									
									}
									j=8+read_jem_p;
									//ȡ����
									sever_2_buf[j]=read_jem_g;
									for(i=0;i<read_jem_g+1;i++){
									 sever_2_buf[j+1+i]=red_pdp_power[i];
									}
									j=j+read_jem_g+1;
									//ȥAPN��
									sever_2_buf[j]=read_jem_j;
									for(i=0;i<read_jem_j+1;i++){
										sever_2_buf[j+1+i]=red_pdp_apn[i];
									}
						
						      j=j+read_jem_j+1;
						
					}
		
		
		
		       
									
				
						if(s)
							{						
								//ȡip��ַ����������������
								for(i=0;i<5;i++){
									sever_2_buf[j+i]=0x00;
								}
					}
						
						j=j+5;
						sever_2_buf[j-1]=Q24_CheckSum(sever_2_buf,j-1);
						
						sever_2_buf[3]=j; //��ֵ֡������ 
						if(s){
						for(i=0;i<j;i++){
							
							USART3_SendByte(sever_2_buf[i]);
						}
					}
				return s;
					
}
	
//�ۼӺ�У�鷽ʽ     
u8 TX_CheckSum(u8 *buf, u8 len) 
{ 
    u8 i, ret_x = 0;
    uint64_t ret= 0;
    for(i=0; i<len; i++)
    {
        ret += *(buf++);
    }
		
	//	USART2_printf( USART2, "\r\n read is  %d \r\n", ret );
    ret_x =ret&0xff;
    return ret_x;
}




