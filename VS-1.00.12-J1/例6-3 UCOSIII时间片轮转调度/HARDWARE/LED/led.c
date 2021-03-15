#include "led.h"
#include "delay.h"
#include "key.h"
#include "adc.h"
 #include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

extern u16 ch1_time,ch2_time,ch3_time,ch4_time,ch5_time,ch6_time;      //定时器里面的变量记录那个通道ms值，使输出每一个通道值在1ms内不会重复输出
extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //开机默认周期10ms  

u16 ch2_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch2_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch2_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch2_unm_E2_x1;  //通道2按摩波E2输出控制变量



u16 ch6_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch6_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch6_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch6_unm_E2_x1;  //通道2按摩波E2输出控制变量


u16 ch5_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch5_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch5_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch5_unm_E2_x1;  //通道2按摩波E2输出控制变量

u16 ch4_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch4_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch4_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch4_unm_E2_x1=0;  //通道2按摩波E2输出控制变量

u16 ch3_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch3_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch3_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch3_unm_E2_x1=0;  //通道2按摩波E2输出控制变量

u16 ch1_unm_E1_t=560; //通道2按摩波E1 自动变化周期 
u16 ch1_unm_E3_t=100; //通道2按摩波E3 自动变化周期 
u8  ch1_unm_E3_x1=0;  //通道2按摩波E3输出控制变量

u16 ch1_unm_E2_x1;  //通道2按摩波E2输出控制变量


u8 send_guzhang=0;  //发送接触故障码治疗标志




u8 clos_bis_unm=1;

//疏密波输出控制变量
u16 ch1_shu_mi=0,ch2_shu_mi=0,ch3_shu_mi=0,ch4_shu_mi=0,ch5_shu_mi=0,ch6_shu_mi=0; 

u8  ch1_bool=0,ch2_bool=0,ch3_bool=0,ch4_bool=0,ch5_bool=0,ch6_bool=0;

u8 GZE_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x48,0x00,0x00};  //短路开路故障提示变量



u8 set_ch1_bo=1;  //开机默认是连续波输出
u8 set_ch2_bo=1;  //开机默认是连续波输出
u8 set_ch3_bo=1;  //开机默认是连续波输出
u8 set_ch4_bo=1;  //开机默认是连续波输出
u8 set_ch5_bo=1;  //开机默认是连续波输出
u8 set_ch6_bo=1;  //开机默认是连续波输出


u8 text_ch1=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路
u8 text_ch2=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路
u8 text_ch3=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路
u8 text_ch4=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路
u8 text_ch5=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路
u8 text_ch6=0;   //定义检测短路开路频率 暂时定为没100个脉冲检测一次锻炼开路

extern u8 ch1_duan_kai;  //通道1短路开路变量
extern u8 ch2_duan_kai;  //通道2短路开路变量
extern u8 ch3_duan_kai;  //通道3短路开路变量
extern u8 ch4_duan_kai;  //通道4短路开路变量
extern u8 ch5_duan_kai;  //通道5短路开路变量
extern u8 ch6_duan_kai;  //通道6短路开路变量



int delay_long_lang=130; 

int delay_long_lang_1=260; 

int delay_long=148;  //电脉冲宽度时间定义 单位为us秒   以前是200吧  135   148
int delay_long_1=60;  //电脉冲宽度时间定义 单位为us秒  以前是100    55     60


u16 sumi_x1,sumi_x2,sumi_x3,sumi_x4,sumi_x5,sumi_x6; //疏密波的控制变量 高电平 输出状态
u16 sumi_y1,sumi_y2,sumi_y3,sumi_y4,sumi_y5,sumi_y6; //疏密波的控制变量 低电平 输出状态

u16 qinchui_x1,qingchui_x1;  //轻捶波输出高电平
u16 qinchui_y1,qinchui_4T;   //轻捶波输出低电平


u16 sumi_4T;          //4倍输出时间再停止一下

bool ch_lang=FALSE;

extern  u8 tan_xue_boo ; //探穴检测到定义函数 当检测到穴位时为2，检测穴位相应完为0，应为是震动电路产生的穴位信号，所以每次相应完都置为0 .

extern u8 system_start_EL;  //开路短路停止输出电脉冲标识

 extern u8 bisuo_boo;  //打开

extern u8 zhi_unm_date;





void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能PB,PE端口时钟
	

	//加热管脚
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);		
	

 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7| 
	                              GPIO_Pin_8|GPIO_Pin_9;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	
 GPIO_SetBits(GPIOG,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7| 
	                              GPIO_Pin_8|GPIO_Pin_9);	
	
	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;				 //LED0-->PB.5 端口配置  HT-2的 LED控制管脚 
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //开漏输出输出   
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz  
// GPIO_Init(GPIOE, &GPIO_InitStructure);	
//	
// GPIO_SetBits(GPIOE,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);	
	


 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);    //电脉冲口PB3是jiack口，关闭一下这个工能

	
	////通道5至6 电脉冲输出配置  HT-3   加热管脚 PD 12  13    14   15 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //MCU_O6-PD6  MCU_O5-PD7 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOD,GPIO_Pin_6|GPIO_Pin_7);						 //PB.5 输出高
 GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
 
	////通道2至6 电脉冲输出配置  HT-3
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9;			//MCU_O1-PB9  MCU_O2-PB8   MCU_O3-PB4  MCU_O4-PB3
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9);						 //PB.5 输出高
	
	
	
 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
}

