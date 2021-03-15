#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
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

////IO��������
//#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=8<<8;}
//#define DS18B20_IO_OUT() {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=3<<8;}

//ԭ��ͼ�����PE0 ���ȼ������� ʵ�ʽӵ��ǵ�Ƭ����PE0��
#define HOT_IO_IN()  {GPIOE->CRL&=0XFFFFFFF0;GPIOE->CRL|=8;} //��������
#define HOT_IO_OUT() {GPIOE->CRL&=0XFFFFFFF0;GPIOE->CRL|=3;}  //�������

//ԭ��ͼ�����PE6 �ӵ���������� ʵ�ʽӵ��ǵ�Ƭ����PE6��
#define SELET_IO_IN()  {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=8<<24;}
#define SELET_IO_OUT() {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=3<<24;}

//ԭ��ͼ�����PD9 ���õ����� ʵ�ʽӵ��ǵ�Ƭ����PD9��   
#define SET_IO_IN()   {GPIOD->CRH&=0XFFFFFF0F;GPIOD->CRH|=8<<4;}
#define SET_IO_OUT()  {GPIOD->CRH&=0XFFFFFF0F;GPIOD->CRH|=3<<4;}



//ԭ��ͼ�����KOY 6 ���õ�����6λ  ʵ�ʽӵ��ǵ�Ƭ����PC8  
#define KOY6_IO_IN()  {GPIOC->CRH&=0XFFFFFFF0;GPIOC->CRH|=8;}
#define KOY6_IO_OUT() {GPIOC->CRH&=0XFFFFFFF0;GPIOC->CRH|=3;}

//ԭ��ͼ�����KOY 5 ���õ�����5λ  ʵ�ʽӵ��ǵ�Ƭ����PC9  
#define KOY5_IO_IN()  {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=8<<4;}
#define KOY5_IO_OUT() {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=3<<4;}

//ԭ��ͼ�����KOY 4 ���õ�����4λ  ʵ�ʽӵ��ǵ�Ƭ����PD2  
#define KOY4_IO_IN()  {GPIOD->CRL&=0XFFFFF0FF;GPIOD->CRL|=8<<8;}
#define KOY4_IO_OUT() {GPIOD->CRL&=0XFFFFF0FF;GPIOD->CRL|=3<<8;}


//ԭ��ͼ�����KOY 3 ���õ�����3λ  ʵ�ʽӵ��ǵ�Ƭ����PD3  
#define KOY3_IO_IN()  {GPIOD->CRL&=0XFFFF0FFF;GPIOD->CRL|=8<<12;}
#define KOY3_IO_OUT() {GPIOD->CRL&=0XFFFF0FFF;GPIOD->CRL|=3<<12;}

//ԭ��ͼ�����KOY 2 ���õ�����2λ  ʵ�ʽӵ��ǵ�Ƭ����PD4  
#define KOY2_IO_IN()  {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=8<<16;}
#define KOY2_IO_OUT() {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=3<<16;}

//ԭ��ͼ�����KOY 1 ���õ�����1λ  ʵ�ʽӵ��ǵ�Ƭ����PD5  
#define KOY1_IO_IN()  {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=8<<20;}
#define KOY1_IO_OUT() {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=3<<20;}


//#define	DS18B20_DQ_OUT PEout(2) //���ݶ˿�	PA0 
//#define	DS18B20_DQ_IN  PEin(2)  //���ݶ˿�	PA0 

#define	HOT_DQ_OUT PEout(0) //�м����� 
#define	HOT_DQ_IN  PEin(0)  //�м�����

#define	SELET_DQ_OUT PEout(6) //�е�����
#define	SELET_DQ_IN  PEin(6)  //�е�����

#define	SET_DQ_OUT PDout(9) //��������
#define	SET_DQ_IN  PDin(9)  //��������


#define	KOY6_DQ_OUT PCout(8) //�м����� 
#define	KOY6_DQ_IN  PCin(8)  //�м�����

#define	KOY5_DQ_OUT PCout(9) //�м����� 
#define	KOY5_DQ_IN  PCin(9)  //�м�����

#define	KOY4_DQ_OUT PDout(2) //�м����� 
#define	KOY4_DQ_IN  PDin(2)  //�м�����

#define	KOY3_DQ_OUT PDout(3) //�м����� 
#define	KOY3_DQ_IN  PDin(3)  //�м�����

#define	KOY2_DQ_OUT PDout(4) //�м����� 
#define	KOY2_DQ_IN  PDin(4)  //�м�����

#define	KOY1_DQ_OUT PDout(5) //�м����� 
#define	KOY1_DQ_IN  PDin(5)  //�м�����

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP




 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	  2	//KEY1����
#define KEY2_PRES	  3	//KEY2����
#define WKUP_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)


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
int key_red_H(void);  //ɨ��������ֵ
int key_red(void);
 
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void );  	//����ɨ�躯��				

void set_ch1_6_sinc(void);
void fa_set_elect(void);

void set_ch1_sinc(void);
void set_ch2_sinc(void);
void set_ch3_sinc(void);
void set_ch4_sinc(void);
void set_ch5_sinc(void);
void set_ch6_sinc(void);

#endif
