#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

////IO方向设置
//#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=8<<8;}
//#define DS18B20_IO_OUT() {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=3<<8;}

//原理图标号是PE0 加热键的行码 实际接的是单片机的PE0口
#define HOT_IO_IN()  {GPIOE->CRL&=0XFFFFFFF0;GPIOE->CRL|=8;} //上拉输入
#define HOT_IO_OUT() {GPIOE->CRL&=0XFFFFFFF0;GPIOE->CRL|=3;}  //推挽输出

//原理图标号是PE6 加电脉冲的行码 实际接的是单片机的PE6口
#define SELET_IO_IN()  {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=8<<24;}
#define SELET_IO_OUT() {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=3<<24;}

//原理图标号是PD9 设置的行码 实际接的是单片机的PD9口   
#define SET_IO_IN()   {GPIOD->CRH&=0XFFFFFF0F;GPIOD->CRH|=8<<4;}
#define SET_IO_OUT()  {GPIOD->CRH&=0XFFFFFF0F;GPIOD->CRH|=3<<4;}



//原理图标号是KOY 6 设置的列码6位  实际接的是单片机的PC8  
#define KOY6_IO_IN()  {GPIOC->CRH&=0XFFFFFFF0;GPIOC->CRH|=8;}
#define KOY6_IO_OUT() {GPIOC->CRH&=0XFFFFFFF0;GPIOC->CRH|=3;}

//原理图标号是KOY 5 设置的列码5位  实际接的是单片机的PC9  
#define KOY5_IO_IN()  {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=8<<4;}
#define KOY5_IO_OUT() {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=3<<4;}

//原理图标号是KOY 4 设置的列码4位  实际接的是单片机的PD2  
#define KOY4_IO_IN()  {GPIOD->CRL&=0XFFFFF0FF;GPIOD->CRL|=8<<8;}
#define KOY4_IO_OUT() {GPIOD->CRL&=0XFFFFF0FF;GPIOD->CRL|=3<<8;}


//原理图标号是KOY 3 设置的列码3位  实际接的是单片机的PD3  
#define KOY3_IO_IN()  {GPIOD->CRL&=0XFFFF0FFF;GPIOD->CRL|=8<<12;}
#define KOY3_IO_OUT() {GPIOD->CRL&=0XFFFF0FFF;GPIOD->CRL|=3<<12;}

//原理图标号是KOY 2 设置的列码2位  实际接的是单片机的PD4  
#define KOY2_IO_IN()  {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=8<<16;}
#define KOY2_IO_OUT() {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=3<<16;}

//原理图标号是KOY 1 设置的列码1位  实际接的是单片机的PD5  
#define KOY1_IO_IN()  {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=8<<20;}
#define KOY1_IO_OUT() {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=3<<20;}


//#define	DS18B20_DQ_OUT PEout(2) //数据端口	PA0 
//#define	DS18B20_DQ_IN  PEin(2)  //数据端口	PA0 

#define	HOT_DQ_OUT PEout(0) //行加热码 
#define	HOT_DQ_IN  PEin(0)  //行加热码

#define	SELET_DQ_OUT PEout(6) //行电脉冲
#define	SELET_DQ_IN  PEin(6)  //行电脉冲

#define	SET_DQ_OUT PDout(9) //行设置码
#define	SET_DQ_IN  PDin(9)  //行设置码


#define	KOY6_DQ_OUT PCout(8) //行加热码 
#define	KOY6_DQ_IN  PCin(8)  //行加热码

#define	KOY5_DQ_OUT PCout(9) //行加热码 
#define	KOY5_DQ_IN  PCin(9)  //行加热码

#define	KOY4_DQ_OUT PDout(2) //行加热码 
#define	KOY4_DQ_IN  PDin(2)  //行加热码

#define	KOY3_DQ_OUT PDout(3) //行加热码 
#define	KOY3_DQ_IN  PDin(3)  //行加热码

#define	KOY2_DQ_OUT PDout(4) //行加热码 
#define	KOY2_DQ_IN  PDin(4)  //行加热码

#define	KOY1_DQ_OUT PDout(5) //行加热码 
#define	KOY1_DQ_IN  PDin(5)  //行加热码

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP




 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define KEY2_PRES	  3	//KEY2按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP/KEY_UP)


typedef enum {FALSE = 0 ,TRUE = 1} bool;

void set_ch1_6_to_display(void);
u8 CK_ELE(u8 key);
u8 get_set_num(void);
u8 get_hot1_6_num(void);
u8 get_eleck1_6_num(void);
void save_ch1_6_24c02(void);
void read_ch1_6_at2402(void);

u8 Start_KEY_Scan_dian(void);
u8 KEY_Scan_dian(void);
void disply_time(void);
int key_set(void);

u8 key_start(u8 i); 
int key_red_H(void);  //扫描行码数值
int key_red(void);
 
void KEY_Init(void);//IO初始化
u8 KEY_Scan(void );  	//按键扫描函数				

void set_ch1_6_sinc(void);
void fa_set_elect(void);

void set_ch1_sinc(void);
void set_ch2_sinc(void);
void set_ch3_sinc(void);
void set_ch4_sinc(void);
void set_ch5_sinc(void);
void set_ch6_sinc(void);

#endif
