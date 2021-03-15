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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//串口3初始化代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/3/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	


//串口接收缓存区 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
//中断缓存串口数据
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

extern u8 zd_che_net;  //主动查网的功能

u8 revert_buf[100];  //回复A1数据


u8 work_stay=0x03;    //工作状态  
u8 ietnet_stay=0x04;  //网络状态
extern u8 NL660_ip[4];
u8 jem_p,jem_g,jem_j;
u8 read_jem_p, read_jem_g, read_jem_j;

extern uint8_t U2_P;
extern unsigned char U2BUF[256];

extern u8 Objective_ip_B_1[4];
extern u8 Objective_ip_B_2[4];
extern u8 Objective_ip_B_3[4];
extern u8 Objective_ip_B_4[4];
extern u8 jh_boot; //激活标志
extern u8 zh_send_A2;   //是否开启主动回复A2操作等于1开启这个功能 ，0：得到IP地址是关闭这个操作，这个标志置0；

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

u8  qidiao_time=1;  //关闭更新起吊时间变量
u8	xidiao_time=0;  //打开更新卸吊时间变量	
u8  seve_biaoding=0;

u8 NL660_ip[4]={0x00,0x00,0x00,0x00};//本机ip

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
//上传服务器标定数据缓存区数组  
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
	
		//周期上次数据buf
	u8 send_zhouqi_date[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

 u8 xin_tiao_time[]={0xA5,0x5A,0x0F,0x07,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0xCC,0x33,0xC3,0x3C};
	

 
 
//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度

u8 sevr_03_comd=0;

u8 biaoding_bool=0; //接到标定数据的标志,代表可以往上发一次标定信息
u8 time_data_bool=0; //接到实时数据上传数据,代表可以往服务器发一帧新的数据
u8 long_time_data_bool=0; //接到工作周期上传数据帧,代表可以往服务器发一帧新的数据
		
u8 zhouqi_bool;  //周期上传控制变量  
		
extern u8 sever_time ;   //是否收到时间变量  		
		
/////////////////黑匣子变量存储标定数据////////////////////////////////
u8 shebei_1,shebei_2,shebei_3,shebei_4; 
 
u16 th_num=0; //塔吊编号
u16 liju=0; //力矩曲线变量
u16 zo_x=0; //坐标 X  
u16 zo_y=0; //坐标 Y
u16 qizhon=0; //起重臂长
u16 pingheng=0; //平衡臂长
u16 tagao=0;  //塔帽高
u16 qibh=0;   //起重臂高
u16 ssb=0 ;   //绳索倍率
u16 gd_AD1=0 ; //高度标定
u16 gd_X1 =0 ; //高度标定
u16 gd_AD2=0 ; //高度标定
u16 gd_X2 =0 ; //高度标定

u16 fd_AD1=0 ; //幅度标定
u16 fd_X1 =0 ; //幅度标定
u16 fd_AD2=0 ; //幅度标定
u16 fd_X2 =0 ; //幅度标定

u16 hd_AD1=0 ; //回转标定
u16 hd_X1 =0 ; //回转标定
u16 hd_AD2=0 ; //回转标定
u16 hd_X2 =0 ; //回转标定

u16 ld_AD1=0 ; //重量标定
u16 ld_X1 =0 ; //重量标定
u16 ld_AD2=0 ; //重量标定
u16 ld_X2 =0 ; //重量标定

u16 fs_bd=0 ; //风速标定校准值
u16 qx_bd=0 ; //倾斜标定校准 
u16 gds_bd=0 ; //高度上限位
u16 gdx_bd=0;  //高度下限位
u16 fdjx_bd=0; //幅度近端限位
u16 fdyx_bd=0; //幅度远端限位

u16 hzz_bd=0 ; //回转左限位
u16 hzy_bd=0 ; //回转右限位
u16 hpb_jl=0 ; //水平报警距离
u16 czb_jl=0 ; //垂直报价距离
u16 zlbj_bf=0 ; //重量报警百分比
u16 fsba_jz=0 ; //风速报警值
u16 qxb_jz=0 ; //倾斜报警值
u16 spy_jl=0 ; //水平预警距离 
u16 czyj_jl=0 ; //垂直预警距离 
u16 zlyj_bf=0 ; //重量预警百分比 
u16 fsy_jz=0  ; //风速预警值
u16 qxy_jz=0 ;  //倾斜预警值 
u16 pzzd_yx=0 ; //碰撞制动允许
u16 sfrz_qr=0 ; //身份认证是否确认
u16 gprs_sc=0 ; //GPRS 锁车   

//////////////////////////实时上传数据帧变量存储//////////////////////////
u16 higt_unm=0;  //高度 
u16 fdu_unm=0;  //幅度
u16 hzu_unm=0;  //回转
u16 zhong_unm=0 ; //载重
u16 dqyxzd_unm=0 ; //当前允许最大载重
u16 zzbf_unm=0  ;  //载重百分比
u16 fengs_unm=0 ;  //风速
u16 qxie_unm=0  ;  //倾斜
u16 gdxwz_unm=0 ;  //高度限位值
u16 gdhzx_unm=0 ;  //高度回转限位
u16 zhxzz_unm=0 ;  //载重限制值
u16 qxfsxz_unm=0 ; //倾斜风速限制
u16 gssxxw_unm=0 ; //干涉上下限位
u16 gyqhxw_unm=0 ; //干涉前后限位
u16 gyzyxw_unm=0 ; //干涉左右限位
u16 pzsxxw_unm=0 ; //碰撞上下限位
u16 pzqhxw_unm=0 ; //碰撞前后限位
u16 pzzyxw_unm=0 ; //碰撞左右限位
u16 jdqzt_unm=0  ; //继电器状态
u16 gzzt_unm=0   ; //工作状态
u16 qgqljzt_unm=0 ; // 传感器连接状态 

////////////////////工作周期数据帧数据存储////////////////////////////////
u16 qdsj_unm=0;   //起吊时间 
u16 qddgd_unm=0 ; //起吊点高度
u16 qddfd_unm=0 ; //起吊点幅度
u16 qddhz_unm=0 ; //起吊点回转
u16 ydsj_unm=0  ; //卸吊时间 
u16 xdgd_unm=0  ; //卸吊高度
u16 xdfd_unm=0  ; //卸吊幅度
u16 xdhz_unm=0  ; //卸吊回转
u16 bcdz_unm=0  ; //本次吊重  
u16 zdfh_unm=0  ; //最大负荷 

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
		
	//	if((U3_P_x<255)&&(usart3_reved_boo))//舍弃接收到的溢出部分的数据
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

//获取接收到的数据和长度
unsigned char *get_rebuff3(uint8_t *len)
{
    *len = U3_P_x;
    return (unsigned char *)&U3TEMP;
	
}

//获取数据类型
uint8_t Type_Get()
{
	
	uint8_t lens[1];
	u16 sever=0x0000,key=0;
	
	unsigned char *addr;
	//addr=get_rebuff3(lens);
	addr=get_rebuff2(lens);
	key=addr[4]<<8;
	sever= key+addr[5];
	printf("收到的数据寄存器地址为：%x",sever);
	switch(sever)
	{
	  case 0x1030:   //打开添加清水
		return 1;
		break;	
		
		case 0x1015:   //打开添加清水
		return 4;
		break;	
			
		case 0x1017:   //打开添加营养液 

		return 5;
		break;
		
		case 0x101A:   //打开添加营养液 
		return 6;
		break;
		
		case 0x0010:   //设置时钟的小时指令 
		return 7;
		break;
		
		case 0x0012:   //设置时钟的分钟指令 
		return 8;
		break;
		
		case 0x0014:   //设置时钟的秒指令 
		return 9;
		break;
		
		case 0x0180:   //设置生长灯亮度调节 
		return 10;
		break;
				
				
		case 0x0122:   //打开日光灯指令
	  time_data_bool=1;
		return 2;
		break;
		
		case 0x0124:   //打开风扇指令  
	  //long_time_data_bool=1;
		return 3;
		break;
		
	}
	
	
	
	
	return 0;
	
}




//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart3_init(u32 bound)
{  	 
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
  

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
//	TIM7_Int_Init(99,7199);		//10ms中断
	USART3_RX_STA=0;		//清零
//	TIM_Cmd(TIM7,DISABLE);			//关闭定时器7
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	u8 *pbuf;
	va_list ap;
//	pbuf=mymalloc(SRAMIN,USART3_MAX_SEND_LEN);	//申请内存
	if(!pbuf)									//内存申请失败
	{
		printf("u3 malloc error\r\n");
		return ;
	}
	va_start(ap,fmt);
	vsprintf((char*)pbuf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)pbuf);				//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕      
		USART_SendData(USART3,pbuf[j]); 
	}
