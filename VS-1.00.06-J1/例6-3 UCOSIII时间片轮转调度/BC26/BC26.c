#include "sys.h"
#include "BC26.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "usart.h"	
#include "delay.h"
#include "led.h"
#include "math.h"
#include "usart3.h"	  
typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;
extern uint8_t U2_P;
extern unsigned char U2BUF[256];

u8 phone_sevie_num[16];
u8 beibuf[11]={0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x20,0x4f,0x4b};
u8 zaoshenbuf_1[]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x00,0x00,0xFA,0x0F,
0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x01,0x00,0xFB,0x11,
0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x02,0x00,0xFC,0x13,
0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x03,0x00,0xFC,0x15,
0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x05,0x00,0xFF,0x19,
0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x21,0xAF};

u8 zaoshenbuf_6[]={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x21,0xAF};

	
	
	
u8 xiaoqu[5];
u8 xinhao[8];
u8 pdp_name[30];
u8 pdp_power[30];
u8 pdp_apn[20];

extern u8 NL660_ip[4];
extern u8 sever_2_buf[];
extern u8 system_int;
u8 Objective_ip_1[4]={0};
u8 Objective_ip_2[4]={0};
u8 Objective_ip_3[4]={0};
u8 Objective_ip_4[4]={0};

u8 Objective_ip_B_1[4]={0};
u8 Objective_ip_B_2[4]={0};
u8 Objective_ip_B_3[4]={0};
u8 Objective_ip_B_4[4]={0};

extern u8 ietnet_stay;

u8 ietnet_a=0;
u8 ietnet_b=0;
extern u8 read_jem_p, read_jem_g, read_jem_j;
extern u8 jh_boot;

extern u8 red_pdp_name[30];
extern u8 red_pdp_power[30];
extern u8 red_pdp_apn[20];

extern u8 zd_che_net;  //Ö÷¶¯²éÍøµÄ¹¦ÄÜ

extern u8 work_stay;    //¹¤×÷×´Ì¬  
extern u8 ietnet_stay;  //ÍøÂç×´Ì

/*
**************************************************************************************
*	ASCII×ª»»³É16½øÖÆÊý¾Ý
**************************************************************************************
*/
u8 ASCII_u16(u8 data)
{
	if((data >= '0')&&(data<='9'))
	{		
		data = data - 0x30;
	}
	else if((data >= 'A')&&(data<='F'))
	{
		data = data - 'A' + 0x0A;
	}
	return data;
}


u8 BC20_init(void)
{
	u8 i,CSQ,a,k,j;
	uint16_t BeTime;
	unsigned char *cse;
	unsigned char *bc;
	//²é¿¨ÊÇ·ñÔÚÉÏÃæ
	LED_R=1; 
//	LED_G=0; //¿ªÂÌµÆ
	OFF_4G();
	
	
	for(i=0;i<10;i++)	
	{			
		USART2_DMASS("AT+CPIN?\r\n","OK",1000,100);
	//	USART2_printf(USART2,"AT+CPIN?\r\n");
		if(strstr(U2BUF,"+CPIN: READY")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
			i=10;
		}
		//USART3_DMAS(U2BUF);
		delay_ms(1000);
	}
	LED_R=0; //¹Ø±ÕºìµÆ
	LED_G=1; //¿ªÂÌµÆ
	
//	
//	//CSQ
////	for(i=0;i<150;i++)
////	{
//		 //GPRSÄ£¿éCSQÖµ
	
		while(!((CSQ<32)&&(CSQ>5)))  
		{
			//i=150;
			CSQ = EC20_CSQ_State();
		}
		
		
		
////		delay_ms(200);
////	}
//	

		//CEREG	
	USART2_DMASS("AT+CREG=2\r\n","OK",1000,500);
	
	for(i=0;i<150;i++)
	{
		clean_rebuff2();
	  USART2_DMASS("AT+CREG?\r\n","OK",1000,500);
//		if(strstr(U2BUF,"+CREG: 2,1")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
//		{
//			//+CREG: 2,1,"10D0","117050C",7
//			cse=strstr(U2BUF,"+CREG:");
//	    ietnet_a=1;
//			cse=cse+12;
//			for(k=0;k<4;k++){
//				xiaoqu[k]=cse[k];
//				USART2_SendByte(xiaoqu[k]);
//			}
//			cse=cse+7;
//			
//			for(k=0;k<7;k++){
//				xinhao[k]=cse[k];
//				USART2_SendByte(xinhao[k]);
//				
//			}
//			
//			i=150;
//		}

		if(strstr(U2BUF,"+CREG: 2,5,")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
			i=150;
			ietnet_stay=0x01;
		}
			if(strstr(U2BUF,"+CREG: 2,1,")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
			i=150;
			ietnet_stay=0x01;
		}
		
		//i=150;
		delay_ms(200);
		
	}
	
	clean_rebuff2();
  USART2_DMASS("AT+CGATT=1\r\n","OK",1000,500);
	delay_ms(100);
	
	if(strstr(U2BUF,"OK")!=NULL)
	{
		jh_boot=2;    //´ò¿ªLEDµÈÉÁË¸ÕÒµ½ÍøÂçÁË
	}
	
		for(i=0;i<5;i++)
	{
	  USART2_DMASS("AT+QIACT=1\r\n","OK",1000,100);
	
	  delay_ms(500);
		USART2_DMAS("AT+QIOPEN=1,0,");	
		USART2_SendByte('"');
		USART2_DMAS("TCP");
		USART2_SendByte('"');
	  USART2_DMAS(",");
		USART2_SendByte('"');
		USART2_DMAS("106.14.57.96");
		USART2_SendByte('"');
		USART2_DMAS(",20000,0,2");
		USART2_DMAS("\r\n");
		delay_ms(1500);
		
		if(i==5){
			REST=0;
			delay_ms(500);  //¸´Î»¹Ü½ÅÁ¬½ÓPC7  
			REST=1;}
		
		if(strstr(U2BUF,"CONNECT")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
			i=5;
			system_int=1; //Á¬½ÓÍøÂç³É¹¦
		}
		
		
			
			
	}	
		
//	delay_ms(500);
//	delay_ms(500);
//    send_server_data();
//	  send_server_data();
	clean_rebuff2();
	clean_rebuff2();
	
	//	system_int=1;
	
	
	
}



