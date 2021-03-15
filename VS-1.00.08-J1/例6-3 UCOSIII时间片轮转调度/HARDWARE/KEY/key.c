#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "24cxx.h"	
#include "w25qxx.h"	

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
		
extern u8 set_ch1_bo;  //����Ĭ�������������
extern u8 set_ch2_bo;  //����Ĭ�������������
extern u8 set_ch3_bo;  //����Ĭ�������������
extern u8 set_ch4_bo;  //����Ĭ�������������
extern u8 set_ch5_bo;  //����Ĭ�������������
extern u8 set_ch6_bo;  //����Ĭ�������������

extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //����Ĭ������10ms 
extern	u8 tan_xue_boo; //̽Ѩ��⵽���庯�� ����⵽ѨλʱΪ2�����Ѩλ��Ӧ��Ϊ0��ӦΪ���𶯵�·������Ѩλ�źţ�����ÿ����Ӧ�궼��Ϊ0 .					
u8 tong_start=0;	  //��ȡ��һ��ͨ����������ť����״̬ 0�Ǵ򿪣�1�ǹر�  5λ��1ͨ��  4λ��2ͨ�� 3λ��3ͨ��  2��4ͨ��   1λ��5ͨ��   0λ��6ͨ��  					
extern u16 elect_time; //��ʱ������Ļ�ϵ�������ʾͼ��״̬						
						
u8 set_tong=1; //��ǰ��������һ��ͨ������ʼ��������1ͨ����
extern u8 wendu[9] ;

u8 key_umb=0;

u8 key_staty=4;




u8 time_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x02,0x00,0x00};  //����1ͨ��ʱ��ı�������  ��ʱ 
u8 time_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x04,0x00,0x00};  //����2ͨ��ʱ��ı�������
u8 time_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x06,0x00,0x00};  //����3ͨ��ʱ��ı�������
u8 time_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x08,0x00,0x00};  //����4ͨ��ʱ��ı�������
u8 time_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x10,0x00,0x00};  //����5ͨ��ʱ��ı�������
u8 time_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x12,0x00,0x00};  //����6ͨ��ʱ��ı�������


u8 zhou_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x14,0x00,0x00};  //����1ͨ��ʱ��ı�������  ����
u8 zhou_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x16,0x00,0x00};  //����2ͨ��ʱ��ı�������
u8 zhou_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x18,0x00,0x00};  //����3ͨ��ʱ��ı�������
u8 zhou_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x20,0x00,0x00};  //����4ͨ��ʱ��ı�������
u8 zhou_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x22,0x00,0x00};  //����5ͨ��ʱ��ı�������
u8 zhou_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x24,0x00,0x00};  //����6ͨ��ʱ��ı�������



u8 boxin_1[9]={0xA5,0x5A,0x05,0x82,0x01,0x01,0x00,0x00};  //����1ͨ��ʱ��ı�������   ����
u8 boxin_2[9]={0xA5,0x5A,0x05,0x82,0x01,0x02,0x00,0x00};  //����2ͨ��ʱ��ı�������
u8 boxin_3[9]={0xA5,0x5A,0x05,0x82,0x01,0x03,0x00,0x00};  //����3ͨ��ʱ��ı�������
u8 boxin_4[9]={0xA5,0x5A,0x05,0x82,0x01,0x04,0x00,0x00};  //����4ͨ��ʱ��ı�������
u8 boxin_5[9]={0xA5,0x5A,0x05,0x82,0x01,0x05,0x00,0x00};  //����5ͨ��ʱ��ı�������
u8 boxin_6[9]={0xA5,0x5A,0x05,0x82,0x01,0x06,0x00,0x00};  //����6ͨ��ʱ��ı�������


u8 kuang_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x25,0x00,0x02};  //����1ͨ��ʱ��ı�������  ����ѡ���
u8 kuang_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x26,0x00,0x02};  //����2ͨ��ʱ��ı�������
u8 kuang_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x27,0x00,0x02};  //����3ͨ��ʱ��ı�������
u8 kuang_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x28,0x00,0x02};  //����4ͨ��ʱ��ı�������
u8 kuang_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x29,0x00,0x02};  //����5ͨ��ʱ��ı�������
u8 kuang_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x30,0x00,0x02};  //����6ͨ��ʱ��ı�������


u8 HOT_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x40,0x00,0x00};  //����ָʾ����ʾ����ʾ����
u8 ELCK_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x35,0x00,0x00};  //���������ָʾ����ʾ����

u8 XUE_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x44,0x00,0x00};  //̽Ѩָʾ����ʾ����

u8 JS_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x50,0x00,0x00};  //��·��·���ر�



u8 ELCK_UF[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


extern  u8 xian_buf[6];

bool  H1_BOOl=0,H2_BOOl=0,H3_BOOl=0,H4_BOOl=0,H5_BOOl=0,H6_BOOl=0;
u8 H1_um=0,H2_um=0,H3_um=0,H4_um=0,H5_um=0,H6_um=0;
u8 ELCK1_um=0,ELCK2_um=0,ELCK3_um=0,ELCK4_um=0,ELCK5_um=0,ELCK6_um=0;

u8 HOT_unm=0,ELCK_unm=0,ELCK_unm_X,ELCK_unm_U1,ELCK_unm_U2,ELCK_unm_U3,ELCK_unm_U4;  //������ȣ������� �����������洢��ַ��

u8 ELCK_unm_i=0;

extern u8 system_start_EL;  //��·��·ֹͣ����������ʶ

bool tang_bool=0;           //̽Ѩ��־

bool clos_bis=TRUE;   //��ӳ���̽Ѩ�����رձ�����������  

extern u8 clos_bis_unm;//��ӳ���̽Ѩ�����رձ�����������  

extern u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;


u8 key_up=1;   //�������ɿ���־ ,���Ƽ���ɨ�躯���Ƿ�֧����������
u8 key_up_1=1; //�������ɿ���־ ,���Ƽ���ɨ�躯���Ƿ�֧����������
u8 key_up_2=1; //�������ɿ���־ ,���Ƽ���ɨ�躯���Ƿ�֧����������

u8 key_ele_unm=0; //�����������ݴ�����λ��

u8 zhi_unm_date=0;





//������Ϣ����ṹ��
//set_time;			���ù�������ʱ��  
//zhou_time;    ���������������	
//bo_xin;       ���ò����������  1->����  2->�ᴷ  3->����  4->��ĦE1 5->��ĦE2 6->��ĦE3 
//shen_time;    ����ʣ��ʱ�䣨��ʱδ���ã�
struct _zhi_liao 
{										    
	u8 set_time;			//���ù�������ʱ��  
	u8 zhou_time;			//���������������	
	u8 bo_xin;				//���ò����������  1->����  2->�ᴷ  3->����  4->��ĦE1 5->��ĦE2 6->��ĦE3 
  u8 shen_time;     //����ʣ��ʱ�䣨��ʱδ���ã�

}; 	 

//ͨ��1������Ϣ����ṹ��
struct _zhi_liao tongdao_1={60,2,1,60};
//ͨ��2������Ϣ����ṹ��
struct _zhi_liao tongdao_2={60,2,1,60};
//ͨ��3������Ϣ����ṹ��
struct _zhi_liao tongdao_3={60,2,1,60};
//ͨ��4������Ϣ����ṹ��
struct _zhi_liao tongdao_4={60,2,1,60};
//ͨ��5������Ϣ����ṹ��
struct _zhi_liao tongdao_5={60,2,1,60};
//ͨ��6������Ϣ����ṹ��
struct _zhi_liao tongdao_6={60,2,1,60};


//�������������ʱ�䶨��
u8 ch1_start=2,ch2_start=2,ch3_start=2,ch4_start=2,ch5_start=2,ch6_start=2;




//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTA,PORTEʱ��
  //����ɨ�������ʼ������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_6;    //���Ȱ������������������ ��һ�汾��PE0 ��ĳ�PE0 ; ���������������� ��һ�汾��PC13 ��ĳ�PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;          //GPIO_Mode_IPU; //���ó��������� ���Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //��ʼ��	
  GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_6);
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //���ü��̹��������λ ��һ�汾��PD9 ��2�汾�ĳ� PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó���������  ��ĳ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	//��һ�汾��������GPIO���� KY01-PC8   KYO2-PC9  KYO3-PA8   KY04-PC0   KK05-PC11   KYO6-PC12
	//�ڶ��汾��������GPIO���� KY01-PD5   KYO2-PD4  KYO3-PD3   KY04-PD2   KK05-PC9   KYO6-PC8
	
	//��ʼ�� WK_UP-->GPIOA.8	  ��������  ���������ʼ������Ϊ0    KOY3.
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;			//���ó���������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.0
 // GPIO_ResetBits(GPIOA,GPIO_Pin_8);


	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9;			// �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOA.0
  //GPIO_ResetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8);
	
 // GPIO_SetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8);
 // delay_ms(100);