//	myfree(SRAMIN,pbuf);						//释放内存
}


/****************************************************************************
* 名	称：void USART3_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART3_SendByte(u8 Data)		   //单字符数据输出
{
	USART_SendData(USART3, Data);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* 名	称：void USART3_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART3_SendString(u8* Data,u32 Len)		   //多字符输出
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
//判断U1_P是否继续增加，用于判断串口发送是否完成
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
//数据校验：正确返回1，错误返回0
uint8_t Check_CK(unsigned char *addr)
{
	uint8_t i,j,k;
	unsigned char s=0x00;
	unsigned char checksum =addr[3];
	j=addr[2];//长度

	 k=ASCII_u16(addr[3])-1; //来的数据位数
	 s=Q24_CheckSum(addr,k);
	// printf("du check_CK  is:%x\r\n",s);
	
	if(s==addr[3])
	{
		return 0x02;//校验正确
	}
	
	return 0x00;
}


uint8_t BLE_Check(void)
{
	uint8_t lens[1],k,s;
	unsigned char *addr;
	uint16_t i,j;

	//addr=get_rebuff3(lens);  //返回数组指针给定义的临时指针便于数据
	

	addr=get_rebuff2(lens);
	
	if(Check_HD(addr)==0x01)  //判断命令字头的存在 0xff 0x7e 
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
		addr=NULL;
		 return 0x00;//字头无效，结束本次检测，清空串口缓冲单元
		
	
	  

	

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
		//USART2_printf( USART2, "\r\n 校验 read is  %d \r\n", ret );
    ret_x =(ret&0xff);		
   // USART2_printf( USART2, "\r\n 校验 read is  %d \r\n", ret_x );		
		send_zhuce_date[10]=0x00;
		send_zhuce_date[10]=ret_x;	

		// send_zhuce_date[10]=TX_CheckSum(send_zhuce_date,10);
	
		for(i=0;i<15;i++)
		{			
		 USART2_SendByte(send_zhuce_date[i]);					
		}	
		
		 OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			
			if(sever_time){ return 1; y=20; } ; //连续发 3次，以没一秒为间隔 ，判断串口2收到有没有收到服务器的应答回复。没有就重启设备
			
			
			
		}
		if(!sever_time) return 0;

	
}

uint8_t USART3_GET(void)
{
	//解析串口3接受的命令，通过串口2发送响应的AT指令。操作网络模块  

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

	uint32_t fs_du;   //风速表示
	uint16_t fx_du;   //方向表示
	
	u8 w_s=0;        //定义一个补尝 
	u8 weizhang_bool=0;
	

	 u8 ret_x = 0;
   uint32_t ret=0;

	
	
	
	
	
	uint16_t read_remj;
	
	unsigned char *add_ut1;
	unsigned char *addr;
	
	if(U3_P_x>0){

	  if(CK_U3P())  //检查数据有没有接受完整
	{	
    		
			if(BLE_Check()==0x02)	 //检查BLE端数据,0x02为检查到完整的正确数据
			{
				
				USART2_DMAS("zou ya feng ffffffffffffffffffff\r\n");
				
        s=Type_Get();
			//	USART2_printf( USART2, "\r\n read is  %d \r\n", s );
		//		USART3_SendByte(s);
			switch(s)
						{
						//得到开水泵指令后
						case 0x01 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开水泵操作手段指令 %d \r\n",th_num);	
					
            j=th_num&0x01;
						if(j){ PWOR_YANG=0; } else PWOR_YANG=1;
						
						}		 
					  break ;
						
						case 0x04 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开抽清水到营养液桶中指令 %d \r\n",th_num);	
            j=th_num&0x01;
						if(j){ PWOR_SHU=0; } else PWOR_SHU=1;
						
						}		 
					  break ;	
						
						case 0x05 :
						{
						
            addr=get_rebuff3(lens);
		 				th_num=addr[8];  //塔吊编号 							
						USART2_printf( USART2, "\r\n 打开水泵操作手段指令 %d \r\n",th_num);	
            j=th_num&0x01;
						if(j){ PWOR_YINGYANG=0; } else PWOR_YINGYANG=1;
						
						}		 
					  break ;	
						
						
		       //得到实时数据帧数据，开始处理  			
						case  2 :
					{
					  addr=get_rebuff3(lens);
		 				th_num=addr[8];  //塔吊编号      
						USART2_printf( USART2, "\r\n 日光灯操作手段指令 %d \r\n",th_num);	 
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
		 				th_num=addr[8];  //塔吊编号      
						USART2_printf( USART2, "\r\n 风扇操作手段指令 %d \r\n",th_num);	
						
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

       
						
			clean_rebuff3();	//清空
				
			}
			 }

	}

	
	
	//返回在一个字符串里面第几个che，数据为位数值
	//che:分割符的十六进制表示,数据位最长150个字节  
	//k：数组里第一个分割符后面开始取数据
	//返回：在数组里的第几位数字开始去数据
	int chenk_date(u8 *str, u8 che , int k )
	{
		 u8 *s;
		int i,j,h,y;
     j=0;
		 h=0;
		 y=k;
		s=str;
	//	printf("du 年 字符串 is:%s\r\n",s);
		
		for(i=0;i<70;i++){
			 h++;
			if(s[i]==che)
				{
				   j++;
					
				//	printf("du 年 jss is:%d\r\n",j);
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
函数: Flash_write_buf(u32 addr,u8 *p,u16 n)
功能: 写数据到片上flash
参数: addr:地址
			*p:写入的数据
			n:个数
返回: 无
**************************************************/
void Flash_write_buf(u32 addr,u8 *p,u16 n)
{
	int i;
	FLASH_Unlock();//flash解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清楚标志
	FLASH_ErasePage(addr);
	while(n--)
	{
		i=*p;
		FLASH_ProgramWord(addr, i);
		p++;
		addr+=4;
	}
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清楚标志
	FLASH_Lock();//flash加锁
}
/*************************************************
函数: Flash_read_buf(void)
功能: 读片上flash
参数: 无
返回: 无
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
									//取pdp的用户名
									for(i=0;i<jem_p;i++)
									{
									sever_2_buf[8+i]=pdp_name[i];
									
									}
									j=8+jem_p;
									//取密码
									sever_2_buf[j]=jem_g;
									for(i=0;i<jem_g+1;i++){
									 sever_2_buf[j+1+i]=pdp_power[i];
									}
									j=j+jem_g+1;
									//去APN号
									sever_2_buf[j]=jem_j;
									for(i=0;i<jem_j+1;i++){
										sever_2_buf[j+1+i]=pdp_apn[i];
									}
									j=j+jem_j+1;
									
				
						if(s){
							for(i=0;i<5;i++){
							sever_2_buf[j+i]=NL660_ip[i];
							zh_send_A2=0;     //关闭主动上报工作状态功能
						  }
        	    }else{						
								//取ip地址给返回主机的数组
								for(i=0;i<5;i++){
									sever_2_buf[j+i]=0x00;
								}
					}
						
						j=j+5;
						sever_2_buf[j-1]=Q24_CheckSum(sever_2_buf,j-1);
						
						sever_2_buf[3]=j; //赋值帧长数据 
						
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
			||(strstr(U2BUF,"+CREG: 2,3")!=NULL)||(strstr(U2BUF,"+CREG: 2,4")!=NULL))//判断回复数据
		{
     s=1;
			ietnet_stay=0x04;
			jh_boot=0;
						LED_G=1;
						LED_R=0;
		}else {
			s=0;
		}
			      
						
		        sever_2_buf[5]=work_stay;	// 工作状态值不改变
						sever_2_buf[6]=ietnet_stay;
		
				if(send_cong_apn_bool){  
									sever_2_buf[7]=jem_p;
									//取pdp的用户名
									for(i=0;i<jem_p;i++)
									{
									sever_2_buf[8+i]=pdp_name[i];
									
									}
									j=8+jem_p;
									//取密码
									sever_2_buf[j]=jem_g;
									for(i=0;i<jem_g+1;i++){
									 sever_2_buf[j+1+i]=pdp_power[i];
									}
									j=j+jem_g+1;
									//去APN号
									sever_2_buf[j]=jem_j;
									for(i=0;i<jem_j+1;i++){
										sever_2_buf[j+1+i]=pdp_apn[i];
									}
									j=j+jem_j+1;
									
					}else{
						
						      sever_2_buf[7]=read_jem_p;
									//取pdp的用户名
									for(i=0;i<read_jem_p;i++)
									{
									sever_2_buf[8+i]=red_pdp_name[i];
									
									}
									j=8+read_jem_p;
									//取密码
									sever_2_buf[j]=read_jem_g;
									for(i=0;i<read_jem_g+1;i++){
									 sever_2_buf[j+1+i]=red_pdp_power[i];
									}
									j=j+read_jem_g+1;
									//去APN号
									sever_2_buf[j]=read_jem_j;
									for(i=0;i<read_jem_j+1;i++){
										sever_2_buf[j+1+i]=red_pdp_apn[i];
									}
						
						      j=j+read_jem_j+1;
						
					}
		
		
		
		       
									
				
						if(s)
							{						
								//取ip地址给返回主机的数组
								for(i=0;i<5;i++){
									sever_2_buf[j+i]=0x00;
								}
					}
						
						j=j+5;
						sever_2_buf[j-1]=Q24_CheckSum(sever_2_buf,j-1);
						
						sever_2_buf[3]=j; //赋值帧长数据 
						if(s){
						for(i=0;i<j;i++){
							
							USART3_SendByte(sever_2_buf[i]);
						}
					}
				return s;
					
}
	
//累加和校验方式     
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




