#include "sys.h"
#include "usart.h"	  
#include "delay.h"
#include "rtc.h" 
#include "led.h"
#include "usart3.h"	
#include "ds18b20.h" 


////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 
//中断缓存串口数据
uint8_t U2_P = 0;
//uint8_t U2_P_x = 0;
extern u8  seve_biaoding; //是否发送注册信息到服务器标志  

unsigned char U2BUF[256]={0};
uint8_t LAST_CHAR_U2=0;

unsigned char U2_QHTPRADE[1024]={0};
uint16_t U2_QHTTPREAD_P = 0;

u8 U2_DATA_flag=0;//数据保存位置切换

uint8_t U2_P_RC=0;		//U2_P记录
unsigned char *gps;
u8 sever_time=0;

extern u8 time_num_low;

extern u16 work_REST;

extern u16 th_num; //塔吊编号

u8 houre=0;
u8 feng=0;
u8 miao=0;

u8 bool_SHU=0;

u8 LED1_EN, LED2_EN, LED3_EN, LED4_EN;

extern u16 LED_PWOR; //定义灯的亮度变量
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}
//串口2初始话函数
void usart2_init(u32 bound){
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //串口2时钟使能
	
	//配置串口 Tx (PA.02) 为复用推挽输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//串口发送引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化引脚
    
	// 配置串口 Rx (PA.03) 为浮空输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;					//串口接收引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化引脚

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开串口中断
	NVIC_Init(&NVIC_InitStructure);								//初始化中断向量表
	
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;    //一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;     //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 //收发模式
  USART_Init(USART2, &USART_InitStructure);	//初始化串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART2, ENABLE);                    //使能串口 
	
	
}





void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
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


//获取接收到的数据和长度
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
* 名	称：void USART2_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART2_SendString(u8* Data,u32 Len)		   //多字符输出
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
//等待BeTime	
		if(DsData!=NULL){
			do{
				BeTime--;
			//	Delay_ms(1);
				delay_ms(1);
			//	IWDG_ReloadCounter();	//喂狗
			}while((strstr(U2BUF,DsData)==NULL) && (BeTime>0));
		}
//检测串口数据	
		aftime=AfTime;
		do{
			
		  delay_ms(1);
			if(!CK_U2P()){   //在
				aftime=AfTime;
			}
		
			aftime--;								
		//	IWDG_ReloadCounter();	//喂狗
		}while(aftime>0);//注意避免aftime=0时LookUSART2_GetBuffCount()=1的情况出现
					
	//	USART2_RX_Buffer_Clear();
			
//通过串口1显示				
//			U2BUF[U2_P]='\0';
//			for(i=0;i<U2_P;i++){
//				DebugPf("%c",U2BUF[i]);
//			}
//			DebugPf("\r\n");
			
		return U2_P;	
		
}

//获取接收到的数据和长度
unsigned char *get_rebuff2(uint8_t *len)
{
    *len = U2_P;
    return (unsigned char *)&U2BUF;
	
}

//判断U2_P是否继续增加，用于判断串口发送是否完成
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
		return 0x02;  //收到实时数据回复帧数据
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

	addr=get_rebuff2(lens);  //返回数组指针给定义的临时指针便于数据
	
	if(Check_HD_2(addr)==0x01)  //判断命令字头的存在 0xff 0x7e 
		{	
			
//			// k=ASCII_u16(addr[3])-1; //来的数据位数
//			 k=addr[3]-1; //来的数据位数
//			 s=Q24_CheckSum(addr,k);
	//	   printf("du check_CK  is:%x\r\n",s);
	//		 printf("du check_CK  is:%x\r\n",addr[k]);
//			if(s==addr[k])
//			{
	//		printf("jiao yan cheng gong ",s);
				return 0x02;//校验正确
		//	}
			
			
		}
		
		if(Check_HD_2(addr)==0x02)  //收到实时数据帧回复消息，把重启机制变量回零  
		{	
			work_REST=0;
			
		}
		
		
		addr=NULL;
		 return 0x00;//字头无效，结束本次检测，清空串口缓冲单元
		
	
	  

	

}