//  HOT_IO_IN();
//////  HOT_IO_OUT();
//// // HOT_DQ_OUT=0;

//   KOY6_IO_IN();
//		//KOY6_IO_OUT()
//	//	KOY6_DQ_OUT=1;
////		
//KOY5_IO_IN();
//KOY4_IO_IN();
//KOY2_IO_IN();
//KOY1_IO_IN();
//KOY3_IO_IN();




}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(void)
{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
//	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
// 	return 0;// �ް�������
//	
//	
//  int cord_h,cord_l;//����ֵ
	int cord_buf=0,cord_buf_L=0,key=0xffff;
	
	//��������Ϊ0����ȡ�����ֵ��
	
	
	
	key_start(1);          //�ȶ�����ֵ����ֵΪ��λ���������1Ϊ����ֵ��6�е�λȫΪ�㣻������ 
 
	cord_buf=key_red_H(); 
	
	
 if(cord_buf!=0x01C0)
	{  			
	
		printf("��������3��%X\r\n",cord_buf);
		
	  key_umb=key_staty;		//���¸���ɨ��ʱ��	
		
			//printf("ɨ�赽������\r\n");
		 //delay_ms(50);        //�����������
	 
	  cord_buf=key_red_H();  //�ٴζ�ȡһ����ֵ��
	 if(cord_buf!=0x01C0)
	  { 
			
     
			
		//	KEY_Init();
			
		//��ȡ����ļ�����ֵ   
		key_start(2);
		//KEY_Init();
			
		// delay_ms(50);
			
		cord_buf_L=	key_red();	//ȡ�����룬���水�º�����λΪ�� ����λΪ�ߡ�û�а�����ֵΪ0X3F��Ӧ 11 1111  	
		printf("�������룺%X",cord_buf_L);	
				
		//if(cord_buf_L==0x1F) printf("ɨ�赽������0XDF\r\n");
			
		cord_buf=cord_buf+cord_buf_L;	
		 switch(cord_buf)
		{	
			
      case 0xDF:      //����0xC0  ����Ϊ 0X1F	 ���Ϊ case ֵ  
               key=1; //���� 6 ͨ��������
			printf("ɨ�赽������0XDF\r\n");
			
			return key;
			
       break;
			
			case 0xEF:   //����0xC0 ���� 0x2F   ���Ϊ case ֵ 
					 key=2;  //����5 ͨ�������� 
			return key;
			
	//		printf("ɨ�赽������0XFF\r\n");
					 break;

			case 0xF7:  //����0XC0 ���� 0x37    ���Ϊ case ֵ 
					 key=3;  //����4 ͨ��������
			return key;
			
   //   printf("ɨ�赽������0XF7\r\n");
			
					 break;

			case 0xFB:  //����0xC0 ���� 0x3B	   ���Ϊ case ֵ 
					 key=4; //����3 ͨ��������
			
					return key;
			
		//	printf("ɨ�赽������0XFB\r\n");
					 break;

		  case 0xFD:  //����0xC0 ���� 0x3D	   ���Ϊ case ֵ 
               key=5; //����2 ͨ��������
			
					return key;
			
		//	printf("ɨ�赽������0XFD\r\n");
               break;

		  case 0xFE:  //����0xC0 ���� 0x3E	  ���Ϊ case ֵ 
				
					printf("ɨ�赽������0XFE\r\n");
			
          key=6; //����1 ͨ��������               
              
					return key;
    	break;
			
			
		////////////////////������������ذ�������ɨ����////////////////////////

//		  case 0x15F:       //����0x140  ����Ϊ 0X1F	   ���Ϊ case ֵ 
//               key=7;
//			             //������ 6 ͨ��������
//			printf("ɨ�赽������0X15F\r\n");
//               break;			


//          case 0x16F:   //����0x140 ���� 0x2F 	   ���Ϊ case ֵ 
//               key=8;   //������ 5 ͨ�������� 
//			   //led=0; 
//               break;

//          case 0x177:  //����0x140 ���� 0x37  	   ���Ϊ case ֵ 
//               key=9;  //���� 4 ͨ��������

//               break;

//          case 0x17B:  //����0x140 ���� 0x3B		   ���Ϊ case ֵ 
//               key=10; //���� 3 ͨ��������
//               break;
// 	
//		  case 0x17D:  //����0x140 ���� 0x3D		   ���Ϊ case ֵ 
//               key=11; //���� 2 ͨ��������
//               break;

//		  case 0x17E:  //����0x140 ���� 0x3E		   ���Ϊ case ֵ 
//               key=12; //���� 1 ͨ��������               
               
//		       break;
			
			
			
	 ////////////////////������������ذ�������ɨ����////////////////////////

		      case 0x19F :      //����0x180  ����Ϊ 0X1F	   ���Ϊ case ֵ 
              key=13;
			         // key=14;             //̽ѨW ͨ��������	   KYO 6
			         printf("ɨ�赽������ ̽Ѩ 0X19F\r\n");
			    
					return key;
					
               break;		  	

          case 0x1BB :   //����0x180 ���� 0x2F 	    ���Ϊ case ֵ 
              key=14;   // ͨ��D ������ 		KYO 5
					
					    // key=13;
			         printf("ɨ�赽������ ͨ��D 0X1BB\r\n");
					return key;
					
               break;

          case 0x1AF  :  //����0x180 ���� 0x37  	    ���Ϊ case ֵ 
              // key=15;  // ���� B  ������	   KYO 4
					     key=16;
               printf("ɨ�赽������ ���� 0X1AF\r\n");
					return key;
               break;

          case 0x1BD :  //����0x180 ���� 0x3B	    ���Ϊ case ֵ 
             //  key=16;   //�������� �� �� ������		KYO 3
					
					     key=15;
					     printf("ɨ�赽������ ���� �� 0X1BD\r\n");
					return key;
					
               break;
 	
		      case 0x1BE:  //����0x180 ���� 0x3D		���Ϊ case ֵ 
						
              // key=17; //�������� �� �� ������		KYO 2
					      
					    key=18;
					return key;
					
			         printf("ɨ�赽������ ������  0X1B7\r\n");
			
               break;

		      case 0x1B7 :  //����0x180 ���� 0x3E	    ���Ϊ case ֵ 
              // key=18;  //ʱ�� ������      KYO 1   
			
					     key=17;
					
			         printf("ɨ�赽������  ʱ��  0X1BE\r\n");
			   return key;
               break;
		
			
			
			
			
		}	
			
		}
		
	}
	
  return 0;// �ް�������
	
}




