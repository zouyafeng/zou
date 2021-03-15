#include "sys.h"
#include "usart.h"	  
#include "delay.h"
#include "rtc.h" 
#include "led.h"
#include "usart3.h"	
#include "ds18b20.h" 


////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 
//�жϻ��洮������
uint8_t U2_P = 0;
//uint8_t U2_P_x = 0;
extern u8  seve_biaoding; //�Ƿ���ע����Ϣ����������־  

unsigned char U2BUF[256]={0};
uint8_t LAST_CHAR_U2=0;

unsigned char U2_QHTPRADE[1024]={0};
uint16_t U2_QHTTPREAD_P = 0;

u8 U2_DATA_flag=0;//���ݱ���λ���л�

uint8_t U2_P_RC=0;		//U2_P��¼
unsigned char *gps;
u8 sever_time=0;

extern u8 time_num_low;

extern u16 work_REST;

extern u16 th_num; //�������

u8 houre=0;
u8 feng=0;
u8 miao=0;

u8 bool_SHU=0;

u8 LED1_EN, LED2_EN, LED3_EN, LED4_EN;

extern u16 LED_PWOR; //����Ƶ����ȱ���
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}
//����2��ʼ������
void usart2_init(u32 bound){
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//�������ŷ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //����2ʱ��ʹ��
	
	//���ô��� Tx (PA.02) Ϊ�����������
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//���ڷ�������
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//�����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//Ƶ��50MHz
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//��ʼ������
    
	// ���ô��� Rx (PA.03) Ϊ��������
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;					//���ڽ�������
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//��ʼ������

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//�����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//�򿪴����ж�
	NVIC_Init(&NVIC_InitStructure);								//��ʼ���ж�������
	
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;    //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;     //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 //�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);	//��ʼ������
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	
	
}





void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(USART_RX_STA<255){
			
		USART_RX_BUF[USART_RX_STA]=Res;
		
		//USART3_SendByte(USART_RX_BUF[USART_RX_STA]);
		USART_RX_STA++;
			
		}
			
     } 
#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
#endif
		 
} 
#endif	




void clean_rebuff1(void)
{
	uint8_t i;
	for(i=0;i<255;i++){
		USART_RX_BUF[i]=0x00;
	}
	USART_RX_BUF[255]=0x00;
  USART_RX_STA = 0;
}


//��ȡ���յ������ݺͳ���
unsigned char *get_rebuff1(void)
{
   if(strstr(USART_RX_BUF,"$GNRMC")!=NULL)
   return (unsigned char *)strstr(USART_RX_BUF,"$GNRMC");
	 if(strstr(USART_RX_BUF,"$GPRMC")!=NULL)
	 return (unsigned char *)strstr(USART_RX_BUF,"$GPRMC");
	 if(strstr(USART_RX_BUF,"$GARMC")!=NULL)
	 return (unsigned char *)strstr(USART_RX_BUF,"$GARMC");
	 if(strstr(USART_RX_BUF,"$GBRMC")!=NULL)
	 return (unsigned char *)strstr(USART_RX_BUF,"$GBRMC");
	 if(strstr(USART_RX_BUF,"$GLRMC")!=NULL)
	 return (unsigned char *)strstr(USART_RX_BUF,"$GLRMC");

}