void send_A2_date(void)
{
	int i,j,k;
	
						sever_2_buf[5]=work_stay;
						sever_2_buf[6]=ietnet_stay;
	

	
	                sever_2_buf[7]=read_jem_p;
									//È¡pdpµÄÓÃ»§Ãû
									for(i=0;i<read_jem_p;i++)
									{
									sever_2_buf[8+i]=red_pdp_name[i];
									
									}
									j=8+read_jem_p;
									//È¡ÃÜÂë
									sever_2_buf[j]=read_jem_g;
									for(i=0;i<read_jem_g+1;i++){
									 sever_2_buf[j+1+i]=red_pdp_power[i];
									}
									j=j+read_jem_g+1;
									//È¥APNºÅ
									sever_2_buf[j]=read_jem_j;
									for(i=0;i<read_jem_j+1;i++){
										sever_2_buf[j+1+i]=red_pdp_apn[i];
									}
						
						      j=j+read_jem_j+1;
	
											//È¡ipµØÖ·¸ø·µ»ØÖ÷»úµÄÊý×é
						for(i=0;i<5;i++){
							sever_2_buf[j+i]=NL660_ip[i];
						}
						j=j+5;
						sever_2_buf[j-1]=Q24_CheckSum(sever_2_buf,j-1);
						
						sever_2_buf[3]=j; //¸³ÖµÖ¡³¤Êý¾Ý 
						
						for(i=0;i<j;i++){
							
							USART3_SendByte(sever_2_buf[i]);
							
						}
									
	
}





/*
**************************************************************************************
*	GPRSÄ£¿éCSQÖµ
**************************************************************************************
*/
u8 EC20_CSQ_State(void)
{
	u8 i,CSQ=0;
	unsigned char *gps;
	
	USART2_DMASS("AT+CSQ\r\n","OK",1000,300);//
	
//#if USART3_DEBUG
//	USART3_DMAS(U2BUF);
//#endif
	
	if(strstr(U2BUF,"+CSQ:")!=NULL)
	{
		gps = strstr(U2BUF,"+CSQ:");
		gps += 6; 
		i=0;
		do{
			i++;
		}while(gps[i]!=',' && i<2);
		
		switch (i){			
			case 2:
				CSQ = (gps[0] - 0x30) * 10 + gps[1] - 0x30;
				break;		
			case 1:
				CSQ = gps[0] - 0x30;
				break;		
			default:
				break;
		}
		
	}
	return CSQ;
}
//µÃµ½IPµÄµÄº¯Êý
//·µ»ØÖµ£¬1£ºµÃµ½ÁËIPµØÖ·
 //       0£ºÃ»ÓÐµÃµ½IP