u8 KEY_Scan_dian(void)
{

u8 cord_buf=0,y=0,i=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;	
	
	//SELET_IO_OUT();
	//SELET_DQ_OUT=0;	 //���������й������õͣ������롣
	
  HOT_IO_OUT();
	HOT_DQ_OUT=1;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=0;	 //���ε�����������ӣ�������øߣ����Ե��������롣
		
	SET_IO_OUT();
	SET_DQ_OUT=1;
	
	
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	delay_ms(5);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
	
	
	
 k6=KOY6_DQ_IN;
 k5=KOY5_DQ_IN;
 k4=KOY4_DQ_IN;
 k3=KOY3_DQ_IN;
 k2=KOY2_DQ_IN;
 k1=KOY1_DQ_IN;
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	
	tong_start=ELCK_unm; //��ͨ�������������Ϣ���ݳ�ȥ  
	
	
	
	///////////////////�����Աȼ������ݶ���ͬ������������������ݣ�����ͬ��ɾ��/�����ⷢ�ͳ������е����//////////////////////
	 ELCK_UF[ELCK_unm_i]=ELCK_unm;
	 ELCK_unm_i++;
	 
	 if(ELCK_unm_i>=2){
		 ELCK_unm_i=0;
		 if((ELCK_UF[0]==ELCK_unm)&&(ELCK_UF[1]==ELCK_unm))
		 {
			 ELCK_LED[7]=ELCK_unm;
			 
			 
			if( ELCK_LED[7]==0x3f)
			 {
				system_start_EL=1; 
				 
			 }
			 
			 
		 }
		 
		
		 
		 
		 
		 
		 fa_set_elect();
		 
		 
		 
		 
	 }
	 
	 
	
	
	
	
//	//��ɨ�赽�е�����ѡ��任��ʱ��ŷ���һ�θ�����Ļ��Ϣָ����ٲ���Ҫ�ķ��Ͳ�������ʾCPUִ��Ч�� 
//	if(ELCK_unm_X!=ELCK_unm){
//    ELCK_unm_X=ELCK_unm;
//		
//		ELCK_LED[7]=ELCK_unm;
//		
//   	for(j=0;j<5;j++){
//			 for(i=0;i<8;i++)
//					{
//						
//						USART_SendData(USART2,ELCK_LED[i]);
//						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//						
//					}
//					} 
		
		//printf("���巢�Ϳ����� ��%d \r\n",ELCK_unm );			
					
		
	
	
////�ڻ����������������ʱ��������ʱ�䵽����������һ������ʱ��
//	
//	if(k6) feng_6=1; else feng_6=0;
//	if(k5) feng_5=1; else feng_5=0;
//	if(k4) feng_4=1; else feng_4=0;
//	if(k3) feng_3=1; else feng_3=0;
//	if(k2) feng_2=1; else feng_2=0;
//	if(k1) feng_1=1; else feng_1=0;
	
	if(tong_start!=0x3f){
		
		return 1;
	}else
 {
		return 0;
		
	}
	
	
	
	
}


//���������������źţ�����������⵽��ťû���㣬�������У�������Ļ������ͼ��δ�����û���֪����һ·û�е�����



u8 Start_KEY_Scan_dian(void)
{

u8 cord_buf=0,y=0,i=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;	
	
	//SELET_IO_OUT();
	//SELET_DQ_OUT=0;	 //���������й������õͣ������롣
	
  HOT_IO_OUT();
	HOT_DQ_OUT=1;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=0;	 //���ε�����������ӣ�������øߣ����Ե��������롣
		
	SET_IO_OUT();
	SET_DQ_OUT=1;
	
	
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	delay_ms(100);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
	
	
	
 k6=KOY6_DQ_IN;
 k5=KOY5_DQ_IN;
 k4=KOY4_DQ_IN;
 k3=KOY3_DQ_IN;
 k2=KOY2_DQ_IN;
 k1=KOY1_DQ_IN;
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	
	tong_start=ELCK_unm; //��ͨ�������������Ϣ���ݳ�ȥ  
	
	//��ɨ�赽�е�����ѡ��任��ʱ��ŷ���һ�θ�����Ļ��Ϣָ����ٲ���Ҫ�ķ��Ͳ�������ʾCPUִ��Ч�� 
	if(ELCK_unm_X!=ELCK_unm){
    ELCK_unm_X=ELCK_unm;
		
		ELCK_LED[7]=ELCK_unm;
		
   	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,ELCK_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
		
		//printf("���巢�Ϳ����� ��%d \r\n",ELCK_unm );			
					
	}		
	
	
//�ڻ����������������ʱ��������ʱ�䵽����������һ������ʱ��
	

	
	if(tong_start!=0x3f){
		
		return 1;
	}else
 {
		return 0;
		
	}
	
	delay_ms(500);
	
	
}






//�������忪��״̬��Ϣ���͵���ʾ����
void fa_set_elect(void)
	{
	u8 i,j,k;
	
	  	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,ELCK_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	
	
}

 ///////////////ȡһ�μ��̰���������ֵ,��8λ������ û����ʱ���ص����� 0X3F  ////////////////////////////
 int key_red(void)
 {
  int cord=0;
  char cord_buf=0,y=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	 
   y=0;
   k=0;
   k6=KOY6_DQ_IN;
   k5=KOY5_DQ_IN;
   k4=KOY4_DQ_IN;
   k3=KOY3_DQ_IN;
   k2=KOY2_DQ_IN;
   k1=KOY1_DQ_IN;
	 
   //k1=1;
	 
	 
   k6=(k6<<5);
   k5=(k5<<4);
   k4=(k4<<3);
   k3=(k3<<2);
   k2=(k2<<1);
  
   y=(k6+k5+k4+k3+k2+k1);
  // if(y==0x3f) led=0;

   cord=y;
   //if(cord==0x1C0){led=0;}


   return cord;

 }