//以前是推挽输出，周公后来改成了集电极输出配置成原LED1——LED6的配置方式只改4051的控制管脚 
void off_4051_int(void)   
{
	
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟	

	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				     //蜂鸣器LEEP的指示管脚     
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);	                   //蜂鸣器的控制管脚
	
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //4051控制使能管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出输出 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);    //INT1 INT2 置高就关闭了
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;				 //4051控制使能管脚 A=PC10  B=PC11  C=PC12  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出输出 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);		
 GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);    // 4051的选择使能 A B  C  输出控制角   
 

	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;				     //系统信号指示灯 LED_G的控制管脚   
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);	
 GPIO_SetBits(GPIOD,GPIO_Pin_8);	
 GPIO_ResetBits(GPIOD,GPIO_Pin_11);


 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 //电平转换使能控制引脚，配置成开漏输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出输出 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);           //OE
	



	
}


//打开蜂鸣器，是蜂鸣器发出叫声
void on_leep(void)
{
	u8 i,j;
	
  if(bisuo_boo){   //关闭和打开闭锁功能测试
	
	if((ch1_duan_kai>=26)||(ch2_duan_kai>=26)||(ch3_duan_kai>=26)||(ch4_duan_kai>=26)||(ch5_duan_kai>=26)||(ch6_duan_kai>=26))
  {
	 system_start_EL=0;  //关闭电脉冲输出  
	
		send_guzhang=2;   //打开发送标标识
		
		GZE_LED[7]=0x00;
		
				for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,GZE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
		
  }
	
	//关闭闭锁恢复正常的指令只发生一次，减小显示屏压力
	if(send_guzhang==2){
		
		if(system_start_EL==1){
			
			GZE_LED[7]=0x01;
			send_guzhang=0;   //关闭发送标标识
			
							for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,GZE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
			
			
		}
		
		
		
	}
	
	
	
	
	if(!system_start_EL) //短路开路蜂鸣器提示
	{
	LEEP=1;
		
	}else{
		
	LEEP=0;
	}
	
	
}
	
	//LEEP=~LEEP;
	
	if((ch1_duan_kai>=26)||(ch2_duan_kai>=26)||(ch3_duan_kai>=26)||(ch4_duan_kai>=26)||(ch5_duan_kai>=26)||(ch6_duan_kai>=26)||(tan_xue_boo==2)||(zhi_unm_date>=2))
	{    //当连续检测到大于等于6次短路开路后蜂鸣器发出提示音
		
  LEEP=1;

	tan_xue_boo=0;
	ch1_duan_kai=0;  //清除提示音标志
	ch2_duan_kai=0;	
	ch3_duan_kai=0;  //清除提示音标志
	ch4_duan_kai=0;	
  ch5_duan_kai=0;  //清除提示音标志
	ch6_duan_kai=0;			
	zhi_unm_date=0;	
		
	}else{
	LEEP=0;
}
	
	
	
	
	if((tan_xue_boo==2)||(zhi_unm_date>=2))
	{    
		
 // LEEP=1;  //蜂鸣器发出提示音

	tan_xue_boo=0;   //置位探穴标志
	zhi_unm_date=0;	
		
	}else{
//	LEEP=0;
}
	






	
	
	
	
}





void off_NO1_4051(void){    //打开通道1输出    
	
	
	
GPIO_SetBits(GPIOD,GPIO_Pin_0);
GPIO_ResetBits(GPIOD,GPIO_Pin_1);		
	
GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_12);
GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	
}


void close_4051(void){    //  
	
	
	
GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);    //INT1 INT2 置高就关闭了
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



//输入参数说明  pin为打开那一片4051芯片的使能 1为打开第一个片4051选择器，2为打开第二片4051选择器，
//xin为打开那一个选择端口将MI值输出到X1~X8的端口 
void star_4051(u8 pin,u8 xin)
{
	if(pin==1){ INH1=0; INH2=1;}
	if(pin==2){ INH1=1; INH2=0;}
	if(xin==0){DC4051_C=0;DC4051_B=0;DC4051_A=0;}  //打开0通道
	if(xin==1){DC4051_C=0;DC4051_B=0;DC4051_A=1;}  //打开0通道
	if(xin==2){DC4051_C=0;DC4051_B=1;DC4051_A=0;}  //打开0通道
	if(xin==3){DC4051_C=0;DC4051_B=1;DC4051_A=1;}  //打开0通道
	if(xin==4){DC4051_C=1;DC4051_B=0;DC4051_A=0;}  //打开0通道
	if(xin==5){DC4051_C=1;DC4051_B=0;DC4051_A=1;}  //打开0通道
	if(xin==6){DC4051_C=1;DC4051_B=1;DC4051_A=0;}  //打开0通道
	if(xin==7){DC4051_C=1;DC4051_B=1;DC4051_A=1;}  //打开0通道
}


