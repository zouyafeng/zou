#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
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
#define LED_R PDout(11)  // PB5红灯
#define LED_G PDout(8)  // PE5	蓝灯
#define REST  PCout(3)  // PE5	蓝灯
#define MOD_4G PCout(3) //4G模块开关机引脚 

#define PWOR_SHU  PGout(9) // 抽水 到营养液混合槽里
#define PWOR_YANG PGout(8) // 营养也抽到栽培槽里 
#define PWOR_YINGYANG  PGout(7) // 将营养液抽到混合液中

#define PWOR_WENG  PGout(6)

//#define LED_1     PDout(9)  //日光灯第一排
#define LED_2     PDout(8)  //日光灯第二排
#define LED_3     PDout(7)  //日光灯第三排
#define LED_4     PDout(6)  //日光灯第四排

#define MCU_MUS     PDout(0)  //日光灯第四排

#define MCU_H1     PCout(7)
#define MCU_H2     PCout(6)
#define MCU_H3     PDout(15)
#define MCU_H4     PDout(14)
#define MCU_H5     PDout(13)
#define MCU_H6     PDout(12)

#define feng_1     PEout(2)  //日光灯第一排
#define feng_2     PEout(3)  //日光灯第二排
#define feng_3     PEout(4)  //日光灯第三排
#define feng_4     PEout(5)  //日光灯第四排
//#define feng_5     PEout(6)  //日光灯第四排
#define feng_6     PEout(1)  //日光灯第四排


#define MCU_O1     PBout(9)  //第1通道输出电脉冲GPIO 
#define MCU_O2     PBout(8)  //第2通道输出电脉冲GPIO 
#define MCU_O3     PBout(4)  //第3通道输出电脉冲GPIO 
#define MCU_O4     PBout(3)  //第4通道输出电脉冲GPIO 
#define MCU_O5     PDout(7)  //第5通道输出电脉冲GPIO 
#define MCU_O6     PDout(6)  //第6通道输出电脉冲GPIO 


#define INH1      PDout(1)  //第一片4051的使能管脚
#define INH2      PDout(0)  //第二片4051的使能管脚
#define DC4051_A      PCout(10) //DC4051选择器A的接入端
#define DC4051_B      PCout(11) //DC4051选择器B的接入端
#define DC4051_C      PCout(12) //DC4051选择器C的接入端

#define LEEP      PBout(14)    //蜂鸣器控制端口

#define OE_1         PAout(8)    //OE
#define POWER_1      PDout(11)    //POWER 12V控制供电管脚


void layout_ch6_pin(void);
void layout_ch5_pin(void);
void layout_ch4_pin(void);
void layout_ch3_pin(void);
void layout_ch2_pin(void);
void layout_ch1_pin(void);


void off_NO1_4051(void);
void close_4051(void);

void off_4051_int(void);
void set_ms(void);
void star_4051(u8 pin,u8 xin);

void ch_X_set_ms(u8 ch, u8 h);
void texi_ch2(void);
void on_leep(void);  //打开蜂鸣器


void LED_Init(void);//初始化
void OFF_4G();
void ON_4G();
		 				    
#endif
