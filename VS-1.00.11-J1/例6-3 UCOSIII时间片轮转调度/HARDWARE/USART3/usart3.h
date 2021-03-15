#ifndef __USART3_H
#define __USART3_H 
#include "sys.h"
#include "stdio.h"	  
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

#define USART3_MAX_RECV_LEN		600					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		600					//最大发送缓存字节数
#define USART3_RX_EN 			1				      	//0,不接收;1,接收.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern vu16 USART3_RX_STA;   						//接收数据状态

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
