//设置那一个通道输出什么波形 
//设置那一个通道输出电脉冲信号波形 1为1通道输出 2为2通道输出 。。。。。。。。。
// h 为1时输出正波 h为2时输出负波 
void ch_X_set_ms(u8 ch, u8 h)
{
	
	
	if(ch==1){
	if(h==1){  //正脉冲输出   
	star_4051(1,2);
	MCU_O1=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch1();
		
	}
  delay_us(delay_long_1);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
	star_4051(1,3); //打开1片4051的2通道    
	MCU_O1=0;
	delay_us(delay_long);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==3){  //正脉冲输出   300ms输出
	star_4051(1,2);
	MCU_O1=0;
	delay_us(delay_long_lang);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch1();
	}
		
  delay_us(delay_long_lang);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	if(h==4){  //负脉冲输出
	star_4051(1,3); //打开1片4051的2通道    
	MCU_O1=0;
	delay_us(delay_long_lang_1);
	MCU_O1=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
	
}
	

	if(ch==2){
	if(h==1){  //正脉冲输出    
	star_4051(1,5);
	MCU_O2=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch2();
	}
	
  delay_us(delay_long_1);		
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
  star_4051(1,1);	 //第一版是（1，7）第二版（1，1）
	MCU_O2=0;
	delay_us(delay_long);
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //正脉冲输出    
	star_4051(1,5);
	MCU_O2=0;
	delay_us(delay_long_lang);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量		
	Get_ch2();
	}
	
  delay_us(delay_long_lang);		
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==4){  //负脉冲输出
  star_4051(1,1);	 //第一版是（1，7）第二版（1，1）
	MCU_O2=0;
	delay_us(delay_long_lang_1);
	MCU_O2=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
}
	
	if(ch==3){ 
	if(h==1){  //正脉冲输出     
	star_4051(1,6);
	MCU_O3=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch3();
	}
		
  delay_us(delay_long_1);	
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
  star_4051(1,4);	 //打开1片4051的2通道    
	MCU_O3=0;
	delay_us(delay_long);
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //正脉冲输出     
	star_4051(1,6);
	MCU_O3=0;
	delay_us(delay_long_lang);
	
	if(clos_bis_unm){  //关闭短路开路检测控制变量		
	Get_ch3();
		
	}
  delay_us(delay_long_lang);	
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==4){  //负脉冲输出
  star_4051(1,4);	 //打开1片4051的2通道    
	MCU_O3=0;
	delay_us(delay_long_lang_1);
	MCU_O3=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
}
	
	if(ch==4){
	if(h==1){  //正脉冲输出    
	star_4051(2,2);
	MCU_O4=0;
	delay_us(delay_long_1);
		
	if(clos_bis_unm){  //关闭短路开路检测控制变量		
	Get_ch4();
	}
  delay_us(delay_long_1);	
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
	star_4051(2,3); //打开1片4051的2通道   
	MCU_O4=0;
	delay_us(delay_long);
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //正脉冲输出    
	star_4051(2,2);
	MCU_O4=0;
	delay_us(delay_long_lang);
			
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch4();
	}
  delay_us(delay_long_lang);	
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==4){  //负脉冲输出
	star_4051(2,3); //打开1片4051的2通道   
	MCU_O4=0;
	delay_us(delay_long_lang_1);
	MCU_O4=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
	
}
	



	if(ch==5){
	if(h==1){  //正脉冲输出   
	star_4051(2,5);
	MCU_O5=0;
	delay_us(delay_long_1);
	
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch5();
		
	}
	
  delay_us(delay_long_1);	
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
	star_4051(2,1);	 //第一版是（2.7） 第二版是（2，1）
	MCU_O5=0;
	delay_us(delay_long);
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==3){  //正脉冲输出   
	star_4051(2,5);
	MCU_O5=0;
	delay_us(delay_long_lang);
			
	if(clos_bis_unm){  //关闭短路开路检测控制变量	
	Get_ch5();
	}
  delay_us(delay_long_lang);	
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	
	if(h==4){  //负脉冲输出
	star_4051(2,1);	 //第一版是（2.7） 第二版是（2，1）
	MCU_O5=0;
	delay_us(delay_long_lang_1);
	MCU_O5=1;
	delay_us(10);	
	close_4051();
	}
	
	
	
}
	
	if(ch==6){
	if(h==1){  //正脉冲输出
  star_4051(2,6);	
	MCU_O6=0;
	delay_us(delay_long_1);
		
		
	
	if(clos_bis_unm==1){  //关闭短路开路检测控制变量	
	Get_ch6();
	}
	
	delay_us(delay_long_1);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
	
	if(h==2){  //负脉冲输出
	star_4051(2,4);
	MCU_O6=0;
	delay_us(delay_long);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
	
 	if(h==3){  //正脉冲输出
  star_4051(2,6);	
	MCU_O6=0;
	delay_us(delay_long_lang);
		
		

	if(clos_bis_unm==1){  //关闭短路开路检测控制变量	
	Get_ch6();
	
	}
	
	delay_us(delay_long_lang);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
	if(h==4){  //负脉冲输出
	star_4051(2,4);
	MCU_O6=0;
	delay_us(delay_long_lang_1);
	MCU_O6=1;
	delay_us(10);	
	close_4051();
	}
 
}
	
	

}





/////////////通道2 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch2_pin(void)
{
	if(set_ch2_bo==1)   //设置为连续波
	{
	 if(ch2_time==2){
		ch_X_set_ms(2,3);  //2通道正脉冲输出 
	 }
	if(ch2_time>=ch2_unm) ch2_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch2_bo==2)
 { 
	   
	 
	 	 sumi_x1=(ch2_unm-8);
		 sumi_x2=(sumi_x1+ch2_unm);

     sumi_y1=(ch2_unm+2);
		 sumi_y2=(sumi_y1+ch2_unm);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch2_time>=sumi_y2){
			 ch2_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch2_shu_mi++;
			 //周期等于实际输出周期
			 if(ch2_shu_mi<40){
		
				 ch2_unm=ch2_unm;
	
			 }
			 //周期等于实际周期的的4倍；
			  if(ch2_shu_mi==40){
				
				 ch2_unm=(ch2_unm*4);
				
			 }
			 
			 if(ch2_shu_mi>=80){
				 ch2_shu_mi=0;
			//	 set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
			 set_ch2_sinc();	 
				 
			 }	 
		 }
 
	 
	 
	 
	 
	 
	 
//	   sumi_x1=(ch2_unm+22); //高电平输出状态时间 第2周期输出 
//	   sumi_x2=((ch2_unm*2)+42);
//	   sumi_x3=((ch2_unm*3)+62); 
//     
//	 
//	   sumi_y1=(ch2_unm+32); //低电平输出状态时间 第2周期输出时间点位
//	   sumi_y2=(sumi_x2+10);
//	   sumi_y3=(sumi_x3+10); 
//     
//	   sumi_4T=((ch2_unm*7)+80);
//	 
//     if((ch2_time==2)||(ch2_time==sumi_x1)||(ch2_time==sumi_x2) ||(ch2_time==sumi_x3)  )	 
//		 {
//			 ch_X_set_ms(2,1);  //2通道正脉冲输出 
//			 
//		 }
//	   if((ch2_time==12)||(ch2_time==sumi_y1)||(ch2_time==sumi_y2) ||(ch2_time==sumi_y3)    )
//		{
//			 
//			 ch_X_set_ms(2,2);  //2通道负脉冲输出 	 
//			 
//		 }
//		 
//		if(ch2_time>=sumi_4T) ch2_time=0;  //控制周期输出T的值   
//		 
		 
		 
		 		
	}
	
	if(set_ch2_bo==3)    //轻捶波输出
	{
		
		
		sumi_x1=(ch2_unm-8);
		 sumi_x2=(sumi_x1+ch2_unm);

     sumi_y1=(ch2_unm+2);
		 sumi_y2=(sumi_y1+ch2_unm);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch2_time>=sumi_y2){
			 ch2_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			// set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
	 
			 set_ch2_sinc();
			 
			 }	
		
		
		
		
		
//		qinchui_x1=2; //第一次输出轻捶波的正波
//		qinchui_y1=(ch2_unm+10+2); //第一次输出轻捶波的负波
//		qinchui_4T=((ch2_unm*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2通道负脉冲输出 	
//				
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //控制周期输出T的值  
//		
		
		
		
	}
	
	if(set_ch2_bo==4)  //设置为按摩波E1波的输出  输出波形周期为0.01～0.56s变化的轻捶波  
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch2_unm_E1_t-8);
		 sumi_x2=(sumi_x1+ch2_unm_E1_t);

     sumi_y1=(ch2_unm_E1_t+2);
		 sumi_y2=(sumi_y1+ch2_unm_E1_t);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch2_time>=sumi_y2){
			 ch2_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			// set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
	 
			 }	
		
		
		