//�õ���λ�����롣
  int key_red_H(void)
{

   int cord=0;
   u8 cord_buf=0,y=0,j=0,k=0;
   
   y=HOT_DQ_IN;  //��������     ������0X
   y=(y<<2);    
   //j=SELET_DQ_IN; //����������
	 j=1;
   j=(j<<1);
   k=SET_DQ_IN;   //���ð�������
   y=(y+j+k);
   cord=y;
   cord=(cord<<6); 	 
   

   return cord;
	
}



u8 key_start(u8 i)
{
	//ɨ�����룬��������Ϊ����ģʽ������Ϊ�������0��
	
	
	if(i==1){
		
	SET_IO_OUT();
	SET_DQ_OUT=1;	
		
	HOT_IO_OUT();
	HOT_DQ_OUT=1;
		
	delay_ms(20);	
		
	HOT_IO_IN();
	//SELET_IO_IN(); 	
	SET_IO_IN();	
		
		
		
		
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=0;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=0;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=0;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=0;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=0;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=0;

	}
	
	if(i==2){
		
	//	printf("ɨ�赽��GAOGAOGAO0XDF\r\n");
		
		
	HOT_IO_OUT();
	HOT_DQ_OUT=0;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=1;	 //���ε�����������ӣ�������øߣ����Ե��������롣
	//SELET_DQ_OUT=0;

		
	SET_IO_OUT();
	SET_DQ_OUT=0;
	
		
		

	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	//delay_ms(20);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
		
	}
	
	
	
	
	
}