int get_ip()
{
	int i,j,k;
	unsigned char *bc;
	
	  USART2_DMASS("AT+QIACT?\r\n","OK",1000,500);
		if(strstr(U2BUF,"+QIACT: 1,1,1")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
		
			bc = strstr(U2BUF,"+QIACT: 1,1,1,");
			//ÌáÆðIP
			bc=bc+14; 
     // USART2_printf( USART2, "\r\n iiiiiiiiiiiiiiiiiiiiiiiiis %s \r\n",bc);
			i=0;
			for(j=0;j<4;j++)
			{ 
				bc += (i+1); 
				i=0;					
				do{
					i++;
				}while(bc[i]!='.' && bc[i]!='\0' && bc[i] != 0x22 && i<3);
				
				switch (i){
					case 3:
							NL660_ip[j] = ASCII_u16(bc[0]) * 100 + ASCII_u16(bc[1]) *10 + ASCII_u16(bc[2]);
						break;					
					case 2:		
							NL660_ip[j] = ASCII_u16(bc[0]) *10 + ASCII_u16(bc[1]);
						break;
					case 1:		
							NL660_ip[j] = ASCII_u16(bc[0]);
						break;
					default:
						break;
				}	
				
				
			}
			
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[0]);
//			USART2_printf( USART2, "\r\n  is %d \r\n",NL660_ip[1]);
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[2]);
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[3]);
		
		if(NL660_ip[0] >= 100)
		{
			Objective_ip_1[0] = (NL660_ip[0]/100) + 0x30;
			Objective_ip_1[1] = ((NL660_ip[0]%100)/10) + 0x30;
			Objective_ip_1[2] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[3] = '\0';
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=Objective_ip_1[2];
//			ip_writ_add[3]=0x2e;
		}
		else if((NL660_ip[0]< 100) && (NL660_ip[0] >= 10))
		{
			Objective_ip_1[0] = (NL660_ip[0]/10) + 0x30;
			Objective_ip_1[1] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[2] = '\0';	
			
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=0x2e;
			
		}
		else
		{
			Objective_ip_1[0] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[1] = '\0';		
		}

		if(NL660_ip[1] >= 100)
		{
			Objective_ip_2[0] = (NL660_ip[1]/100) + 0x30;
			Objective_ip_2[1] = ((NL660_ip[1]%100)/10) + 0x30;
			Objective_ip_2[2] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[3] = '\0';
		}
		else if((NL660_ip[1] < 100) && (NL660_ip[1] >= 10))
		{
			Objective_ip_2[0] = (NL660_ip[1]/10) + 0x30;
			Objective_ip_2[1] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[2] = '\0';		
		}
		else
		{
			Objective_ip_2[0] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[1] = '\0';		
		}
		//3
	
	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[2] >= 100)
		{
			Objective_ip_3[0] = (NL660_ip[2]/100) + 0x30;
			Objective_ip_3[1] = ((NL660_ip[2]%100)/10) + 0x30;
			Objective_ip_3[2] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[3] = '\0';
		}
		else if((NL660_ip[2] < 100) && (NL660_ip[2] >= 10))
		{
			Objective_ip_3[0] = (NL660_ip[2]/10) + 0x30;
			Objective_ip_3[1] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[2] = '\0';		
		}
		else
		{
			Objective_ip_3[0] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[1] = '\0';		
		}
		//4

	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[3] >= 100)
		{
			Objective_ip_4[0] = (NL660_ip[3]/100) + 0x30;
			Objective_ip_4[1] = ((NL660_ip[3]%100)/10) + 0x30;
			Objective_ip_4[2] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[3] = '\0';
		}
		else if((NL660_ip[3] < 100) && (NL660_ip[3] >= 10))
		{
			Objective_ip_4[0] = (NL660_ip[3]/10) + 0x30;
			Objective_ip_4[1] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[2] = '\0';		
		}
		else
		{
			Objective_ip_4[0] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[1] = '\0';		
		}
			
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_1);
//		USART2_SendByte('"');
//		
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_2);
//		USART2_SendByte('"');
//			
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_3);
//		USART2_SendByte('"');
//		
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_4);
//		USART2_SendByte('"');
     k=1;
		return k;
		
		}
		
		else {
		USART2_DMASS("AT+QIACT=1\r\n","OK",1000,200);
		USART2_DMASS("AT+QIACT?\r\n","OK",1000,500);
		if(strstr(U2BUF,"+QIACT: 1,1,1")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
			
			bc = strstr(U2BUF,"+QIACT: 1,1,1,");
			//ÌáÆðIP
			bc=bc+14; 
     // USART2_printf( USART2, "\r\n is %s \r\n",bc);
			i=0;
			for(j=0;j<4;j++)
			{ 
				bc += (i+1); 
				i=0;					
				do{
					i++;
				}while(bc[i]!='.' && bc[i]!='\0' && bc[i] != 0x22 && i<3);
				
				switch (i){
					case 3:
							NL660_ip[j] = ASCII_u16(bc[0]) * 100 + ASCII_u16(bc[1]) *10 + ASCII_u16(bc[2]);
						break;					
					case 2:		
							NL660_ip[j] = ASCII_u16(bc[0]) *10 + ASCII_u16(bc[1]);
						break;
					case 1:		
							NL660_ip[j] = ASCII_u16(bc[0]);
						break;
					default:
						break;
				}	
				
				
			}
			
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[0]);
//			USART2_printf( USART2, "\r\n  is %d \r\n",NL660_ip[1]);
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[2]);
//			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[3]);
			
		if(NL660_ip[0] >= 100)
		{
			Objective_ip_1[0] = (NL660_ip[0]/100) + 0x30;
			Objective_ip_1[1] = ((NL660_ip[0]%100)/10) + 0x30;
			Objective_ip_1[2] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[3] = '\0';
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=Objective_ip_1[2];
//			ip_writ_add[3]=0x2e;
		}
		else if((NL660_ip[0]< 100) && (NL660_ip[0] >= 10))
		{
			Objective_ip_1[0] = (NL660_ip[0]/10) + 0x30;
			Objective_ip_1[1] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[2] = '\0';	
			
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=0x2e;
			
		}
		else
		{
			Objective_ip_1[0] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[1] = '\0';		
		}

		if(NL660_ip[1] >= 100)
		{
			Objective_ip_2[0] = (NL660_ip[1]/100) + 0x30;
			Objective_ip_2[1] = ((NL660_ip[1]%100)/10) + 0x30;
			Objective_ip_2[2] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[3] = '\0';
		}
		else if((NL660_ip[1] < 100) && (NL660_ip[1] >= 10))
		{
			Objective_ip_2[0] = (NL660_ip[1]/10) + 0x30;
			Objective_ip_2[1] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[2] = '\0';		
		}
		else
		{
			Objective_ip_2[0] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[1] = '\0';		
		}
		//3
	
	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[2] >= 100)
		{
			Objective_ip_3[0] = (NL660_ip[2]/100) + 0x30;
			Objective_ip_3[1] = ((NL660_ip[2]%100)/10) + 0x30;
			Objective_ip_3[2] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[3] = '\0';
		}
		else if((NL660_ip[2] < 100) && (NL660_ip[2] >= 10))
		{
			Objective_ip_3[0] = (NL660_ip[2]/10) + 0x30;
			Objective_ip_3[1] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[2] = '\0';		
		}
		else
		{
			Objective_ip_3[0] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[1] = '\0';		
		}
		//4

	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[3] >= 100)
		{
			Objective_ip_4[0] = (NL660_ip[3]/100) + 0x30;
			Objective_ip_4[1] = ((NL660_ip[3]%100)/10) + 0x30;
			Objective_ip_4[2] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[3] = '\0';
		}
		else if((NL660_ip[3] < 100) && (NL660_ip[3] >= 10))
		{
			Objective_ip_4[0] = (NL660_ip[3]/10) + 0x30;
			Objective_ip_4[1] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[2] = '\0';		
		}
		else
		{
			Objective_ip_4[0] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[1] = '\0';		
		}
	  k=1;
		return k; //µÃµ½IPµÄµØÖ· 

  } else 
		{
			k=0;
			return k; //Ã»ÓÐµÃµ½IPµØÖ·
			
		}
		
}
		
  }
			
	//µÃµ½IPµÄµÄº¯Êý