//		qinchui_x1=2; //第一次输出轻捶波的正波
//		qinchui_y1=(ch2_unm_E1_t+10+2); //第一次输出轻捶波的负波
//		qinchui_4T=((ch2_unm_E1_t*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2通道负脉冲输出 	
//				
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //控制周期输出T的值  
//		
		
		
	}
	
	if(set_ch2_bo==5) //按摩波E2波输出波形
	{
				if(ch2_unm_E2_x1<=1200){
				 if(ch2_time==2){
				 ch_X_set_ms(2,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch2_unm_E2_x1>1200){
					
					if(ch2_time==2){
				 ch_X_set_ms(2,4);  //2通道负脉冲输出 
				 
			 }}
				
			ch2_unm_E2_x1++;
			if(ch2_unm_E2_x1>=2401) ch2_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
			if(ch2_time>=20) ch2_time=0;  //控制周期输出T的值   

	}
	
	if(set_ch2_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		 sumi_x1=(ch2_unm_E3_t-8);
		 sumi_x2=(sumi_x1+ch2_unm_E3_t);

     sumi_y1=(ch2_unm_E3_t+2);
		 sumi_y2=(sumi_y1+ch2_unm_E3_t);

		if((ch2_time==sumi_x1)||(ch2_time==sumi_x2))	 
		 {
			ch_X_set_ms(2,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch2_time==sumi_y1)||(ch2_time==sumi_y2))
		{
			 
	    ch_X_set_ms(2,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch2_time>=sumi_y2){
			 ch2_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch2_unm_E3_x1++; //计算脉冲变量一个负波也一个正波为一个
	 
			 }	
		 
		if(ch2_unm_E3_x1>=5) { 
		ch2_unm_E3_t=(ch2_unm_E3_t-10);
    if(	ch2_unm_E3_t<=10){
    ch2_unm_E3_t=100;
		}			

		ch2_unm_E3_x1=0;   }
		
		
		
		
		
		
		
		
		
		
		
//	  qinchui_x1=2; //第一次输出轻捶波的正波
//		qinchui_y1=(ch2_unm_E3_t+10+2); //第一次输出轻捶波的负波
//		qinchui_4T=((ch2_unm_E3_t*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch2_time==2) {
//		ch_X_set_ms(2,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch2_time==qinchui_y1) {
//		ch_X_set_ms(2,2);  //2通道负脉冲输出 	
//		ch2_unm_E3_x1++; //计算脉冲变量一个负波也一个正波为一个
//    			
//		}
//		
//		if(ch2_time>=qinchui_4T) ch2_time=0;  //控制周期输出T的值  
//		if(ch2_unm_E3_x1>=10) { 
//		ch2_unm_E3_t=(ch2_unm_E3_t-10);
//    if(	ch2_unm_E3_t<=10){
//    ch2_unm_E3_t=100;
//		}			

//		ch2_unm_E3_x1=0;   }
//		
		
		
		
		
		
		
	}
	
	
	
}



/////////////通道6 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch6_pin(void)
{
	if(set_ch6_bo==1)   //设置为连续波
	{
	 if(ch6_time==7){
		ch_X_set_ms(6,3);  //2通道正脉冲输出 
	 }
	if(ch6_time>=ch6_unm) ch6_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch6_bo==2)
 { 
	 
	 	 sumi_x1=(ch6_unm-3);
		 sumi_x2=(sumi_x1+ch6_unm);

     sumi_y1=(ch6_unm+7);
		 sumi_y2=(sumi_y1+ch6_unm);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch6_time>=sumi_y2){
			 ch6_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch6_shu_mi++;
			 //周期等于实际输出周期
			 if(ch6_shu_mi<40){
			
				 ch6_unm=ch6_unm;		
			 }
			 //周期等于实际周期的的4倍；
			  if(ch6_shu_mi==40){
				
				 ch6_unm=(ch6_unm*4);
				
			 }
			 
			 if(ch6_shu_mi>=80){
				 ch6_shu_mi=0;
				 //set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				 set_ch6_sinc();
				 
			 }	 
		 }
 
	 
	 
	 
	 
//	   sumi_x1=(ch6_unm+26); //高电平输出状态时间 第2周期输出 
//	   sumi_x2=((ch6_unm*2)+46);
//	   sumi_x3=((ch6_unm*3)+66); 
//     
//	 
//	   sumi_y1=(ch6_unm+36); //低电平输出状态时间 第2周期输出时间点位
//	   sumi_y2=(sumi_x2+10);
//	   sumi_y3=(sumi_x3+10); 
//     
//	   sumi_4T=((ch6_unm*7)+80);
//	 
//     if((ch6_time==6)||(ch6_time==sumi_x1)||(ch6_time==sumi_x2) ||(ch6_time==sumi_x3)  )	 
//		 {
//			 ch_X_set_ms(6,1);  //2通道正脉冲输出 
//			 
//		 }
//	   if((ch6_time==16)||(ch6_time==sumi_y1)||(ch6_time==sumi_y2) ||(ch6_time==sumi_y3)    )
//		{
//			 
//			 ch_X_set_ms(6,2);  //2通道负脉冲输出 	 
//			 
//		 }
//		 
//			 if(ch6_time>=sumi_4T) ch6_time=0;  //控制周期输出T的值   
//		




		 
	}
	
	if(set_ch6_bo==3)    //轻捶波输出
	{
		
		 sumi_x1=(ch6_unm-3);
		 sumi_x2=(sumi_x1+ch6_unm);

     sumi_y1=(ch6_unm+7);
		 sumi_y2=(sumi_y1+ch6_unm);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch6_time>=sumi_y2){
			 ch6_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		   //set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
			set_ch6_sinc();	

			 
			 }	
		
		
		
//		qinchui_x1=6; //第一次输出轻捶波的正波
//		qinchui_y1=(ch6_unm+10+6); //第一次输出轻捶波的负波
//		qinchui_4T=((ch6_unm*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2通道负脉冲输出 	
//				
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //控制周期输出T的值  
//		
		
		
		
		
		
	}
	
	if(set_ch6_bo==4)  //设置为按摩波E1波的输出  变化规律先发周期0.01的50个正波，再发周期0.01的50个负波，
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch6_unm_E1_t-3);
		 sumi_x2=(sumi_x1+ch6_unm_E1_t);

     sumi_y1=(ch6_unm_E1_t+7);
		 sumi_y2=(sumi_y1+ch6_unm_E1_t);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch6_time>=sumi_y2){
			 ch6_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		  // set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				 
			 }	
		
		
		
		
		
		
