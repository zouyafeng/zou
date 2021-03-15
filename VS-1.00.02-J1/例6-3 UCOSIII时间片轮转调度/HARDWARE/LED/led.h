#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED_R PDout(11)  // PB5���
#define LED_G PDout(8)  // PE5	����
#define REST  PCout(3)  // PE5	����
#define MOD_4G PCout(3) //4Gģ�鿪�ػ����� 

#define PWOR_SHU  PGout(9) // ��ˮ ��Ӫ��Һ��ϲ���
#define PWOR_YANG PGout(8) // Ӫ��Ҳ�鵽������� 
#define PWOR_YINGYANG  PGout(7) // ��Ӫ��Һ�鵽���Һ��

#define PWOR_WENG  PGout(6)

//#define LED_1     PDout(9)  //�չ�Ƶ�һ��
#define LED_2     PDout(8)  //�չ�Ƶڶ���
#define LED_3     PDout(7)  //�չ�Ƶ�����
#define LED_4     PDout(6)  //�չ�Ƶ�����

#define MCU_MUS     PDout(0)  //�չ�Ƶ�����

#define MCU_H1     PCout(7)
#define MCU_H2     PCout(6)
#define MCU_H3     PDout(15)
#define MCU_H4     PDout(14)
#define MCU_H5     PDout(13)
#define MCU_H6     PDout(12)

#define feng_1     PEout(2)  //�չ�Ƶ�һ��
#define feng_2     PEout(3)  //�չ�Ƶڶ���
#define feng_3     PEout(4)  //�չ�Ƶ�����
#define feng_4     PEout(5)  //�չ�Ƶ�����
//#define feng_5     PEout(6)  //�չ�Ƶ�����
#define feng_6     PEout(1)  //�չ�Ƶ�����


#define MCU_O1     PBout(9)  //��1ͨ�����������GPIO 
#define MCU_O2     PBout(8)  //��2ͨ�����������GPIO 
#define MCU_O3     PBout(4)  //��3ͨ�����������GPIO 
#define MCU_O4     PBout(3)  //��4ͨ�����������GPIO 
#define MCU_O5     PDout(7)  //��5ͨ�����������GPIO 
#define MCU_O6     PDout(6)  //��6ͨ�����������GPIO 


#define INH1      PDout(1)  //��һƬ4051��ʹ�ܹܽ�
#define INH2      PDout(0)  //�ڶ�Ƭ4051��ʹ�ܹܽ�
#define DC4051_A      PCout(10) //DC4051ѡ����A�Ľ����
#define DC4051_B      PCout(11) //DC4051ѡ����B�Ľ����
#define DC4051_C      PCout(12) //DC4051ѡ����C�Ľ����

#define LEEP      PBout(14)    //���������ƶ˿�

#define OE_1         PAout(8)    //OE
#define POWER_1      PDout(11)    //POWER 12V���ƹ���ܽ�


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
void on_leep(void);  //�򿪷�����


void LED_Init(void);//��ʼ��
void OFF_4G();
void ON_4G();
		 				    
#endif