/****************************************************************************
* 名	称：void USART2_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART2_SendByte(u8 Data)		   //单字符数据输出
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

//获取字头位置取字符串IMEE号的位置信息
//返回位置
uint8_t CK_NUM_I2()
{
	uint8_t i,j,k; 
		for(i=0;i<U2_P;i++){
		if((U2BUF[i]==0x2B) && (U2BUF[i+1]==0x43) && (U2BUF[i+2]==0x47)){	//帧头校验
			return i;
			}
		}

	return 0xff;
}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
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
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
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

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
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
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
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
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;
	
		unsigned char *addr;
	

	
	

	
	if(U2_P>0){
		
		USART2_DMAS("zou 111111111111111111111111111111\r\n");
		
		
  if(CK_U2P())  //检查数据有没有接受完整
	{	
		
		USART2_DMAS("zou 111111111111111111111111111111\r\n");
		
		
	if(BLE_Check()==0x02)	 //检查BLE端数据,0x02为检查到完整的正确数据
			{
				
				USART2_DMAS("zou ya feng ffffffffffffffffffff\r\n");
				
        s=Type_Get();
			//	USART2_printf( USART2, "\r\n read is  %d \r\n", s );
		//		USART3_SendByte(s);
			switch(s)
						{
						//得到开水泵指令后
				
						case 1 :
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开添水泵段指令 %d \r\n",th_num);	
	
            if(th_num==1)	{ PWOR_SHU=0; bool_SHU=1;    }
						if(th_num==0)	{PWOR_SHU=1;  bool_SHU=0;    }	

						}
					  break ;
						
						
						case 4 :
						{
						
          //  addr=get_rebuff3(lens);
						//	get_rebuff2
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开添加清水手段指令 %d \r\n",th_num);	
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
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开添加营养液手段指令 %d \r\n",th_num);	

//            j=th_num&0x01;
//						if(j){ PWOR_YINGYANG=0; } else PWOR_YINGYANG=1;
							
						if(th_num==1)	PWOR_YINGYANG=0;
						if(th_num==0)	PWOR_YINGYANG=1;	
							
							
						}		 
					  break ;						
						
						
						case 6 :
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开加热功能，给混合液添加温度  %d \r\n",th_num);						
						if(th_num==1)	PWOR_WENG=0;
						if(th_num==0)	PWOR_WENG=1;
						}		 
					  break ;						
							
						case 7:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 设置小时 时钟   %d \r\n",th_num);
            if(th_num<=24&&th_num>0){
				    hour=th_num;
							
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,hour,calendar.min,calendar.sec);
						
						}
							
						}		 
					  break ;				
						
						case 8:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 设置时间分钟   %d \r\n",th_num);
            if(th_num<=60&&th_num>0){
						
				    feng=th_num;
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,feng,calendar.sec);
						
						}
							
						}		 
					  break ;
						
						case 9:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 设置时间秒秒秒  %d \r\n",th_num);
            if(th_num<=60&&th_num>0){
						
				    miao=th_num;
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,miao);
						
						}
							
						}		 
					  break ;	
						
						
						case 10:
						{
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 设置植物生长灯亮度调节  %d \r\n",th_num);
        
						LED_PWOR=th_num * 3;
							
						}		 
					  break ;	
						
						
		       //得到实时数据帧数据，开始处理  			
						case  2 :
					{
					  //addr=get_rebuff3(lens);
						addr=get_rebuff2(lens);
		 				th_num=addr[8];  //塔吊编号      
						USART2_printf( USART2, "\r\n 日光灯操作手段指令 %d \r\n",th_num);	 
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
		 				th_num=addr[8];  //塔吊编号      
						USART2_printf( USART2, "\r\n 风扇操作手段指令 %d \r\n",th_num);	
						
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
					
					default: USART2_printf( USART2, "\r\n 没有操作指令 \r\n");
					
					
					}
						
					clean_rebuff2();	
					
				}
	
		
		clean_rebuff2();	
		

	}
	
	
	
}
	

}