//		qinchui_x1=6; //第一次输出轻捶波的正波
//		qinchui_y1=(ch6_unm_E1_t+10+6); //第一次输出轻捶波的负波
//		qinchui_4T=((ch6_unm_E1_t*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2通道负脉冲输出 	
//				
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //控制周期输出T的值  
//		
		
		
	}
	
	if(set_ch6_bo==5) //按摩波E2波输出波形
	{
				if(ch6_unm_E2_x1<=1200){
				 if(ch6_time==7){
				 ch_X_set_ms(6,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch6_unm_E2_x1>1200){
					
					if(ch6_time==7){
				 ch_X_set_ms(6,4);  //2通道负脉冲输出 
				 
			 }}
				
			ch6_unm_E2_x1++;
			if(ch6_unm_E2_x1>=2401) ch6_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
			if(ch6_time>=20) ch6_time=0;  //控制周期输出T的值   

	}
	
	if(set_ch6_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		
		 sumi_x1=(ch6_unm_E3_t-3);
		 sumi_x2=(sumi_x1+ch6_unm_E3_t);

     sumi_y1=(ch6_unm_E3_t+7);
		 sumi_y2=(sumi_y1+ch6_unm_E3_t);

		if((ch6_time==sumi_x1)||(ch6_time==sumi_x2))	 
		 {
			ch_X_set_ms(6,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch6_time==sumi_y1)||(ch6_time==sumi_y2))
		{
			 
	    ch_X_set_ms(6,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch6_time>=sumi_y2){
			 ch6_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
       ch6_unm_E3_x1++; //计算脉冲变量一个负波也一个正波为一个
				 
			 }	
		
		if(ch6_unm_E3_x1>=5) { 
		ch6_unm_E3_t=(ch6_unm_E3_t-10);
    if(	ch6_unm_E3_t<=10){
    ch6_unm_E3_t=100;
		}			

		ch6_unm_E3_x1=0;   }
		
		
		
		
		
		
//	  qinchui_x1=6; //第一次输出轻捶波的正波
//		qinchui_y1=(ch6_unm_E3_t+10+6); //第一次输出轻捶波的负波
//		qinchui_4T=((ch6_unm_E3_t*2)+20);//轻捶波的总周期为两个设置周期加两个正负连续周期时间
//		
//		
//		if(ch6_time==6) {
//		ch_X_set_ms(6,1);  //2通道正脉冲输出 	
//				
//		}
//		
//		if(ch6_time==qinchui_y1) {
//		ch_X_set_ms(6,2);  //2通道负脉冲输出 	
//		ch6_unm_E3_x1++; //计算脉冲变量一个负波也一个正波为一个
//    			
//		}
//		
//		if(ch6_time>=qinchui_4T) ch6_time=0;  //控制周期输出T的值  
//		if(ch6_unm_E3_x1>=10) { 
//		ch6_unm_E3_t=(ch6_unm_E3_t-10);
//    if(	ch6_unm_E3_t<=10){
//    ch6_unm_E3_t=100;
//		}			

//		ch6_unm_E3_x1=0;   }
//		
		
		
		
		
		
	}
	
	
	
}

/////////////通道6 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch5_pin(void)
{
	if(set_ch5_bo==1)   //设置为连续波
	{
	 if(ch5_time==6){
		ch_X_set_ms(5,3);  //6通道正脉冲输出 
	 }
	if(ch5_time>=ch5_unm) ch5_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch5_bo==2)
 { 
	 
	 	 sumi_x1=(ch5_unm-4);
		 sumi_x2=(sumi_x1+ch5_unm);

     sumi_y1=(ch5_unm+6);
		 sumi_y2=(sumi_y1+ch5_unm);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch5_time>=sumi_y2){
			 ch5_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch5_shu_mi++;
			 //周期等于实际输出周期
			 if(ch5_shu_mi<40){
				 ch5_unm=ch5_unm;
			 }
			 //周期等于实际周期的的4倍；
			  if(ch5_shu_mi==40){
				
				 ch5_unm=(ch5_unm*4);
				
			 }
			 
			 if(ch5_shu_mi>=80){
				 ch5_shu_mi=0;
				// set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				 set_ch5_sinc();		 
			 }	 
		 }
 
	 
		 
		 
		 
		 
	}
	
	if(set_ch5_bo==3)    //轻捶波输出
	{
		
		
		 sumi_x1=(ch5_unm-4);
		 sumi_x2=(sumi_x1+ch5_unm);

     sumi_y1=(ch5_unm+6);
		 sumi_y2=(sumi_y1+ch5_unm);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3通道负脉冲输出 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		   //set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				set_ch5_sinc();	

				
			 }	 


		

		
		
		
	}
	
	if(set_ch5_bo==4)  //设置为按摩波E1波的输出  变化规律先发周期0.01的50个正波，再发周期0.01的50个负波，
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch5_unm_E1_t-4);
		 sumi_x2=(sumi_x1+ch5_unm_E1_t);

     sumi_y1=(ch5_unm_E1_t+6);
		 sumi_y2=(sumi_y1+ch5_unm_E1_t);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3通道负脉冲输出 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
	//	   set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				 		 
			 }	 
		
	
		
	
		
		
		
	}
	
	if(set_ch5_bo==5) //按摩波E2波输出波形
	{
				if(ch5_unm_E2_x1<=1200){
				 if(ch5_time==6){
				 ch_X_set_ms(5,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch5_unm_E2_x1>1200){
					
					if(ch5_time==6){
				 ch_X_set_ms(5,4);  //2通道负脉冲输出 
				 
			 }}
				
			ch5_unm_E2_x1++;
			if(ch5_unm_E2_x1>=2401) ch5_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
			if(ch5_time>=20) ch5_time=0;  //控制周期输出T的值   

	}
	
	if(set_ch5_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		
		
		 sumi_x1=(ch5_unm_E3_t-4);
		 sumi_x2=(sumi_x1+ch5_unm_E3_t);

     sumi_y1=(ch5_unm_E3_t+6);
		 sumi_y2=(sumi_y1+ch5_unm_E3_t);

		if((ch5_time==sumi_x1)||(ch5_time==sumi_x2))	 
		{
			ch_X_set_ms(5,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch5_time==sumi_y1)||(ch5_time==sumi_y2))
		{
			 
	    ch_X_set_ms(5,2);  //3通道负脉冲输出 
			 
		 }
		
			if(ch5_time>=sumi_y2){
			 ch5_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
	     ch5_unm_E3_x1++; //计算脉冲变量一个负波也一个正波为一个
				 		 
			 }	 
		
		if(ch5_unm_E3_x1>=5) { 
		ch5_unm_E3_t=(ch5_unm_E3_t-10);
    if(	ch5_unm_E3_t<=10){
    ch5_unm_E3_t=100;
		}			

		ch5_unm_E3_x1=0;   }
		
	
		
	}
	
	
	
}


/////////////通道4 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch4_pin(void)
{
	if(set_ch4_bo==1)   //设置为连续波
	{
	 if(ch4_time==5){
		ch_X_set_ms(4,3);  //6通道正脉冲输出 
	 }
	if(ch4_time>=ch4_unm) ch4_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch4_bo==2)
 { 
	   
	 	 sumi_x1=(ch4_unm-5);
		 sumi_x2=(sumi_x1+ch4_unm);

     sumi_y1=(ch4_unm+5);
		 sumi_y2=(sumi_y1+ch4_unm);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch4_time>=sumi_y2){
			 ch4_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch4_shu_mi++;
			 //周期等于实际输出周期
			 if(ch4_shu_mi<40){
				if(ch4_bool){
				 ch4_unm=ch4_unm;

				}			
			 }
			 //周期等于实际周期的的4倍；
			  if(ch4_shu_mi==40){
				
				 ch4_unm=(ch4_unm*4);
				
			 }
			 
			 if(ch4_shu_mi>=80){
				 ch4_shu_mi=0;
				 //set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				 set_ch4_sinc();	
				 
			 }	 
		 }
 


		 
	}
	
	if(set_ch4_bo==3)    //轻捶波输出
	{
		
		
		 sumi_x1=(ch4_unm-5);
		 sumi_x2=(sumi_x1+ch4_unm);

     sumi_y1=(ch4_unm+5);
		 sumi_y2=(sumi_y1+ch4_unm);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2通道负脉冲输出 
			 
		 }
		
 	if(ch4_time>=sumi_y2){
			ch4_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		 // set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		
		 set_ch4_sinc();
		
		
		}
		


	}
	
	if(set_ch4_bo==4)  //设置为按摩波E1波的输出  变化规律先发周期0.01的50个正波，再发周期0.01的50个负波，
	{
		
		//ch2_unm_E1_t
		
		
		 sumi_x1=(ch4_unm_E1_t-5);
		 sumi_x2=(sumi_x1+ch4_unm_E1_t);

     sumi_y1=(ch4_unm_E1_t+5);
		 sumi_y2=(sumi_y1+ch4_unm_E1_t);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2通道负脉冲输出 
			 
		 }
		
		
		if(ch4_time>=sumi_y2){
			ch4_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
	//	  set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		
		}
		


		
		
	}
	
	if(set_ch4_bo==5) //按摩波E2波输出波形
	{
				if(ch4_unm_E2_x1<=1200){
				 if(ch4_time==5){
				 ch_X_set_ms(4,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch4_unm_E2_x1>1200){
					
					if(ch4_time==5){
				 ch_X_set_ms(4,4);  //2通道负脉冲输出 
				 
			 }}
				
			ch4_unm_E2_x1++;
			if(ch4_unm_E2_x1>=2401) ch4_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
			if(ch4_time>=20) ch4_time=0;  //控制周期输出T的值   

	}
	
  if(set_ch4_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		
		
		 sumi_x1=(ch4_unm_E3_t-5);
		 sumi_x2=(sumi_x1+ch4_unm_E3_t);

     sumi_y1=(ch4_unm_E3_t+5);
		 sumi_y2=(sumi_y1+ch4_unm_E3_t);

		if((ch4_time==sumi_x1)||(ch4_time==sumi_x2))	 
		 {
		 ch_X_set_ms(4,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch4_time==sumi_y1)||(ch4_time==sumi_y2))
		{
			 
	    ch_X_set_ms(4,2);  //2通道负脉冲输出 
			 
		 }
		
		
	if(ch4_time>=sumi_y2){
		ch4_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
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


/////////////通道3 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch3_pin(void)
{
	if(set_ch3_bo==1)   //设置为连续波
	{
	 if(ch3_time==4){
		ch_X_set_ms(3,3);  //6通道正脉冲输出 
	 }
	if(ch3_time>=ch3_unm) ch3_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch3_bo==2)
 { 
	 

		 
		 sumi_x1=(ch3_unm-6);
		 sumi_x2=(sumi_x1+ch3_unm);

     sumi_y1=(ch3_unm+4);
		 sumi_y2=(sumi_y1+ch3_unm);

		 
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
	    ch_X_set_ms(3,2);  //3通道负脉冲输出 
			 
		 }
		
//对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch3_time>=sumi_y2){
			 ch3_time=2;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch3_shu_mi++;
			 //周期等于实际输出周期
			 if(ch3_shu_mi<40){
				if(ch3_bool){
				 ch3_unm=ch3_unm;

				}			
			 }
			 //周期等于实际周期的的4倍；
			  if(ch3_shu_mi==40){
				
				 ch3_unm=(ch3_unm*4);
				
			 }
			 
			 if(ch3_shu_mi>=80){
				 ch3_shu_mi=0;
				// set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				  set_ch3_sinc();
				 
			 }
			 
			 
			 
		 }
		 
		 
		 
		 
		 
		 
		 
	}
	
	if(set_ch3_bo==3)    //轻捶波输出
	{
		

		
	   sumi_x1=(ch3_unm-6);
		 sumi_x2=(sumi_x1+ch3_unm);

     sumi_y1=(ch3_unm+4);
		 sumi_y2=(sumi_y1+ch3_unm);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2通道负脉冲输出 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		//  set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		set_ch3_sinc();
			
			
		}
		
		
		
		
		
	}
	
	if(set_ch3_bo==4)  //设置为按摩波E1波的输出  变化规律先发周期0.01的50个正波，再发周期0.01的50个负波，
	{
		
		//ch2_unm_E1_t
		

		
		 sumi_x1=(ch3_unm_E1_t-6);
		 sumi_x2=(sumi_x1+ch3_unm_E1_t);

     sumi_y1=(ch3_unm_E1_t+4);
		 sumi_y2=(sumi_y1+ch3_unm_E1_t);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2通道负脉冲输出 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		//  set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		
		}
		
		
		
		
	}
	
	if(set_ch3_bo==5) //按摩波E2波输出波形
	{
		    ch3_unm_E2_x1++;
	     if(ch3_unm_E2_x1<=1200){
				 if(ch3_time==4){
				 ch_X_set_ms(3,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			  }
		    }
	       if(ch3_unm_E2_x1>1200){
					if(ch3_time==4){
				 ch_X_set_ms(3,4);  //2通道负脉冲输出 
				 
			 }
			}
				
			
	   if(ch3_unm_E2_x1>=2401) ch3_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
	   if(ch3_time>=20) ch3_time=0;  //控制周期输出T的值   

	}
	
	if(set_ch3_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		
		 sumi_x1=(ch3_unm_E3_t-6);
		 sumi_x2=(sumi_x1+ch3_unm_E3_t);

     sumi_y1=(ch3_unm_E3_t+4);
		 sumi_y2=(sumi_y1+ch3_unm_E3_t);
		
		if((ch3_time==sumi_x1)||(ch3_time==sumi_x2))	 
		 {
			ch_X_set_ms(3,1);  //3通道正脉冲输出 
			 
		 }
	   if((ch3_time==sumi_y1)||(ch3_time==sumi_y2))
		{
			 
			ch_X_set_ms(3,2);  //2通道负脉冲输出 	
			 
		 }
		
		if(ch3_time>=sumi_y2){
			ch3_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
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


/////////////通道1 输出波形函数 /////////////////////
///set_bo为设置的什么波形1：连续波  2：疏密波    3:轻捶波   4:按摩波E1  5:按摩波E2   6:按摩波E3  

void layout_ch1_pin(void)
{
	if(set_ch1_bo==1)   //设置为连续波
	{
	 if(ch1_time==3){
		ch_X_set_ms(1,3);  //1通道正脉冲输出 
	 }
	if(ch1_time>=ch1_unm) ch1_time=0;  //控制周期输出T的值   
	 	 
	}
	
 //设置为疏密波   连续发4个密波   一个高电平10ms，低电脉冲10ms  周期T  再发4个    过4T的时间接着发密波4个  一个T   
	if(set_ch1_bo==2)
 { 


     sumi_x1=(ch1_unm-7);
		 sumi_x2=(sumi_x1+ch1_unm);

     sumi_y1=(ch1_unm+3);
		 sumi_y2=(sumi_y1+ch1_unm);

		 
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2通道负脉冲输出 	 
			 
		 }
		
//		 //对通道1计数变量进行清零操作，对输出脉冲数据进行加一操作。控制疏密输出的密波数与疏波数的个数。
		 if(ch1_time>=sumi_y2){
			 ch1_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
			 ch1_shu_mi++;
			 //周期等于实际输出周期
			 if(ch1_shu_mi<40){
				if(ch1_bool){
				 ch1_unm=ch1_unm;

				}
				
				// ch1_bool=0;
				
				
			 }
			 //周期等于实际周期的的4倍；
			  if(ch1_shu_mi==40){
				
				//if(!ch1_bool){
				 ch1_unm=(ch1_unm*4);
				//	ch1_bool=2;
				//}
				
			 }
			 
			 if(ch1_shu_mi>=80){
				 ch1_shu_mi=0;
				// set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				set_ch1_sinc(); 
				 
			 }
			 
			 
			 
		 }
		 
		 
		 
		 
		 
		 
		 		
	}
	
	if(set_ch1_bo==3)    //轻捶波输出
	{

		
		 sumi_x1=(ch1_unm-7);
		 sumi_x2=(sumi_x1+ch1_unm);

     sumi_y1=(ch1_unm+3);
		 sumi_y2=(sumi_y1+ch1_unm);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2通道负脉冲输出 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
			ch1_time=4;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
	//	  set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		set_ch1_sinc(); 
		}
		
		
		
		
	}
	
	if(set_ch1_bo==4)  //设置为按摩波E1波的输出  周期在0.56s到0.02s其周期变化的轻捶波组成，
	{
		
		//ch2_unm_E1_t

		
		 sumi_x1=(ch1_unm_E1_t-7);
		 sumi_x2=(sumi_x1+ch1_unm_E1_t);

     sumi_y1=(ch1_unm_E1_t+3);
		 sumi_y2=(sumi_y1+ch1_unm_E1_t);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2通道负脉冲输出 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
			ch1_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		 // set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
		
		}
		
		
		
		
		
		
		
	}
	
	if(set_ch1_bo==5) //按摩波E2波输出波形
	{
				if(ch1_unm_E2_x1<=1200){
				 if(ch1_time==3){
				 ch_X_set_ms(1,3);  //2通道正脉冲输出 
				 //ch2_unm_E2_x1++;

			 }
		 }
				if(ch1_unm_E2_x1>1200){
					
					if(ch1_time==3){
				 ch_X_set_ms(1,4);  //2通道负脉冲输出 
				 
			 }}
				
			ch1_unm_E2_x1++;
			if(ch1_unm_E2_x1>=2401) ch1_unm_E2_x1=0;  //输出口脉冲数量控制变量置为0；
			if(ch1_time>=20) ch1_time=0;  //控制周期输出T的值   

	}
	
	if(set_ch1_bo==6)   //按摩波E3波周期从0.1缩小到0.01 的轻捶波每10个脉冲周期后将周期缩小0.01s
	{    
		  //ch2_unm_E3_t
		
		 sumi_x1=(ch1_unm_E3_t-7);
		 sumi_x2=(sumi_x1+ch1_unm_E3_t);

     sumi_y1=(ch1_unm_E3_t+3);
		 sumi_y2=(sumi_y1+ch1_unm_E3_t);
		
		if((ch1_time==sumi_x1)||(ch1_time==sumi_x2))	 
		 {
			 ch_X_set_ms(1,1);  //2通道正脉冲输出 
			 
		 }
	   if((ch1_time==sumi_y1)||(ch1_time==sumi_y2))
		{
			 
			 ch_X_set_ms(1,2);  //2通道负脉冲输出 	 
			 
		 }
		
		if(ch1_time>=sumi_y2){
		
		ch1_unm_E3_x1++;
		ch1_time=0;    //起始变量数据决定下次从什么时候开始输出正脉冲时间
		
		}
		
		if(ch1_unm_E3_x1>=5) { 
			
		ch1_unm_E3_t=(ch1_unm_E3_t-10);
			
    if(	ch1_unm_E3_t<=10){
    ch1_unm_E3_t=100;
		}			

		ch1_unm_E3_x1=0;   }
		
		
		
		

		
		
		
	}
	
	
	
}
















//给4G模块关机
 void OFF_4G()
	 {
	 MOD_4G=0;
	 delay_ms(200);
	 MOD_4G=1;
	 
 }
 
 //给4G模块关机
 void ON_4G(){
	 MOD_4G=0;
	 delay_ms(200);
	 MOD_4G=1;
	 
 }
 