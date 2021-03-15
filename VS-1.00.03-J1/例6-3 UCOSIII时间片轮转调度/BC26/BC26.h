#ifndef __BC26_H
#define	__BC26_H

#include "sys.h"
u8 BC20_init(void);
u8 EC20_CSQ_State(void);
u8 ASCII_u16(u8 data);
int get_ip();
int get_ip_not();
uint16_t get_B_ip(u8 NL660_ip_0,u8 NL660_ip_1,u8 NL660_ip_2,u8 NL660_ip_3,u8 NL660_ip_4,u8 NL660_ip_5);
void send_A2_date(void);
#endif