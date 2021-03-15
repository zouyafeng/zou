#ifndef __USART3_H
#define __USART3_H 
#include "sys.h"
#include "stdio.h"	  
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

#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1				      	//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern vu16 USART3_RX_STA;   						//��������״̬

void usart3_init(u32 bound);
void u3_printf(char* fmt,...);
void USART3_SendByte(u8 Data);
void USART3_SendString(u8* Data,u32 Len);
void USART3_DMAS(u8* Data);
void clean_rebuff3(void);
uint8_t USART3_GET(void);
unsigned char *get_rebuff3(uint8_t *len);
uint8_t Check_HD(unsigned char *addr);
uint8_t Check_CK(unsigned char *addr);
uint8_t Type_Get();
unsigned char Q24_CheckSum(unsigned char str[], int n);
int chenk_date(unsigned char *str, u8 che , int k );

uint8_t BLE_Check(void);

void Flash_write_buf(u32 addr,u8 *p,u16 n);
void Flash_read_buf(u32 addr,u8 *p,u16 n);
void read_apn_power(void);

int zh_send_a2_sidix(void);

int zh_send_a2(void);
u8 send_server_data(void);


u8 TX_CheckSum(u8 *buf, u8 len);


#endif	   
