//·µ»ØÖµ£¬1£ºµÃµ½ÁËIPµØÖ·
 //       0£ºÃ»ÓÐµÃµ½IP
//Ö»×ö²éÑ¯²Ù×÷£¬²»È¥¼¤»î
int get_ip_not()
{
	int i,j,k;
	unsigned char *bc;
	
	  USART2_DMASS("AT+QIACT?\r\n","OK",1000,500);
		if(strstr(U2BUF,"+QIACT: 1,1,1")!=NULL)//ÅÐ¶Ï»Ø¸´Êý¾Ý
		{
		
			bc = strstr(U2BUF,"+QIACT: 1,1,1,");
			//ÌáÆðIP
			bc=bc+14; 
      USART2_printf( USART2, "\r\n iiiiiiiiiiiiiiiiiiiiiiiiis %s \r\n",bc);
			i=0;
			for(j=0;j<4;j++)
			{ 
				bc += (i+1); 
				i=0;					
				do{
					i++;
				}while(bc[i]!='.' && bc[i]!='\0' && bc[i] != 0x22 && i<3);
				
				switch (i){
					case 3:
							NL660_ip[j] = ASCII_u16(bc[0]) * 100 + ASCII_u16(bc[1]) *10 + ASCII_u16(bc[2]);
						break;					
					case 2:		
							NL660_ip[j] = ASCII_u16(bc[0]) *10 + ASCII_u16(bc[1]);
						break;
					case 1:		
							NL660_ip[j] = ASCII_u16(bc[0]);
						break;
					default:
						break;
				}	
				
				
			}
			
			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[0]);
			USART2_printf( USART2, "\r\n  is %d \r\n",NL660_ip[1]);
			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[2]);
			USART2_printf( USART2, "\r\n is %d \r\n",NL660_ip[3]);
			
		if(NL660_ip[0] >= 100)
		{
			Objective_ip_1[0] = (NL660_ip[0]/100) + 0x30;
			Objective_ip_1[1] = ((NL660_ip[0]%100)/10) + 0x30;
			Objective_ip_1[2] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[3] = '\0';
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=Objective_ip_1[2];
//			ip_writ_add[3]=0x2e;
		}
		else if((NL660_ip[0]< 100) && (NL660_ip[0] >= 10))
		{
			Objective_ip_1[0] = (NL660_ip[0]/10) + 0x30;
			Objective_ip_1[1] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[2] = '\0';	
			
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=0x2e;
			
		}
		else
		{
			Objective_ip_1[0] = (NL660_ip[0]%10) + 0x30;
			Objective_ip_1[1] = '\0';		
		}

		if(NL660_ip[1] >= 100)
		{
			Objective_ip_2[0] = (NL660_ip[1]/100) + 0x30;
			Objective_ip_2[1] = ((NL660_ip[1]%100)/10) + 0x30;
			Objective_ip_2[2] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[3] = '\0';
		}
		else if((NL660_ip[1] < 100) && (NL660_ip[1] >= 10))
		{
			Objective_ip_2[0] = (NL660_ip[1]/10) + 0x30;
			Objective_ip_2[1] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[2] = '\0';		
		}
		else
		{
			Objective_ip_2[0] = (NL660_ip[1]%10) + 0x30;
			Objective_ip_2[1] = '\0';		
		}
		//3
	
	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[2] >= 100)
		{
			Objective_ip_3[0] = (NL660_ip[2]/100) + 0x30;
			Objective_ip_3[1] = ((NL660_ip[2]%100)/10) + 0x30;
			Objective_ip_3[2] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[3] = '\0';
		}
		else if((NL660_ip[2] < 100) && (NL660_ip[2] >= 10))
		{
			Objective_ip_3[0] = (NL660_ip[2]/10) + 0x30;
			Objective_ip_3[1] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[2] = '\0';		
		}
		else
		{
			Objective_ip_3[0] = (NL660_ip[2]%10) + 0x30;
			Objective_ip_3[1] = '\0';		
		}
		//4

	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip[3] >= 100)
		{
			Objective_ip_4[0] = (NL660_ip[3]/100) + 0x30;
			Objective_ip_4[1] = ((NL660_ip[3]%100)/10) + 0x30;
			Objective_ip_4[2] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[3] = '\0';
		}
		else if((NL660_ip[3] < 100) && (NL660_ip[3] >= 10))
		{
			Objective_ip_4[0] = (NL660_ip[3]/10) + 0x30;
			Objective_ip_4[1] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[2] = '\0';		
		}
		else
		{
			Objective_ip_4[0] = (NL660_ip[3]%10) + 0x30;
			Objective_ip_4[1] = '\0';		
		}
			
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_1);
//		USART2_SendByte('"');
//		
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_2);
//		USART2_SendByte('"');
//			
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_3);
//		USART2_SendByte('"');
//		
//		USART2_SendByte('"');
//		USART2_DMAS(Objective_ip_4);
//		USART2_SendByte('"');
     k=1;
		return k;
		
		}
		


}
			
	
	
	
uint16_t get_B_ip(u8 NL660_ip_0,u8 NL660_ip_1,u8 NL660_ip_2,u8 NL660_ip_3,u8 NL660_ip_4,u8 NL660_ip_5)
{
	  uint16_t k=0;
	
		if(NL660_ip_0 >= 100)
		{
			Objective_ip_B_1[0] = (NL660_ip_0/100) + 0x30;
			Objective_ip_B_1[1] = ((NL660_ip_0%100)/10) + 0x30;
			Objective_ip_B_1[2] = (NL660_ip_0%10) + 0x30;
			Objective_ip_B_1[3] = '\0';
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=Objective_ip_1[2];
//			ip_writ_add[3]=0x2e;
		}
		else if((NL660_ip_0< 100) && (NL660_ip_0 >= 10))
		{
			Objective_ip_B_1[0] = (NL660_ip_0/10) + 0x30;
			Objective_ip_B_1[1] = (NL660_ip_0%10) + 0x30;
			Objective_ip_B_1[2] = '\0';	
			
//			ip_writ_add[0]=Objective_ip_1[0];
//			ip_writ_add[1]=Objective_ip_1[1];
//			ip_writ_add[2]=0x2e;
			
		}
		else
		{
			Objective_ip_B_1[0] = (NL660_ip_0%10) + 0x30;
			Objective_ip_B_1[1] = '\0';		
		}

		if(NL660_ip_1 >= 100)
		{
			Objective_ip_B_2[0] = (NL660_ip_1/100) + 0x30;
			Objective_ip_B_2[1] = ((NL660_ip_1%100)/10) + 0x30;
			Objective_ip_B_2[2] = (NL660_ip_1%10) + 0x30;
			Objective_ip_B_2[3] = '\0';
		}
		else if((NL660_ip_1 < 100) && (NL660_ip_1 >= 10))
		{
			Objective_ip_B_2[0] = (NL660_ip_1/10) + 0x30;
			Objective_ip_B_2[1] = (NL660_ip_1%10) + 0x30;
			Objective_ip_B_2[2] = '\0';		
		}
		else
		{
			Objective_ip_B_2[0] = (NL660_ip_1%10) + 0x30;
			Objective_ip_B_2[1] = '\0';		
		}
		//3
	
	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip_2 >= 100)
		{
			Objective_ip_B_3[0] = (NL660_ip_2/100) + 0x30;
			Objective_ip_B_3[1] = ((NL660_ip_2%100)/10) + 0x30;
			Objective_ip_B_3[2] = (NL660_ip_2%10) + 0x30;
			Objective_ip_B_3[3] = '\0';
		}
		else if((NL660_ip_2 < 100) && (NL660_ip_2 >= 10))
		{
			Objective_ip_B_3[0] = (NL660_ip_2/10) + 0x30;
			Objective_ip_B_3[1] = (NL660_ip_2%10) + 0x30;
			Objective_ip_B_3[2] = '\0';		
		}
		else
		{
			Objective_ip_B_3[0] = (NL660_ip_2%10) + 0x30;
			Objective_ip_B_3[1] = '\0';		
		}
		//4

	//	UART4_SendByte(Outgoing_Data[i]);
		
		if(NL660_ip_3 >= 100)
		{
			Objective_ip_B_4[0] = (NL660_ip_3/100) + 0x30;
			Objective_ip_B_4[1] = ((NL660_ip_3%100)/10) + 0x30;
			Objective_ip_B_4[2] = (NL660_ip_3%10) + 0x30;
			Objective_ip_B_4[3] = '\0';
		}
		else if((NL660_ip_3 < 100) && (NL660_ip_3 >= 10))
		{
			Objective_ip_B_4[0] = (NL660_ip_3/10) + 0x30;
			Objective_ip_B_4[1] = (NL660_ip_3%10) + 0x30;
			Objective_ip_B_4[2] = '\0';		
		}
		else
		{
			Objective_ip_B_4[0] = (NL660_ip_3%10) + 0x30;
			Objective_ip_B_4[1] = '\0';		
		}
	
	  k=(NL660_ip_4<<8)+NL660_ip_5;
	
		return k;
	
}
	

	
	