/****************************************************************************
* ��	�ƣ�void USART2_SendString(u8* Data,u32 Len)
* ��	�ܣ����ַ����
* ��ڲ�����Data 	����ĵ��ַ�����
			Len		�ַ�����
* ���ڲ�������
* ˵	������					 
****************************************************************************/
void USART2_SendString(u8* Data,u32 Len)		   //���ַ����
{
	u32 i=0;
	for(i=0;i<Len;i++)
  {    
		USART_SendData(USART2, Data[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

int USART2_DMASS(u8* SrData,u8* DsData,uint16_t BeTime,uint16_t AfTime)
{

		int i=0;
		int RecLen=0;
		int x,y;
		u8 Ubyte=0;
		uint16_t aftime;
		
		clean_rebuff2();
	
		if(SrData!=NULL){
			USART2_SendString(SrData,strlen(SrData));
		}
//�ȴ�BeTime	
		if(DsData!=NULL){
			do{
				BeTime--;
			//	Delay_ms(1);
				delay_ms(1);
			//	IWDG_ReloadCounter();	//ι��
			}while((strstr(U2BUF,DsData)==NULL) && (BeTime>0));
		}
//��⴮������	
		aftime=AfTime;
		do{
			
		  delay_ms(1);
			if(!CK_U2P()){   //��
				aftime=AfTime;
			}
		
			aftime--;								
		//	IWDG_ReloadCounter();	//ι��
		}while(aftime>0);//ע�����aftime=0ʱLookUSART2_GetBuffCount()=1���������
					
	//	USART2_RX_Buffer_Clear();
			
//ͨ������1��ʾ				
//			U2BUF[U2_P]='\0';
//			for(i=0;i<U2_P;i++){
//				DebugPf("%c",U2BUF[i]);
//			}
//			DebugPf("\r\n");
			
		return U2_P;	
		
}

//��ȡ���յ������ݺͳ���
unsigned char *get_rebuff2(uint8_t *len)
{
    *len = U2_P;
    return (unsigned char *)&U2BUF;
	
}

//�ж�U2_P�Ƿ�������ӣ������жϴ��ڷ����Ƿ����
uint8_t CK_U2P()
{
	if(U2_P_RC!=U2_P){
		
		U2_P_RC=U2_P;
		return 0;
		
	}
//	U2_P_RC = 0;
	return 1;
}

void USART2_DMAS(u8* Data)
{
	while(*Data)
	{
		USART2_SendByte(*Data++);
	}
}

static uint8_t Check_HD_2(unsigned char *addr)
{

	if(addr[0]==0xA5 && addr[1]==0x5A&& addr[5]==0x05){
		return 0x02;  //�յ�ʵʱ���ݻظ�֡����
	}
	if(addr[0]==0xA5 && addr[1]==0x5A && addr[5]==0x01){
		return 0x01;
	}
	//clean_rebuff3();
	return 0x00;
}


static uint8_t BLE_Check_2(void)
{
	uint8_t lens[1],k,s;
	unsigned char *addr;
	uint16_t i,j;

	addr=get_rebuff2(lens);  //��������ָ����������ʱָ���������
	
	if(Check_HD_2(addr)==0x01)  //�ж�������ͷ�Ĵ��� 0xff 0x7e 
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
		
		if(Check_HD_2(addr)==0x02)  //�յ�ʵʱ����֡�ظ���Ϣ�����������Ʊ�������  
		{	
			work_REST=0;
			
		}
		
		
		addr=NULL;
		 return 0x00;//��ͷ��Ч���������μ�⣬��մ��ڻ��嵥Ԫ
		
	
	  

	

}







/****************************************************************************
* ��	�ƣ�void USART2_SendByte(u8 Data)
* ��	�ܣ����ַ�����
* ��ڲ�����Data 	���͵��ַ�����
* ���ڲ�������
* ˵	������				   
****************************************************************************/
void USART2_SendByte(u8 Data)		   //���ַ��������
{
	USART_SendData(USART2, Data);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


void USART2_IRQHandler(void)
{
u8 res;	    
#ifdef SYSTEM_SUPPORT_OS	 	
OSIntEnter();    
#endif
	
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		res= USART_ReceiveData(USART2);
		if(U2_P<255)
		{
			//USART3_SendByte(U2BUF[U2_P]);
			U2BUF[U2_P]=res;
			U2_P++;	
		}			

	}

#ifdef SYSTEM_SUPPORT_OS	 
OSIntExit();  											 
#endif
	
}





void clean_rebuff2(void)
{
	uint8_t i;
	for(i=0;i<255;i++){
		U2BUF[i]=0x00;
	}
	U2BUF[255]=0x00;
  U2_P = 0;
}

//��ȡ��ͷλ��ȡ�ַ���IMEE�ŵ�λ����Ϣ
//����λ��
uint8_t CK_NUM_I2()
{
	uint8_t i,j,k; 
		for(i=0;i<U2_P;i++){
		if((U2BUF[i]==0x2B) && (U2BUF[i+1]==0x43) && (U2BUF[i+2]==0x47)){	//֡ͷУ��
			return i;
			}
		}

	return 0xff;
}


/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/*
 * ��������USART2_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART2_printf(USART_TypeDef* USARTx, char *Data,...)
{
	const char *s;
  int d;   
  char buf[10];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;

			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}


uint8_t USART2_GET(void)
{
  uint8_t i,j,k,n=0,b,s ;
	uint8_t lens[1];
	uint8_t shu_a=0,shu_b=0,shu_g=0;
	uint8_t len=0;
	uint8_t rt=0,CSQ=0;
	uint16_t jeep;
	short temperature=0;

	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;
	
		unsigned char *addr;
	

	
	

	
	if(U2_P>0){
		
		USART2_DMAS("zou 111111111111111111111111111111\r\n");
		
		
  if(CK_U2P())  //���������û�н�������
	{	
		
		USART2_DMAS("zou 111111111111111111111111111111\r\n");
		
		
	if(BLE_Check()==0x02)	 //���BLE������,0x02Ϊ��鵽��������ȷ����
			{
				
				USART2_DMAS("zou ya feng ffffffffffffffffffff\r\n");
				
        s=Type_Get();
			//	USART2_printf( USART2, "\r\n read is  %d \r\n", s );
		//		USART3_SendByte(s);
			switch(s)
						{
						//�õ���ˮ��ָ���
				
						case 1 :
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ����ˮ�ö�ָ�� %d \r\n",th_num);	
	
            if(th_num==1)	{ PWOR_SHU=0; bool_SHU=1;    }
						if(th_num==0)	{PWOR_SHU=1;  bool_SHU=0;    }	

						}
					  break ;
						
						
						case 4 :
						{
						
          //  addr=get_rebuff3(lens);
						//	get_rebuff2
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n �������ˮ�ֶ�ָ�� %d \r\n",th_num);	
//            j=th_num&0x01;
//						if(j){ PWOR_YANG=0; } else PWOR_YANG=1;
							
						if(th_num==1)	PWOR_YANG=0;
						if(th_num==0)	PWOR_YANG=1;	
							
							
						}		 
					  break ;
						
						case 5 :
						{

          //  addr=get_rebuff3(lens);
						//	get_rebuff2
							
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n �����Ӫ��Һ�ֶ�ָ�� %d \r\n",th_num);	

//            j=th_num&0x01;
//						if(j){ PWOR_YINGYANG=0; } else PWOR_YINGYANG=1;
							
						if(th_num==1)	PWOR_YINGYANG=0;
						if(th_num==0)	PWOR_YINGYANG=1;	
							
							
						}		 
					  break ;						
						
						
						case 6 :
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n �򿪼��ȹ��ܣ������Һ����¶�  %d \r\n",th_num);						
						if(th_num==1)	PWOR_WENG=0;
						if(th_num==0)	PWOR_WENG=1;
						}		 
					  break ;						
							
						case 7:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ����Сʱ ʱ��   %d \r\n",th_num);
            if(th_num<=24&&th_num>0){
				    hour=th_num;
							
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,hour,calendar.min,calendar.sec);
						
						}
							
						}		 
					  break ;				
						
						case 8:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ����ʱ�����   %d \r\n",th_num);
            if(th_num<=60&&th_num>0){
						
				    feng=th_num;
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,feng,calendar.sec);
						
						}
							
						}		 
					  break ;
						
						case 9:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ����ʱ��������  %d \r\n",th_num);
            if(th_num<=60&&th_num>0){
						
				    miao=th_num;
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,miao);
						
						}
							
						}		 
					  break ;	
						
						
						case 10:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //������� 							
						USART2_printf( USART2, "\r\n ����ֲ�����������ȵ���  %d \r\n",th_num);
        
						LED_PWOR=th_num * 3;
							
						}		 
					  break ;	
						
						
		       //�õ�ʵʱ����֡���ݣ���ʼ����  			
						case  2 :
					{
					  //addr=get_rebuff3(lens);
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //�������      
						USART2_printf( USART2, "\r\n �չ�Ʋ����ֶ�ָ�� %d \r\n",th_num);	 
						j=th_num&0x08;
						if(j){ LED1_EN=1; } else LED1_EN=0;
            j=th_num&0x04;
						if(j){ LED2_EN=1; } else LED2_EN=0;						
						j=th_num&0x02;
						if(j){ LED3_EN=1; } else LED3_EN=0;
            j=th_num&0x01;
						if(j){ LED4_EN=1; } else LED4_EN=0;
							
					}
					  break;
					case  3 :
					{
					  //addr=get_rebuff3(lens);
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //�������      
						USART2_printf( USART2, "\r\n ���Ȳ����ֶ�ָ�� %d \r\n",th_num);	
						
						j=th_num&0x08;
						if(j){ feng_3=0; } else feng_3=1;
            j=th_num&0x04;
						if(j){ feng_4=0; } else feng_4=1;						
						j=th_num&0x02;
						if(j){ feng_1=0; } else feng_1=1;
            j=th_num&0x01;
						if(j){ feng_2=0; } else feng_2=1;
				
							
					}
					  break;
					
					default: USART2_printf( USART2, "\r\n û�в���ָ�� \r\n");
					
					
					}
						
					clean_rebuff2();	
					
				}
	
		
		clean_rebuff2();	
		

	}
	
	
	
}
	

}