int key_set(void)
{
	
	  u8 i,j,task1_num=0;
	  u8 key=0; 
	


	
	// key=KEY_Scan();
		
//		if(key){
//		
//   printf("�����ļ�����ֵ ��%d",key );

//	}
	
	key=get_set_num();
	
// printf("�����ļ�����ֵ ��%d \r\n",key );
	
//	clos_bis  �رձ�����ⰴť
	
		//����ͨ��1��ͨ��6��������Ϣ����
	if(key==20){
	
	clos_bis=!clos_bis;  //�������·��·�����Ʊ�����������
		
   TIM_Cmd(TIM3, DISABLE);  //�رն�ʱ�����ж� 
	 delay_ms(200);
		
		if(clos_bis){
			
			JS_LED[7]=0x01;
			
			clos_bis_unm=1;
		}else {
			
			JS_LED[7]=0x00;
			
			clos_bis_unm=0;
			
			/////////�жϿ��Ʊ���6·///////////////
			ch1_chexe=0; 
			ch2_chexe=0;
			ch3_chexe=0;
			ch4_chexe=0;
			ch5_chexe=0;
			ch6_chexe=0;
			
		}
		
		// printf("����������ֵ ��%d,%d, \r\n",clos_bis,clos_bis_unm);
		
				for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,JS_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
		TIM_Cmd(TIM3, ENABLE);  //������ʱ�����ж�
			
					
					
	}

	//����ͨ��1��ͨ��6��������Ϣ����
	if(key==19){
	
		zhi_unm_date=6; //����������ʾ��������
		save_ch1_6_24c02(); //�����ڵ�ͨ��1��ͨ��6����Ϣ���浽AT24C02оƬ�� ��               
		
		
	}

	//̽Ѩ����
	if(key==13){
		elect_time=7;
		
		
		tang_bool=~tang_bool;
	
		
		if(tang_bool){
			
			XUE_LED[7]=0x01;
			
		}else {
			
			XUE_LED[7]=0x00;
			
		}
		
		for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,XUE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
		key_umb=key_staty;		//���¸���ɨ��ʱ��		
		
	}
	
	
	//ͨ��
	if(key==14){
		elect_time=7;	
			set_tong++;
			if(set_tong>=7)
			set_tong = 1;
			
			wendu[7]=set_tong;
			
		//	kuang_1[7]
			
		//	USART_SendData(USART2,0xA5);
		//	USART_SendData(USART2,0xA5);
			
				for(j=0;j<5;j++){    //�������õ�ͨ��ֵ�ı���˵���������õ�����һ��ͨ�� ����7�������½ǵ�1����2����3����4����5����6 
				 for(i=0;i<9;i++)    //ӦΪ�տ�ʼ��ʱ��������Ļ����û�仯���Զ෢�ͼ���
						{
							
					    USART_SendData(USART2,wendu[i]);
					    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
							
						}
		//			printf("�����ļ�����ֵΪ��%d",wendu[7]);
				} 
				
        delay_ms(20);
				
//				if(set_tong==1) kuang_1[7]=1; else kuang_1[7]=2 ;   
//        if(set_tong==2) kuang_2[7]=1; else kuang_2[7]=2 ;   //1��ʾ������ͼ��  2 ����ʾ������ͼ��
//        if(set_tong==3) kuang_3[7]=1; else kuang_3[7]=2 ;   //1��ʾ������ͼ��  2 ����ʾ������ͼ��
//        if(set_tong==4) kuang_4[7]=1; else kuang_4[7]=2 ;   //1��ʾ������ͼ��  2 ����ʾ������ͼ��
//        if(set_tong==5) kuang_5[7]=1; else kuang_5[7]=2 ;   //1��ʾ������ͼ��  2 ����ʾ������ͼ��
//        if(set_tong==6) kuang_6[7]=1; else kuang_6[7]=2 ;   //1��ʾ������ͼ��  2 ����ʾ������ͼ��
//				
			
			//�ٷ����Ǹ��ƿ����һ������ʾ

				if(set_tong==1){
					
					kuang_1[7]=1;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==2){
					
					kuang_1[7]=2;
					kuang_2[7]=1;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==3){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=1;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==4){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=1;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
					if(set_tong==5){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=1;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==6){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=1;
					
				}
				
				        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_1[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				
					        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_2[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
						        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_3[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				    delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_4[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				
					        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_5[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
						        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_6[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}

								
 key_umb=key_staty;		//���¸���ɨ��ʱ��										
								
return 0;
	goto loop;
				
						
						
		}
		
		
		
		
	
	if(key==15){
		elect_time=7;
		
	  
		
				// ����1ͨ��  ����ѭ������  
				if(set_tong==1){
					
					if(tongdao_1.bo_xin >=6) tongdao_1.bo_xin=1;
					else{
					 tongdao_1.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_1.bo_xin );
					
				boxin_1[7]=tongdao_1.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
					
				}
		
	// ����2ͨ��  ����ѭ������  
				if(set_tong==2){
					
					if(tongdao_2.bo_xin >=6) tongdao_2.bo_xin=1;
					else{
					 tongdao_2.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_2.bo_xin );
					
				boxin_2[7]=tongdao_2.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	
	// ����3ͨ��  ����ѭ������  
				if(set_tong==3){
					
					if(tongdao_3.bo_xin >=6) tongdao_3.bo_xin=1;
					else{
					 tongdao_3.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_2.bo_xin );
					
				boxin_3[7]=tongdao_3.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}

	// ����4ͨ��  ����ѭ������  
				if(set_tong==4){
					
					if(tongdao_4.bo_xin >=6) tongdao_4.bo_xin=1;
					else{
					 tongdao_4.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_2.bo_xin );
					
				boxin_4[7]=tongdao_4.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	// ����5ͨ��  ����ѭ������  
				if(set_tong==5){
					
					if(tongdao_5.bo_xin >=6) tongdao_5.bo_xin=1;
					else{
					 tongdao_5.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_2.bo_xin );
					
				boxin_5[7]=tongdao_5.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}

	// ����6ͨ��  ����ѭ������  
				if(set_tong==6){
					
					if(tongdao_6.bo_xin >=6) tongdao_6.bo_xin=1;
					else{
					 tongdao_6.bo_xin++;					
						
					}
					
				//	printf("���������״ ��%d",tongdao_2.bo_xin );
					
				boxin_6[7]=tongdao_6.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}




 key_umb=key_staty;		//���¸���ɨ��ʱ��		

	set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ			
				
				
return 0;
goto loop;




				
	}
	
	
	
	
	
	
//�����������
	
	if(key==16){
		
		//����1ͨ�� �������¼�����
		elect_time=7;
				if(set_tong==1){
				if(tongdao_1.zhou_time<=2) tongdao_1.zhou_time=2;
				else{
         tongdao_1.zhou_time--;					
					
				}
					
				zhou_1[7]=tongdao_1.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
				
			//����2ͨ�� �������¼�����
		
			if(set_tong==2){
				if(tongdao_2.zhou_time<=2) tongdao_2.zhou_time=2;
				else{
         tongdao_2.zhou_time--;					
					
				}
					
				zhou_2[7]=tongdao_2.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
		//����3ͨ�� �������¼�����
		
			if(set_tong==3){
				if(tongdao_3.zhou_time<=2) tongdao_3.zhou_time=2;
				else{
         tongdao_3.zhou_time--;					
					
				}
					
				zhou_3[7]=tongdao_3.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
			
				//����3ͨ�� �������¼�����
		
			if(set_tong==4){
				if(tongdao_4.zhou_time<=2) tongdao_4.zhou_time=2;
				else{
         tongdao_4.zhou_time--;					
					
				}
					
				zhou_4[7]=tongdao_4.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
					
		//����4ͨ�� �������¼�����
		
			if(set_tong==5){
				if(tongdao_5.zhou_time<=2) tongdao_5.zhou_time=2;
				else{
         tongdao_5.zhou_time--;					
					
				}
					
				zhou_5[7]=tongdao_5.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
					
		//����6ͨ�� �������¼�����
		
			if(set_tong==6){
				if(tongdao_6.zhou_time<=2) tongdao_6.zhou_time=2;
				else{
         tongdao_6.zhou_time--;					
					
				}
					
				zhou_6[7]=tongdao_6.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
			
	set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ		
		
	 key_umb=key_staty;		//���¸���ɨ��ʱ��					
				
				return 0;
				goto loop;
				
				
	}
	
	
	
	//����������ڲ������ڼ�
	
	if(key==17){
		
		elect_time=7;
		//����ͨ��1 ���ڼ� ����  
				if(set_tong==1){
				if(tongdao_1.zhou_time >=199) tongdao_1.zhou_time=199;
				else{
         tongdao_1.zhou_time++;					
					
				}
					
				zhou_1[7]=tongdao_1.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	//����ͨ��2 ���ڼ� ����  
				if(set_tong==2){
				if(tongdao_2.zhou_time >=199) tongdao_2.zhou_time=199;
				else{
         tongdao_2.zhou_time++;					
					
				}
					
				zhou_2[7]=tongdao_2.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}	
	//����ͨ��3 ���ڼ� ����  
				if(set_tong==3){
				if(tongdao_3.zhou_time >=199) tongdao_3.zhou_time=199;
				else{
         tongdao_3.zhou_time++;					
					
				}
					
				zhou_3[7]=tongdao_3.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}				
		
			//����ͨ��4 ���ڼ� ����  
				if(set_tong==4){
				if(tongdao_4.zhou_time >=199) tongdao_4.zhou_time=199;
				else{
         tongdao_4.zhou_time++;					
					
				}
					
				zhou_4[7]=tongdao_4.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}	
	//����ͨ��5 ���ڼ� ����  
				if(set_tong==5){
				if(tongdao_5.zhou_time >=199) tongdao_5.zhou_time=199;
				else{
         tongdao_5.zhou_time++;					
					
				}
					
				zhou_5[7]=tongdao_5.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
				
	//����ͨ��6 ���ڼ� ����  
				if(set_tong==6){
				if(tongdao_6.zhou_time >=199) tongdao_6.zhou_time=199;
				else{
         tongdao_6.zhou_time++;					
					
				}
					
				zhou_6[7]=tongdao_6.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
				
	set_ch1_6_sinc(); //����Ļ����ͨ����Ϣͬ����ϵͳ���������������Ϣ
				
	 key_umb=key_staty;		//���¸���ɨ��ʱ��					
return 0;			
goto loop;
				
	}
	
	
	
	
	
	
	
	
 
	//������ڼ�++
	if(key==18){
	elect_time=7;
	//����ͨ��1������ʱ��
		if(set_tong==1){
			
			if(tongdao_1.set_time <=10) tongdao_1.set_time=65;
			
			tongdao_1.set_time=(tongdao_1.set_time-5);
			

			
			time_1[7]=tongdao_1.set_time;
			tongdao_1.shen_time = tongdao_1.set_time;
			
			ch1_start=2;  //�򿪵������������
			
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_1.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
	//����ͨ��2��ʱ��
	if(set_tong==2){
			
			if(tongdao_2.set_time <=10) tongdao_2.set_time=65;
			
			tongdao_2.set_time=(tongdao_2.set_time-5);
			

			
			time_2[7]=tongdao_2.set_time;
		  tongdao_2.shen_time = tongdao_2.set_time;	
		  ch2_start=2;  //�򿪵������������
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_2.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
		//����ͨ��3��ʱ��
	if(set_tong==3){
			
			if(tongdao_3.set_time <=10) tongdao_3.set_time=65;
			
			tongdao_3.set_time=(tongdao_3.set_time-5);
			

			
			time_3[7]=tongdao_3.set_time;
		tongdao_3.shen_time = tongdao_3.set_time;
			ch3_start=2;  //�򿪵������������
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_3.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
		//����ͨ��3��ʱ��
	if(set_tong==4){
			
			if(tongdao_4.set_time <=10) tongdao_4.set_time=65;
			
			tongdao_4.set_time=(tongdao_4.set_time-5);
			

			
			time_4[7]=tongdao_4.set_time;
		tongdao_4.shen_time = tongdao_4.set_time;
			ch4_start=2;  //�򿪵������������
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_4.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
			//����ͨ��5��ʱ��
	if(set_tong==5){
			
			if(tongdao_5.set_time <=10) tongdao_5.set_time=65;
			
			tongdao_5.set_time=(tongdao_5.set_time-5);
			

			
			time_5[7]=tongdao_5.set_time;
			tongdao_5.shen_time = tongdao_5.set_time;
		ch5_start=2;  //�򿪵������������
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_5.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
			//����ͨ��6��ʱ��
	if(set_tong==6){
			
			if(tongdao_6.set_time <=10) tongdao_6.set_time=65;
			
			tongdao_6.set_time=(tongdao_6.set_time-5);
			

			
			time_6[7]=tongdao_6.set_time;
			tongdao_6.shen_time = tongdao_6.set_time;
		  ch6_start=2;  //�򿪵������������
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_6.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}	
	
		
	key_umb=key_staty;		//���¸���ɨ��ʱ��			
	return 0;	
	goto loop;	
		
		
}















	
 //����ʱ��

  key=get_hot1_6_num();

 if(key==6)
 {
	 elect_time=3;
	 
	 H1_BOOl=!H1_BOOl;
	 
	 if(H1_BOOl==1){ MCU_H1=1; H1_um=1;     }
	 if(H1_BOOl==0){ MCU_H1=0;  H1_um=0;    }
	 
  // printf("���Ȱ���1ͨ��  ��%d",H1_BOOl);
	 H1_um=(H1_um<<5);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
		//	printf("ͨ��1����ʱ�� ��%d",tongdao_1.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
	 
	key_umb=key_staty;		//���¸���ɨ��ʱ��			
	return 0;	
	goto loop;	
		
	 
	}		
	
	if(key==5)
 {
	 elect_time=3;
	 H2_BOOl=!H2_BOOl;
	 
	 if(H2_BOOl==1){ MCU_H2=1;  H2_um=1;       }
	 if(H2_BOOl==0){ MCU_H2=0;  H2_um=0;       }
	 
	 H2_um=(H2_um<<4);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 		 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
	 
	 
	 	key_umb=key_staty;		//���¸���ɨ��ʱ��			
	return 0;	
	goto loop;	
		
					
	}
 
	if(key==4)
 {
	 elect_time=3;
	 H3_BOOl=!H3_BOOl;
	 
	 if(H3_BOOl==1) {MCU_H3=1;   H3_um=1;    }
	 if(H3_BOOl==0) {MCU_H3=0;   H3_um=0;    }
	 
	 
	  H3_um=(H3_um<<3);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 
	 		 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
 	 key_umb=key_staty;		//���¸���ɨ��ʱ��			
	 return 0;	
	 goto loop;	
		
	 
	}
	
	if(key==3)
 {
	 elect_time=3;
	 
	 H4_BOOl=!H4_BOOl;
	 
	 if(H4_BOOl==1){ MCU_H4=1;   H4_um=1;     }
	 if(H4_BOOl==0){ MCU_H4=0;   H4_um=0;     }
	 
	 H4_um=(H4_um<<2);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 	
	 
	  key_umb=key_staty;		//���¸���ɨ��ʱ��			
	  return 0;	
	  goto loop;	
		
	 
	}
	
	if(key==2)
 {
	 
	 elect_time=3;
	 H5_BOOl=!H5_BOOl;
	 
	 if(H5_BOOl==1) {MCU_H5=1; H5_um=1;     }
	 if(H5_BOOl==0) {MCU_H5=0; H5_um=0;    }
	 
	 H5_um=(H5_um<<1);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	  	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	 
	 
	 
 	 key_umb=key_staty;		//���¸���ɨ��ʱ��			
	 return 0;	
	 goto loop;	
		
	 
	}
	
	 if(key==1)
 {
	 elect_time=3;

	 H6_BOOl=!H6_BOOl;
	 
	 if(H6_BOOl==1){ MCU_H6=1; H6_um=1;     }
	 if(H6_BOOl==0){ MCU_H6=0; H6_um=0;     }
	 
	  HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	   	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	 
	 
	 
  // printf("���Ȱ���6ͨ��  ��%d",H6_BOOl);

					
 key_umb=key_staty;		//���¸���ɨ��ʱ��	
 return 0;						
 goto loop;
	 
	}
	
	
	
  
//	if(key_umb<=1){
//	                  
//	KEY_Scan_dian();//û�������������º��ɨ���λ�����ذ�����
//  key_umb=1;
//		
//	}
//	
//	key_umb--;
//	
	
	get_eleck1_6_num();   //���µ�������Ϣ����ʾ���ϡ��������忪�������ý���������Ʒ���
	
	
	
 loop:
	
return 0;
	
	
}

//���ϴδ洢��������Ϣ�������µ���Ļ����Ϊ������ʾ���ݡ�
void set_ch1_6_to_display(void)
{
	u8 i,j,k;
	
	time_1[7]=tongdao_1.set_time;
	time_2[7]=tongdao_2.set_time;
	time_3[7]=tongdao_3.set_time;
	time_4[7]=tongdao_4.set_time;
	time_5[7]=tongdao_5.set_time;
	time_6[7]=tongdao_6.set_time;
	
	
	zhou_1[7]=tongdao_1.zhou_time;
	zhou_2[7]=tongdao_2.zhou_time;
	zhou_3[7]=tongdao_3.zhou_time;
	zhou_4[7]=tongdao_4.zhou_time;
	zhou_5[7]=tongdao_5.zhou_time;
	zhou_6[7]=tongdao_6.zhou_time;
	
	boxin_1[7]=tongdao_1.bo_xin;
	boxin_2[7]=tongdao_2.bo_xin;
	boxin_3[7]=tongdao_3.bo_xin;
	boxin_4[7]=tongdao_4.bo_xin;
	boxin_5[7]=tongdao_5.bo_xin;
	boxin_6[7]=tongdao_6.bo_xin;
	
	///////////////////////////����ͨ��1��������Ϣ/////////////////////////////////////////////
		for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////����ͨ��2��������Ϣ/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
					
///////////////////////////����ͨ��3��������Ϣ/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 					
///////////////////////////����ͨ��4��������Ϣ/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////����ͨ��5��������Ϣ/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////����ͨ��6��������Ϣ/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
					
					
					
					
	
}












//�������ͨ��1��ͨ��6�����Ʋ�����������Ϣ�ļ�������Ϣ��
u8 get_set_num(void)
{
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0,i=0;
	u8 mode;
	mode=1; //����1֧��������������������0��֧��������������
	
	HOT_DQ_OUT=1;
	SELET_DQ_OUT=1;
	SET_DQ_OUT=0;
	delay_ms(10);
	
  k6=KOY6_DQ_IN;  //̽Ѩ
  k5=KOY5_DQ_IN;  //���ڼ�
  k4=KOY4_DQ_IN;  //���ڼ�
  k3=KOY3_DQ_IN;  //ͨ��
  k2=KOY2_DQ_IN;  //����
  k1=KOY1_DQ_IN;  //ʱ��
	
	if(mode)key_up=1;  //�Ƿ�֧��֧��������������
	
	if(key_up&&(k6==0||k5==0||k4==0||k3==0||k2==0||k1==0))
	{
		delay_ms(10);
		key_up=0;
		if(k6==0) {  key=13; delay_ms(200);   //�̰�����̽Ѩ������
     while(!KOY6_DQ_IN){
			delay_ms(200);
			i++; 
		 }
		 if(i>=8) {  key=19;  }   //����̽Ѩ2�����ϼ�⵽Ϊ�����������ݵĹ��ܡ�

		 if(i>=15) { key=20; i=0;}   //����̽Ѩ10������Ϊ�ر������·��·������ʾ����
		 
		return key ; }
		else if (k5==0){key=16;delay_ms(200); return key; }
		else if (k4==0){key=17;delay_ms(200); return key; }
		else if (k3==0){key=14;delay_ms(200); return key; }
		else if (k2==0){key=15;delay_ms(200); return key; }
		else if (k1==0){key=18;delay_ms(200); return key; }
		
	}else if(k6==1&&k5==1&&k4==1&&k3==1&&k2==1&&k1==1)  key_up=1;
	
	return 0;
	
	
	
}



//�������ͨ��1��ͨ��6�����뿪��������Ϣ�ļ�������Ϣ��
u8 get_hot1_6_num(void)
{
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	u8 mode;
	mode=0; //����1֧��������������������0��֧��������������                
        
	HOT_DQ_OUT=0;
	SELET_DQ_OUT=1;
	SET_DQ_OUT=1;
	delay_ms(10);
	
  k6=KOY6_DQ_IN; //����6
  k5=KOY5_DQ_IN; //����5
  k4=KOY4_DQ_IN; //����4
  k3=KOY3_DQ_IN; //����3
  k2=KOY2_DQ_IN; //����2
  k1=KOY1_DQ_IN; //����1
	
	if(mode)key_up_1=1;  //�Ƿ�֧��֧��������������
	
	if(key_up_1&&(k6==0||k5==0||k4==0||k3==0||k2==0||k1==0))
	{
		delay_ms(10);
		key_up_1=0;
		if(k6==0) {     key=1;while(!KOY6_DQ_IN);return key; }
		else if (k5==0){key=2;while(!KOY5_DQ_IN);return key; }
		else if (k4==0){key=3;while(!KOY4_DQ_IN);return key; }
		else if (k3==0){key=4;while(!KOY3_DQ_IN);return key; }
		else if (k2==0){key=5;while(!KOY2_DQ_IN);return key; }
		else if (k1==0){key=6;while(!KOY1_DQ_IN);return key; }
		
		delay_ms(200);
		
		
	}else if(k6==1&&k5==1&&k4==1&&k3==1&&k2==1&&k1==1)  key_up_1=1;
	
	
	
	return 0;
	
	
	
}

//�������ͨ��1��ͨ��6�ĵ�������Ϣ�ļ�������Ϣ��
u8 get_eleck1_6_num(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	u8 mode;
	mode=0;   //����1֧��������������������0��֧��������������                
 
	
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTA,PORTEʱ��
  //����ɨ�������ʼ������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    //���Ȱ������������������ ��һ�汾��PE0 ��ĳ�PE0 ; ���������������� ��һ�汾��PC13 ��ĳ�PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;          //GPIO_Mode_IPU; //���ó��������� ���Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //��ʼ��	

	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //���ü��̹��������λ ��һ�汾��PD9 ��2�汾�ĳ� PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���ó���������  ��ĳ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	
	
	
	//HOT_DQ_OUT=1;
	SELET_DQ_OUT=0;
	//SET_DQ_OUT=1;
	delay_ms(10);
	
	
	
	
  k6=KOY6_DQ_IN; //����6
  k5=KOY5_DQ_IN; //����5
  k4=KOY4_DQ_IN; //����4
  k3=KOY3_DQ_IN; //����3
  k2=KOY2_DQ_IN; //����2
  k1=KOY1_DQ_IN; //����1
	
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	

	
	
//	//�ж�U1_P�Ƿ�������ӣ������жϴ��ڷ����Ƿ����

 if(CK_ELE(ELCK_unm))      //����������û�и���
	{	
	    tong_start=ELCK_unm; //��ͨ�������������Ϣ���ݳ�ȥ  
		
		  printf("���µ��������ֵ\r\n");
		
			ELCK_LED[7]=ELCK_unm;		 
			if( ELCK_LED[7]==0x3f)
			{
				system_start_EL=1; 
				 
			 }
	
	 
		 fa_set_elect();
		 
		
		
	}
	
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    //���Ȱ������������������ ��һ�汾��PE0 ��ĳ�PE0 ; ���������������� ��һ�汾��PC13 ��ĳ�PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;          //GPIO_Mode_IPU; //���ó��������� ���Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //��ʼ��	
  GPIO_SetBits(GPIOE,GPIO_Pin_0);
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //���ü��̹��������λ ��һ�汾��PD9 ��2�汾�ĳ� PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó���������  ��ĳ��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	
	
	
//	
//	///////////////////�����Աȼ������ݶ���ͬ������������������ݣ�����ͬ��ɾ��/�����ⷢ�ͳ������е����//////////////////////
//	 ELCK_UF[ELCK_unm_i]=ELCK_unm;
//	 ELCK_unm_i++;
//	 
//	 if(ELCK_unm_i>=2){
//		 ELCK_unm_i=0;
//		 if((ELCK_UF[0]==ELCK_unm)&&(ELCK_UF[1]==ELCK_unm))
//		 {
//			 
//			 ELCK_LED[7]=ELCK_unm;		 
//			if( ELCK_LED[7]==0x3f)
//			 {
//				system_start_EL=1; 
//				 
//			 }
//			 
//			 
//		 }

//		 fa_set_elect();

//	 }
//	


	
	
	return 0;
	
	
	
}

//����������ݣ�����ֵ�����仯ʱ
u8 CK_ELE(u8 key)
{
	if(key_ele_unm!=key){
		
		key_ele_unm=key;
		return 1;
		
	}
	
	//key_ele_unm=0;
	return 0;
}






//�����������Ϣ���ڣ���������ʱ����Ϣ�Խ���

void set_ch1_6_sinc(void)
{
	//�Խ�������Ʋ���
	 set_ch1_bo=tongdao_1.bo_xin;
   set_ch2_bo=tongdao_2.bo_xin;
	 set_ch3_bo=tongdao_3.bo_xin;
	 set_ch4_bo=tongdao_4.bo_xin;
   set_ch5_bo=tongdao_5.bo_xin;
	 set_ch6_bo=tongdao_6.bo_xin;
	//�Խ������������
	ch1_unm=(tongdao_1.zhou_time*10);
	ch2_unm=(tongdao_2.zhou_time*10);
	ch3_unm=(tongdao_3.zhou_time*10);
	ch4_unm=(tongdao_4.zhou_time*10);
	ch5_unm=(tongdao_5.zhou_time*10);
	ch6_unm=(tongdao_6.zhou_time*10);
	
	
}


void set_ch1_sinc(void)
{

	ch1_unm=(tongdao_1.zhou_time*10);


}


void set_ch2_sinc(void)
{

	ch2_unm=(tongdao_2.zhou_time*10);


}

void set_ch3_sinc(void)
{

	ch3_unm=(tongdao_3.zhou_time*10);


}


void set_ch4_sinc(void)
{

	ch4_unm=(tongdao_4.zhou_time*10);


}



void set_ch5_sinc(void)
{

	ch5_unm=(tongdao_5.zhou_time*10);


}

void set_ch6_sinc(void)
{

	ch6_unm=(tongdao_6.zhou_time*10);


}




//��1ͨ����6ͨ��������Ϣ�洢��W25Q80��оƬ�ڲ�
void save_ch1_6_24c02(void)
{
	//��ͨ��1��ͨ��6��������Ϣ�洢��datch1_6unm[]�������档
	u8 datch1_6unm[19];
	
	datch1_6unm[0]=tongdao_1.set_time;
	datch1_6unm[1]=tongdao_1.zhou_time;
	datch1_6unm[2]=tongdao_1.bo_xin;
	
	datch1_6unm[3]=tongdao_2.set_time;
	datch1_6unm[4]=tongdao_2.zhou_time;
	datch1_6unm[5]=tongdao_3.bo_xin;
	
	datch1_6unm[6]=tongdao_3.set_time;
	datch1_6unm[7]=tongdao_3.zhou_time;
	datch1_6unm[8]=tongdao_3.bo_xin;
	
	datch1_6unm[9]=tongdao_4.set_time;
	datch1_6unm[10]=tongdao_4.zhou_time;
	datch1_6unm[11]=tongdao_4.bo_xin;
	
	datch1_6unm[12]=tongdao_5.set_time;
	datch1_6unm[13]=tongdao_5.zhou_time;
	datch1_6unm[14]=tongdao_5.bo_xin;
	
	datch1_6unm[15]=tongdao_6.set_time;
	datch1_6unm[16]=tongdao_6.zhou_time;
	datch1_6unm[17]=tongdao_6.bo_xin;

  datch1_6unm[18]=33;
	
	W25QXX_Write(datch1_6unm,100,19);
  delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);
	delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);
	delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);

	
}


//��1��6ͨ���������ݴ�W25Q80�ж�ȡ����
void read_ch1_6_at2402(void)
{
	u8 data_ch1_6_read[19];
	
	//AT24CXX_Read(0,data_ch1_6_read,19);
	
	W25QXX_Read(data_ch1_6_read,100,19);
	
	tongdao_1.set_time=data_ch1_6_read[0];
	tongdao_1.zhou_time=data_ch1_6_read[1];
	tongdao_1.bo_xin=data_ch1_6_read[2];
	
	tongdao_2.set_time=data_ch1_6_read[3];
	tongdao_2.zhou_time=data_ch1_6_read[4];
	tongdao_2.bo_xin=data_ch1_6_read[5];
	
	tongdao_3.set_time=data_ch1_6_read[6];
	tongdao_3.zhou_time=data_ch1_6_read[7];
	tongdao_3.bo_xin=data_ch1_6_read[8];
	
	tongdao_4.set_time=data_ch1_6_read[9];
	tongdao_4.zhou_time=data_ch1_6_read[10];
	tongdao_4.bo_xin=data_ch1_6_read[11];
	
	tongdao_5.set_time=data_ch1_6_read[12];
	tongdao_5.zhou_time=data_ch1_6_read[13];
	tongdao_5.bo_xin=data_ch1_6_read[14];
	
	tongdao_6.set_time=data_ch1_6_read[15];
	tongdao_6.zhou_time=data_ch1_6_read[16];
	tongdao_6.bo_xin=data_ch1_6_read[17];
	
	
	tongdao_1.shen_time = tongdao_1.set_time;
	tongdao_2.shen_time = tongdao_2.set_time;
	tongdao_3.shen_time = tongdao_3.set_time;
	tongdao_4.shen_time = tongdao_4.set_time;
	tongdao_5.shen_time = tongdao_5.set_time;
	tongdao_6.shen_time = tongdao_6.set_time;
	
	
	
	
}



//������ʱ���ʣ������ʱ�䷢������Ļ��
void disply_time(void)
{
	u8 tong_S=0,i=0;
	tong_S=tong_start;
	

	
	if(!(tong_S&0x20))
	{

		
   if(tongdao_1.shen_time>=1){
	
		ch1_start=2;
		if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_1.shen_time--;  
		}
	 }else {    
	 
		tan_xue_boo=2;
		 
	 ch1_start=0;
	 
	 }
	 
	 time_1[7]=tongdao_1.shen_time;
	 
	 
		for(i=0;i<8;i++)
		{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}  
	 
	 
	 
	}
	
		if(!(tong_S&0x10))
	{
	
		
		
   if(tongdao_2.shen_time>=1){

		ch2_start=2;
		if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_2.shen_time--;  
		}
	 }else {    
	 
		
		 tan_xue_boo=2;
	 ch2_start=0;
	 
	 }
	 
	 time_2[7]=tongdao_2.shen_time;
	 
	  			  for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
	 
	 
	}
		if(!(tong_S&0x08))
	{
		
	
   if(tongdao_3.shen_time>=1){
			ch3_start=2;
		 	if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_3.shen_time--; 
			}
	 }else {    
	 
		
		 
	 ch3_start=0;
	 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 
	 time_3[7]=tongdao_3.shen_time;
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_3[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	if(!(tong_S&0x04))
	{
		
		
		
	
   if(tongdao_4.shen_time>=1){
		ch4_start=2;
	if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_4.shen_time--; 
		}
	 }else {    
	 
		
		 
	 ch4_start=0;
	 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 time_4[7]=tongdao_4.shen_time;
	 
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_4[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	if(!(tong_S&0x02))
	{
	
		
		

   if(tongdao_5.shen_time>=1){
	  ch5_start=2;
	if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_5.shen_time--; 
			}
	 }else{
		 
		 
		 ch5_start=0;
	 tan_xue_boo=2;
		 
	 }
	 
	 time_5[7]=tongdao_5.shen_time;
	 
	 		 				 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
	 
	 
	 
	}
	
	if(!(tong_S&0x01))
	{
	  
		
		

   if(tongdao_6.shen_time>=1){
		ch5_start=2;
	if(system_start_EL){   //����������ʣ��ʱ�䲻��С
		tongdao_6.shen_time--;
	}
	
	 }else{
		 
		 ch6_start=0;
		 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 time_6[7]=tongdao_6.shen_time;
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_6[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	
}

