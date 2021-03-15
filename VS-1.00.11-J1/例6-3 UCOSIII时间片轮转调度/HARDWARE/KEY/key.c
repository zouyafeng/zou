#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "24cxx.h"	
#include "w25qxx.h"	

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
		
extern u8 set_ch1_bo;  //开机默认是连续波输出
extern u8 set_ch2_bo;  //开机默认是连续波输出
extern u8 set_ch3_bo;  //开机默认是连续波输出
extern u8 set_ch4_bo;  //开机默认是连续波输出
extern u8 set_ch5_bo;  //开机默认是连续波输出
extern u8 set_ch6_bo;  //开机默认是连续波输出

extern u16 ch1_unm,ch2_unm,ch3_unm,ch4_unm,ch5_unm,ch6_unm;    //开机默认周期10ms 
extern	u8 tan_xue_boo; //探穴检测到定义函数 当检测到穴位时为2，检测穴位相应完为0，应为是震动电路产生的穴位信号，所以每次相应完都置为0 .					
u8 tong_start=0;	  //读取那一个通道电脉冲旋钮开启状态 0是打开，1是关闭  5位是1通道  4位是2通道 3位是3通道  2是4通道   1位是5通道   0位是6通道  					
extern u16 elect_time; //定时更新屏幕上电脉冲显示图标状态						
						
u8 set_tong=1; //当前在设置那一个通道，初始化变量是1通道。
extern u8 wendu[9] ;

u8 key_umb=0;

u8 key_staty=4;




u8 time_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x02,0x00,0x00};  //发送1通道时间的变量数组  定时 
u8 time_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x04,0x00,0x00};  //发送2通道时间的变量数组
u8 time_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x06,0x00,0x00};  //发送3通道时间的变量数组
u8 time_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x08,0x00,0x00};  //发送4通道时间的变量数组
u8 time_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x10,0x00,0x00};  //发送5通道时间的变量数组
u8 time_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x12,0x00,0x00};  //发送6通道时间的变量数组


u8 zhou_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x14,0x00,0x00};  //发送1通道时间的变量数组  周期
u8 zhou_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x16,0x00,0x00};  //发送2通道时间的变量数组
u8 zhou_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x18,0x00,0x00};  //发送3通道时间的变量数组
u8 zhou_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x20,0x00,0x00};  //发送4通道时间的变量数组
u8 zhou_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x22,0x00,0x00};  //发送5通道时间的变量数组
u8 zhou_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x24,0x00,0x00};  //发送6通道时间的变量数组



u8 boxin_1[9]={0xA5,0x5A,0x05,0x82,0x01,0x01,0x00,0x00};  //发送1通道时间的变量数组   波形
u8 boxin_2[9]={0xA5,0x5A,0x05,0x82,0x01,0x02,0x00,0x00};  //发送2通道时间的变量数组
u8 boxin_3[9]={0xA5,0x5A,0x05,0x82,0x01,0x03,0x00,0x00};  //发送3通道时间的变量数组
u8 boxin_4[9]={0xA5,0x5A,0x05,0x82,0x01,0x04,0x00,0x00};  //发送4通道时间的变量数组
u8 boxin_5[9]={0xA5,0x5A,0x05,0x82,0x01,0x05,0x00,0x00};  //发送5通道时间的变量数组
u8 boxin_6[9]={0xA5,0x5A,0x05,0x82,0x01,0x06,0x00,0x00};  //发送6通道时间的变量数组


u8 kuang_1[9]={0xA5,0x5A,0x05,0x82,0x00,0x25,0x00,0x02};  //发送1通道时间的变量数组  方框选择框
u8 kuang_2[9]={0xA5,0x5A,0x05,0x82,0x00,0x26,0x00,0x02};  //发送2通道时间的变量数组
u8 kuang_3[9]={0xA5,0x5A,0x05,0x82,0x00,0x27,0x00,0x02};  //发送3通道时间的变量数组
u8 kuang_4[9]={0xA5,0x5A,0x05,0x82,0x00,0x28,0x00,0x02};  //发送4通道时间的变量数组
u8 kuang_5[9]={0xA5,0x5A,0x05,0x82,0x00,0x29,0x00,0x02};  //发送5通道时间的变量数组
u8 kuang_6[9]={0xA5,0x5A,0x05,0x82,0x00,0x30,0x00,0x02};  //发送6通道时间的变量数组


u8 HOT_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x40,0x00,0x00};  //加热指示灯显示灯显示变量
u8 ELCK_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x35,0x00,0x00};  //电脉冲输出指示灯显示变量

u8 XUE_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x44,0x00,0x00};  //探穴指示灯显示变量

u8 JS_LED[9]={0xA5,0x5A,0x05,0x82,0x00,0x50,0x00,0x00};  //短路开路检测关闭



u8 ELCK_UF[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


extern  u8 xian_buf[6];

bool  H1_BOOl=0,H2_BOOl=0,H3_BOOl=0,H4_BOOl=0,H5_BOOl=0,H6_BOOl=0;
u8 H1_um=0,H2_um=0,H3_um=0,H4_um=0,H5_um=0,H6_um=0;
u8 ELCK1_um=0,ELCK2_um=0,ELCK3_um=0,ELCK4_um=0,ELCK5_um=0,ELCK6_um=0;

u8 HOT_unm=0,ELCK_unm=0,ELCK_unm_X,ELCK_unm_U1,ELCK_unm_U2,ELCK_unm_U3,ELCK_unm_U4;  //定义加热，电脉冲 变量计算结果存储地址；

u8 ELCK_unm_i=0;

extern u8 system_start_EL;  //开路短路停止输出电脉冲标识

bool tang_bool=0;           //探穴标志

bool clos_bis=TRUE;   //添加长按探穴按键关闭闭锁报警功能  

extern u8 clos_bis_unm;//添加长按探穴按键关闭闭锁报警功能  

extern u8  ch1_chexe,ch2_chexe,ch3_chexe,ch4_chexe,ch5_chexe,ch6_chexe;


u8 key_up=1;   //按键按松开标志 ,控制键盘扫描函数是否支持连续按下
u8 key_up_1=1; //按键按松开标志 ,控制键盘扫描函数是否支持连续按下
u8 key_up_2=1; //按键按松开标志 ,控制键盘扫描函数是否支持连续按下

u8 key_ele_unm=0; //电脉冲数据暂存数组位置

u8 zhi_unm_date=0;





//波形信息储存结构体
//set_time;			设置工作治疗时间  
//zhou_time;    设置输出波形周期	
//bo_xin;       设置波形输出类型  1->连续  2->轻捶  3->疏密  4->按摩E1 5->按摩E2 6->按摩E3 
//shen_time;    治疗剩余时间（暂时未启用）
struct _zhi_liao 
{										    
	u8 set_time;			//设置工作治疗时间  
	u8 zhou_time;			//设置输出波形周期	
	u8 bo_xin;				//设置波形输出类型  1->连续  2->轻捶  3->疏密  4->按摩E1 5->按摩E2 6->按摩E3 
  u8 shen_time;     //治疗剩余时间（暂时未启用）

}; 	 

//通道1波形信息储存结构体
struct _zhi_liao tongdao_1={60,2,1,60};
//通道2波形信息储存结构体
struct _zhi_liao tongdao_2={60,2,1,60};
//通道3波形信息储存结构体
struct _zhi_liao tongdao_3={60,2,1,60};
//通道4波形信息储存结构体
struct _zhi_liao tongdao_4={60,2,1,60};
//通道5波形信息储存结构体
struct _zhi_liao tongdao_5={60,2,1,60};
//通道6波形信息储存结构体
struct _zhi_liao tongdao_6={60,2,1,60};


//电脉冲输出治疗时间定义
u8 ch1_start=2,ch2_start=2,ch3_start=2,ch4_start=2,ch5_start=2,ch6_start=2;




//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTE时钟
  //键盘扫描行码初始化配置
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_6;    //加热按键的行码输出公共端 第一版本是PE0 后改称PE0 ; 脉冲电针输出公共端 第一版本是PC13 后改称PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;          //GPIO_Mode_IPU; //设置成上拉输入 后改为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //初始化	
  GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_6);
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //设置键盘公共端输出位 第一版本是PD9 第2版本改成 PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成上拉输入  后改称推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	//第一版本列码连接GPIO配置 KY01-PC8   KYO2-PC9  KYO3-PA8   KY04-PC0   KK05-PC11   KYO6-PC12
	//第二版本列码连接GPIO配置 KY01-PD5   KYO2-PD4  KYO3-PD3   KY04-PD2   KK05-PC9   KYO6-PC8
	
	//初始化 WK_UP-->GPIOA.8	  下拉输入  推挽输出初始化配置为0    KOY3.
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;			//设置成上拉输入
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.0
 // GPIO_ResetBits(GPIOA,GPIO_Pin_8);


	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9;			// 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.0
  //GPIO_ResetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8);
	
 // GPIO_SetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8);
 // delay_ms(100);

//  HOT_IO_IN();
//////  HOT_IO_OUT();
//// // HOT_DQ_OUT=0;

//   KOY6_IO_IN();
//		//KOY6_IO_OUT()
//	//	KOY6_DQ_OUT=1;
////		
//KOY5_IO_IN();
//KOY4_IO_IN();
//KOY2_IO_IN();
//KOY1_IO_IN();
//KOY3_IO_IN();




}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(void)
{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
//	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
// 	return 0;// 无按键按下
//	
//	
//  int cord_h,cord_l;//行列值
	int cord_buf=0,cord_buf_L=0,key=0xffff;
	
	//把列码置为0，读取行码的值。
	
	
	
	key_start(1);          //先读出行值，行值为高位，输入参数1为读行值，6列低位全为零；；；。 
 
	cord_buf=key_red_H(); 
	
	
 if(cord_buf!=0x01C0)
	{  			
	
		printf("读到行码3：%X\r\n",cord_buf);
		
	  key_umb=key_staty;		//更新隔断扫描时间	
		
			//printf("扫描到键盘吗\r\n");
		 //delay_ms(50);        //祛除抖动操作
	 
	  cord_buf=key_red_H();  //再次读取一下行值。
	 if(cord_buf!=0x01C0)
	  { 
			
     
			
		//	KEY_Init();
			
		//读取列码的键盘码值   
		key_start(2);
		//KEY_Init();
			
		// delay_ms(50);
			
		cord_buf_L=	key_red();	//取得列码，里面按下后列码位为低 其他位为高。没有按下码值为0X3F对应 11 1111  	
		printf("读到列码：%X",cord_buf_L);	
				
		//if(cord_buf_L==0x1F) printf("扫描到键盘吗0XDF\r\n");
			
		cord_buf=cord_buf+cord_buf_L;	
		 switch(cord_buf)
		{	
			
      case 0xDF:      //行码0xC0  列码为 0X1F	 相加为 case 值  
               key=1; //加热 6 通道键盘码
			printf("扫描到键盘吗0XDF\r\n");
			
			return key;
			
       break;
			
			case 0xEF:   //行码0xC0 列码 0x2F   相加为 case 值 
					 key=2;  //加热5 通道键盘码 
			return key;
			
	//		printf("扫描到键盘吗0XFF\r\n");
					 break;

			case 0xF7:  //行码0XC0 列码 0x37    相加为 case 值 
					 key=3;  //加热4 通道键盘码
			return key;
			
   //   printf("扫描到键盘吗0XF7\r\n");
			
					 break;

			case 0xFB:  //行码0xC0 列码 0x3B	   相加为 case 值 
					 key=4; //加热3 通道键盘吗
			
					return key;
			
		//	printf("扫描到键盘吗0XFB\r\n");
					 break;

		  case 0xFD:  //行码0xC0 列码 0x3D	   相加为 case 值 
               key=5; //加热2 通道键盘吗
			
					return key;
			
		//	printf("扫描到键盘吗0XFD\r\n");
               break;

		  case 0xFE:  //行码0xC0 列码 0x3E	  相加为 case 值 
				
					printf("扫描到键盘吗0XFE\r\n");
			
          key=6; //加热1 通道键盘吗               
              
					return key;
    	break;
			
			
		////////////////////电脉冲输出开关按键键盘扫描码////////////////////////

//		  case 0x15F:       //行码0x140  列码为 0X1F	   相加为 case 值 
//               key=7;
//			             //电脉冲 6 通道键盘码
//			printf("扫描到键盘吗0X15F\r\n");
//               break;			


//          case 0x16F:   //行码0x140 列码 0x2F 	   相加为 case 值 
//               key=8;   //电脉冲 5 通道键盘码 
//			   //led=0; 
//               break;

//          case 0x177:  //行码0x140 列码 0x37  	   相加为 case 值 
//               key=9;  //加热 4 通道键盘码

//               break;

//          case 0x17B:  //行码0x140 列码 0x3B		   相加为 case 值 
//               key=10; //加热 3 通道键盘吗
//               break;
// 	
//		  case 0x17D:  //行码0x140 列码 0x3D		   相加为 case 值 
//               key=11; //加热 2 通道键盘吗
//               break;

//		  case 0x17E:  //行码0x140 列码 0x3E		   相加为 case 值 
//               key=12; //加热 1 通道键盘吗               
               
//		       break;
			
			
			
	 ////////////////////电脉冲输出开关按键键盘扫描码////////////////////////

		      case 0x19F :      //行码0x180  列码为 0X1F	   相加为 case 值 
              key=13;
			         // key=14;             //探穴W 通道键盘码	   KYO 6
			         printf("扫描到键盘吗 探穴 0X19F\r\n");
			    
					return key;
					
               break;		  	

          case 0x1BB :   //行码0x180 列码 0x2F 	    相加为 case 值 
              key=14;   // 通道D 键盘码 		KYO 5
					
					    // key=13;
			         printf("扫描到键盘吗 通道D 0X1BB\r\n");
					return key;
					
               break;

          case 0x1AF  :  //行码0x180 列码 0x37  	    相加为 case 值 
              // key=15;  // 波形 B  键盘码	   KYO 4
					     key=16;
               printf("扫描到键盘吗 波形 0X1AF\r\n");
					return key;
               break;

          case 0x1BD :  //行码0x180 列码 0x3B	    相加为 case 值 
             //  key=16;   //周期设置 下 减 键盘码		KYO 3
					
					     key=15;
					     printf("扫描到键盘吗 周期 下 0X1BD\r\n");
					return key;
					
               break;
 	
		      case 0x1BE:  //行码0x180 列码 0x3D		相加为 case 值 
						
              // key=17; //周期设置 上 加 键盘吗		KYO 2
					      
					    key=18;
					return key;
					
			         printf("扫描到键盘吗 周期上  0X1B7\r\n");
			
               break;

		      case 0x1B7 :  //行码0x180 列码 0x3E	    相加为 case 值 
              // key=18;  //时间 键盘吗      KYO 1   
			
					     key=17;
					
			         printf("扫描到键盘吗  时间  0X1BE\r\n");
			   return key;
               break;
		
			
			
			
			
		}	
			
		}
		
	}
	
  return 0;// 无按键按下
	
}




u8 KEY_Scan_dian(void)
{

u8 cord_buf=0,y=0,i=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;	
	
	//SELET_IO_OUT();
	//SELET_DQ_OUT=0;	 //将电脉冲行公共端置低，读列码。
	
  HOT_IO_OUT();
	HOT_DQ_OUT=1;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=0;	 //屏蔽电脉冲输入端子，将输出置高，忽略电脉冲行码。
		
	SET_IO_OUT();
	SET_DQ_OUT=1;
	
	
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	delay_ms(5);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
	
	
	
 k6=KOY6_DQ_IN;
 k5=KOY5_DQ_IN;
 k4=KOY4_DQ_IN;
 k3=KOY3_DQ_IN;
 k2=KOY2_DQ_IN;
 k1=KOY1_DQ_IN;
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	
	tong_start=ELCK_unm; //将通道电脉冲变量信息传递出去  
	
	
	
	///////////////////连续对比几次数据都相同则更像发送数列数组内容，不相同则删除/避免吴发送出现误判的情况//////////////////////
	 ELCK_UF[ELCK_unm_i]=ELCK_unm;
	 ELCK_unm_i++;
	 
	 if(ELCK_unm_i>=2){
		 ELCK_unm_i=0;
		 if((ELCK_UF[0]==ELCK_unm)&&(ELCK_UF[1]==ELCK_unm))
		 {
			 ELCK_LED[7]=ELCK_unm;
			 
			 
			if( ELCK_LED[7]==0x3f)
			 {
				system_start_EL=1; 
				 
			 }
			 
			 
		 }
		 
		
		 
		 
		 
		 
		 fa_set_elect();
		 
		 
		 
		 
	 }
	 
	 
	
	
	
	
//	//当扫描到有电脉冲选项变换的时候才发送一次更新屏幕信息指令，减少不必要的发送操作，提示CPU执行效率 
//	if(ELCK_unm_X!=ELCK_unm){
//    ELCK_unm_X=ELCK_unm;
//		
//		ELCK_LED[7]=ELCK_unm;
//		
//   	for(j=0;j<5;j++){
//			 for(i=0;i<8;i++)
//					{
//						
//						USART_SendData(USART2,ELCK_LED[i]);
//						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//						
//					}
//					} 
		
		//printf("脉冲发送控制码 ：%d \r\n",ELCK_unm );			
					
		
	
	
////在机器电脉冲输出工作时，当治疗时间到后，重新设置一下治疗时间
//	
//	if(k6) feng_6=1; else feng_6=0;
//	if(k5) feng_5=1; else feng_5=0;
//	if(k4) feng_4=1; else feng_4=0;
//	if(k3) feng_3=1; else feng_3=0;
//	if(k2) feng_2=1; else feng_2=0;
//	if(k1) feng_1=1; else feng_1=0;
	
	if(tong_start!=0x3f){
		
		return 1;
	}else
 {
		return 0;
		
	}
	
	
	
	
}


//开机发生电脉冲信号，解决开机后检测到旋钮没归零，蜂鸣器叫，但是屏幕电脉冲图标未亮，用户不知道那一路没有点亮。



u8 Start_KEY_Scan_dian(void)
{

u8 cord_buf=0,y=0,i=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;	
	
	//SELET_IO_OUT();
	//SELET_DQ_OUT=0;	 //将电脉冲行公共端置低，读列码。
	
  HOT_IO_OUT();
	HOT_DQ_OUT=1;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=0;	 //屏蔽电脉冲输入端子，将输出置高，忽略电脉冲行码。
		
	SET_IO_OUT();
	SET_DQ_OUT=1;
	
	
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	delay_ms(100);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
	
	
	
 k6=KOY6_DQ_IN;
 k5=KOY5_DQ_IN;
 k4=KOY4_DQ_IN;
 k3=KOY3_DQ_IN;
 k2=KOY2_DQ_IN;
 k1=KOY1_DQ_IN;
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	
	tong_start=ELCK_unm; //将通道电脉冲变量信息传递出去  
	
	//当扫描到有电脉冲选项变换的时候才发送一次更新屏幕信息指令，减少不必要的发送操作，提示CPU执行效率 
	if(ELCK_unm_X!=ELCK_unm){
    ELCK_unm_X=ELCK_unm;
		
		ELCK_LED[7]=ELCK_unm;
		
   	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,ELCK_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
		
		//printf("脉冲发送控制码 ：%d \r\n",ELCK_unm );			
					
	}		
	
	
//在机器电脉冲输出工作时，当治疗时间到后，重新设置一下治疗时间
	

	
	if(tong_start!=0x3f){
		
		return 1;
	}else
 {
		return 0;
		
	}
	
	delay_ms(500);
	
	
}






//将电脉冲开机状态信息发送到显示屏上
void fa_set_elect(void)
	{
	u8 i,j,k;
	
	  	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,ELCK_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	
	
}

 ///////////////取一次键盘按下来的码值,低8位的数据 没按下时返回的码是 0X3F  ////////////////////////////
 int key_red(void)
 {
  int cord=0;
  char cord_buf=0,y=0,j=0,k=0,k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	 
   y=0;
   k=0;
   k6=KOY6_DQ_IN;
   k5=KOY5_DQ_IN;
   k4=KOY4_DQ_IN;
   k3=KOY3_DQ_IN;
   k2=KOY2_DQ_IN;
   k1=KOY1_DQ_IN;
	 
   //k1=1;
	 
	 
   k6=(k6<<5);
   k5=(k5<<4);
   k4=(k4<<3);
   k3=(k3<<2);
   k2=(k2<<1);
  
   y=(k6+k5+k4+k3+k2+k1);
  // if(y==0x3f) led=0;

   cord=y;
   //if(cord==0x1C0){led=0;}


   return cord;

 }

//得到高位的行码。
  int key_red_H(void)
{

   int cord=0;
   u8 cord_buf=0,y=0,j=0,k=0;
   
   y=HOT_DQ_IN;  //加热行码     温针码0X
   y=(y<<2);    
   //j=SELET_DQ_IN; //电脉冲行码
	 j=1;
   j=(j<<1);
   k=SET_DQ_IN;   //设置按键行码
   y=(y+j+k);
   cord=y;
   cord=(cord<<6); 	 
   

   return cord;
	
}



u8 key_start(u8 i)
{
	//扫描行码，行码设置为输入模式。列码为推挽输出0；
	
	
	if(i==1){
		
	SET_IO_OUT();
	SET_DQ_OUT=1;	
		
	HOT_IO_OUT();
	HOT_DQ_OUT=1;
		
	delay_ms(20);	
		
	HOT_IO_IN();
	//SELET_IO_IN(); 	
	SET_IO_IN();	
		
		
		
		
	KOY6_IO_OUT();	
	KOY6_DQ_OUT=0;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=0;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=0;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=0;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=0;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=0;

	}
	
	if(i==2){
		
	//	printf("扫描到键GAOGAOGAO0XDF\r\n");
		
		
	HOT_IO_OUT();
	HOT_DQ_OUT=0;	
		
	SELET_IO_OUT();
	SELET_DQ_OUT=1;	 //屏蔽电脉冲输入端子，将输出置高，忽略电脉冲行码。
	//SELET_DQ_OUT=0;

		
	SET_IO_OUT();
	SET_DQ_OUT=0;
	
		
		

	KOY6_IO_OUT();	
	KOY6_DQ_OUT=1;
		
  KOY5_IO_OUT();	
	KOY5_DQ_OUT=1;
		
  KOY4_IO_OUT();	
	KOY4_DQ_OUT=1;
		
	KOY3_IO_OUT();	
	KOY3_DQ_OUT=1;

  KOY2_IO_OUT();	
	KOY2_DQ_OUT=1;
		
	KOY1_IO_OUT();	
	KOY1_DQ_OUT=1;
	
	//delay_ms(20);
	
	
	KOY6_IO_IN(); 	
	KOY5_IO_IN(); 	
	KOY4_IO_IN(); 	
	KOY3_IO_IN(); 
  KOY2_IO_IN(); 
  KOY1_IO_IN(); 
	
		
	}
	
	
	
	
	
}




int key_set(void)
{
	
	  u8 i,j,task1_num=0;
	  u8 key=0; 
	


	
	// key=KEY_Scan();
		
//		if(key){
//		
//   printf("读到的键盘码值 ：%d",key );

//	}
	
	key=get_set_num();
	
// printf("读到的键盘码值 ：%d \r\n",key );
	
//	clos_bis  关闭闭锁检测按钮
	
		//保存通道1至通道6的治疗信息数据
	if(key==20){
	
	clos_bis=!clos_bis;  //对输出短路开路检测控制变量进行设置
		
   TIM_Cmd(TIM3, DISABLE);  //关闭定时器三中断 
	 delay_ms(200);
		
		if(clos_bis){
			
			JS_LED[7]=0x01;
			
			clos_bis_unm=1;
		}else {
			
			JS_LED[7]=0x00;
			
			clos_bis_unm=0;
			
			/////////中断控制变量6路///////////////
			ch1_chexe=0; 
			ch2_chexe=0;
			ch3_chexe=0;
			ch4_chexe=0;
			ch5_chexe=0;
			ch6_chexe=0;
			
		}
		
		// printf("闭锁控制码值 ：%d,%d, \r\n",clos_bis,clos_bis_unm);
		
				for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,JS_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
		TIM_Cmd(TIM3, ENABLE);  //代开定时器三中断
			
					
					
	}

	//保存通道1至通道6的治疗信息数据
	if(key==19){
	
		zhi_unm_date=6; //将蜂鸣器提示音开启。
		save_ch1_6_24c02(); //将现在的通道1至通道6的信息保存到AT24C02芯片中 。               
		
		
	}

	//探穴操作
	if(key==13){
		elect_time=7;
		
		
		tang_bool=~tang_bool;
	
		
		if(tang_bool){
			
			XUE_LED[7]=0x01;
			
		}else {
			
			XUE_LED[7]=0x00;
			
		}
		
		for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,XUE_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
		
		key_umb=key_staty;		//更新隔断扫描时间		
		
	}
	
	
	//通道
	if(key==14){
		elect_time=7;	
			set_tong++;
			if(set_tong>=7)
			set_tong = 1;
			
			wendu[7]=set_tong;
			
		//	kuang_1[7]
			
		//	USART_SendData(USART2,0xA5);
		//	USART_SendData(USART2,0xA5);
			
				for(j=0;j<5;j++){    //发生设置的通道值的变量说明现在设置的是那一个通道 就死7寸屏左下角的1——2——3——4——5——6 
				 for(i=0;i<9;i++)    //应为刚开始的时候发送了屏幕变量没变化所以多发送几次
						{
							
					    USART_SendData(USART2,wendu[i]);
					    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
							
						}
		//			printf("读到的键盘码值为：%d",wendu[7]);
				} 
				
        delay_ms(20);
				
//				if(set_tong==1) kuang_1[7]=1; else kuang_1[7]=2 ;   
//        if(set_tong==2) kuang_2[7]=1; else kuang_2[7]=2 ;   //1显示方括号图像  2 不显示方括号图像
//        if(set_tong==3) kuang_3[7]=1; else kuang_3[7]=2 ;   //1显示方括号图像  2 不显示方括号图像
//        if(set_tong==4) kuang_4[7]=1; else kuang_4[7]=2 ;   //1显示方括号图像  2 不显示方括号图像
//        if(set_tong==5) kuang_5[7]=1; else kuang_5[7]=2 ;   //1显示方括号图像  2 不显示方括号图像
//        if(set_tong==6) kuang_6[7]=1; else kuang_6[7]=2 ;   //1显示方括号图像  2 不显示方括号图像
//				
			
			//再发送那个黄框框，那一个框不显示

				if(set_tong==1){
					
					kuang_1[7]=1;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==2){
					
					kuang_1[7]=2;
					kuang_2[7]=1;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==3){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=1;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==4){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=1;
					kuang_5[7]=2;
					kuang_6[7]=2;
					
				}
				
					if(set_tong==5){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=1;
					kuang_6[7]=2;
					
				}
				
				if(set_tong==6){
					
					kuang_1[7]=2;
					kuang_2[7]=2;
					kuang_3[7]=2;
					kuang_4[7]=2;
					kuang_5[7]=2;
					kuang_6[7]=1;
					
				}
				
				        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_1[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				
					        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_2[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
						        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_3[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				    delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_4[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
				
					        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_5[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}
						        delay_ms(10);
								for(i=0;i<8;i++)
						    {
									
									USART_SendData(USART2,kuang_6[i]);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
									
								}

								
 key_umb=key_staty;		//更新隔断扫描时间										
								
return 0;
	goto loop;
				
						
						
		}
		
		
		
		
	
	if(key==15){
		elect_time=7;
		
	  
		
				// 设置1通道  波形循环设置  
				if(set_tong==1){
					
					if(tongdao_1.bo_xin >=6) tongdao_1.bo_xin=1;
					else{
					 tongdao_1.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_1.bo_xin );
					
				boxin_1[7]=tongdao_1.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
					
				}
		
	// 设置2通道  波形循环设置  
				if(set_tong==2){
					
					if(tongdao_2.bo_xin >=6) tongdao_2.bo_xin=1;
					else{
					 tongdao_2.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_2.bo_xin );
					
				boxin_2[7]=tongdao_2.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	
	// 设置3通道  波形循环设置  
				if(set_tong==3){
					
					if(tongdao_3.bo_xin >=6) tongdao_3.bo_xin=1;
					else{
					 tongdao_3.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_2.bo_xin );
					
				boxin_3[7]=tongdao_3.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}

	// 设置4通道  波形循环设置  
				if(set_tong==4){
					
					if(tongdao_4.bo_xin >=6) tongdao_4.bo_xin=1;
					else{
					 tongdao_4.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_2.bo_xin );
					
				boxin_4[7]=tongdao_4.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	// 设置5通道  波形循环设置  
				if(set_tong==5){
					
					if(tongdao_5.bo_xin >=6) tongdao_5.bo_xin=1;
					else{
					 tongdao_5.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_2.bo_xin );
					
				boxin_5[7]=tongdao_5.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}

	// 设置6通道  波形循环设置  
				if(set_tong==6){
					
					if(tongdao_6.bo_xin >=6) tongdao_6.bo_xin=1;
					else{
					 tongdao_6.bo_xin++;					
						
					}
					
				//	printf("输出波形形状 ：%d",tongdao_2.bo_xin );
					
				boxin_6[7]=tongdao_6.bo_xin;
				
			  for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}




 key_umb=key_staty;		//更新隔断扫描时间		

	set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息			
				
				
return 0;
goto loop;




				
	}
	
	
	
	
	
	
//设置输出波形
	
	if(key==16){
		
		//设置1通道 周期向下减操作
		elect_time=7;
				if(set_tong==1){
				if(tongdao_1.zhou_time<=2) tongdao_1.zhou_time=2;
				else{
         tongdao_1.zhou_time--;					
					
				}
					
				zhou_1[7]=tongdao_1.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
				
			//设置2通道 周期向下减操作
		
			if(set_tong==2){
				if(tongdao_2.zhou_time<=2) tongdao_2.zhou_time=2;
				else{
         tongdao_2.zhou_time--;					
					
				}
					
				zhou_2[7]=tongdao_2.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
		//设置3通道 周期向下减操作
		
			if(set_tong==3){
				if(tongdao_3.zhou_time<=2) tongdao_3.zhou_time=2;
				else{
         tongdao_3.zhou_time--;					
					
				}
					
				zhou_3[7]=tongdao_3.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
			
				//设置3通道 周期向下减操作
		
			if(set_tong==4){
				if(tongdao_4.zhou_time<=2) tongdao_4.zhou_time=2;
				else{
         tongdao_4.zhou_time--;					
					
				}
					
				zhou_4[7]=tongdao_4.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
					
		//设置4通道 周期向下减操作
		
			if(set_tong==5){
				if(tongdao_5.zhou_time<=2) tongdao_5.zhou_time=2;
				else{
         tongdao_5.zhou_time--;					
					
				}
					
				zhou_5[7]=tongdao_5.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
					
		//设置6通道 周期向下减操作
		
			if(set_tong==6){
				if(tongdao_6.zhou_time<=2) tongdao_6.zhou_time=2;
				else{
         tongdao_6.zhou_time--;					
					
				}
					
				zhou_6[7]=tongdao_6.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
			
	set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息		
		
	 key_umb=key_staty;		//更新隔断扫描时间					
				
				return 0;
				goto loop;
				
				
	}
	
	
	
	//设置输出周期波，周期减
	
	if(key==17){
		
		elect_time=7;
		//设置通道1 周期加 操作  
				if(set_tong==1){
				if(tongdao_1.zhou_time >=199) tongdao_1.zhou_time=199;
				else{
         tongdao_1.zhou_time++;					
					
				}
					
				zhou_1[7]=tongdao_1.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}
	//设置通道2 周期加 操作  
				if(set_tong==2){
				if(tongdao_2.zhou_time >=199) tongdao_2.zhou_time=199;
				else{
         tongdao_2.zhou_time++;					
					
				}
					
				zhou_2[7]=tongdao_2.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}	
	//设置通道3 周期加 操作  
				if(set_tong==3){
				if(tongdao_3.zhou_time >=199) tongdao_3.zhou_time=199;
				else{
         tongdao_3.zhou_time++;					
					
				}
					
				zhou_3[7]=tongdao_3.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}				
		
			//设置通道4 周期加 操作  
				if(set_tong==4){
				if(tongdao_4.zhou_time >=199) tongdao_4.zhou_time=199;
				else{
         tongdao_4.zhou_time++;					
					
				}
					
				zhou_4[7]=tongdao_4.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}	
	//设置通道5 周期加 操作  
				if(set_tong==5){
				if(tongdao_5.zhou_time >=199) tongdao_5.zhou_time=199;
				else{
         tongdao_5.zhou_time++;					
					
				}
					
				zhou_5[7]=tongdao_5.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
				
	//设置通道6 周期加 操作  
				if(set_tong==6){
				if(tongdao_6.zhou_time >=199) tongdao_6.zhou_time=199;
				else{
         tongdao_6.zhou_time++;					
					
				}
					
				zhou_6[7]=tongdao_6.zhou_time;
				
					for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
				
				}		
				
	set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
				
	 key_umb=key_staty;		//更新隔断扫描时间					
return 0;			
goto loop;
				
	}
	
	
	
	
	
	
	
	
 
	//输出周期加++
	if(key==18){
	elect_time=7;
	//配置通道1的治疗时间
		if(set_tong==1){
			
			if(tongdao_1.set_time <=10) tongdao_1.set_time=65;
			
			tongdao_1.set_time=(tongdao_1.set_time-5);
			

			
			time_1[7]=tongdao_1.set_time;
			tongdao_1.shen_time = tongdao_1.set_time;
			
			ch1_start=2;  //打开电脉冲输出开关
			
		//	printf("通道1设置时间 ：%d",tongdao_1.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
	//配置通道2的时间
	if(set_tong==2){
			
			if(tongdao_2.set_time <=10) tongdao_2.set_time=65;
			
			tongdao_2.set_time=(tongdao_2.set_time-5);
			

			
			time_2[7]=tongdao_2.set_time;
		  tongdao_2.shen_time = tongdao_2.set_time;	
		  ch2_start=2;  //打开电脉冲输出开关
		//	printf("通道1设置时间 ：%d",tongdao_2.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
		//配置通道3的时间
	if(set_tong==3){
			
			if(tongdao_3.set_time <=10) tongdao_3.set_time=65;
			
			tongdao_3.set_time=(tongdao_3.set_time-5);
			

			
			time_3[7]=tongdao_3.set_time;
		tongdao_3.shen_time = tongdao_3.set_time;
			ch3_start=2;  //打开电脉冲输出开关
		//	printf("通道1设置时间 ：%d",tongdao_3.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
		//配置通道3的时间
	if(set_tong==4){
			
			if(tongdao_4.set_time <=10) tongdao_4.set_time=65;
			
			tongdao_4.set_time=(tongdao_4.set_time-5);
			

			
			time_4[7]=tongdao_4.set_time;
		tongdao_4.shen_time = tongdao_4.set_time;
			ch4_start=2;  //打开电脉冲输出开关
		//	printf("通道1设置时间 ：%d",tongdao_4.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
		
			//配置通道5的时间
	if(set_tong==5){
			
			if(tongdao_5.set_time <=10) tongdao_5.set_time=65;
			
			tongdao_5.set_time=(tongdao_5.set_time-5);
			

			
			time_5[7]=tongdao_5.set_time;
			tongdao_5.shen_time = tongdao_5.set_time;
		ch5_start=2;  //打开电脉冲输出开关
		//	printf("通道1设置时间 ：%d",tongdao_5.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}
			//配置通道6的时间
	if(set_tong==6){
			
			if(tongdao_6.set_time <=10) tongdao_6.set_time=65;
			
			tongdao_6.set_time=(tongdao_6.set_time-5);
			

			
			time_6[7]=tongdao_6.set_time;
			tongdao_6.shen_time = tongdao_6.set_time;
		  ch6_start=2;  //打开电脉冲输出开关
		//	printf("通道1设置时间 ：%d",tongdao_6.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
			
			
		}	
	
		
	key_umb=key_staty;		//更新隔断扫描时间			
	return 0;	
	goto loop;	
		
		
}















	
 //治疗时间

  key=get_hot1_6_num();

 if(key==6)
 {
	 elect_time=3;
	 
	 H1_BOOl=!H1_BOOl;
	 
	 if(H1_BOOl==1){ MCU_H1=1; H1_um=1;     }
	 if(H1_BOOl==0){ MCU_H1=0;  H1_um=0;    }
	 
  // printf("加热按键1通道  ：%d",H1_BOOl);
	 H1_um=(H1_um<<5);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
		//	printf("通道1设置时间 ：%d",tongdao_1.set_time );
			
			 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
	 
	key_umb=key_staty;		//更新隔断扫描时间			
	return 0;	
	goto loop;	
		
	 
	}		
	
	if(key==5)
 {
	 elect_time=3;
	 H2_BOOl=!H2_BOOl;
	 
	 if(H2_BOOl==1){ MCU_H2=1;  H2_um=1;       }
	 if(H2_BOOl==0){ MCU_H2=0;  H2_um=0;       }
	 
	 H2_um=(H2_um<<4);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 		 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
	 
	 
	 	key_umb=key_staty;		//更新隔断扫描时间			
	return 0;	
	goto loop;	
		
					
	}
 
	if(key==4)
 {
	 elect_time=3;
	 H3_BOOl=!H3_BOOl;
	 
	 if(H3_BOOl==1) {MCU_H3=1;   H3_um=1;    }
	 if(H3_BOOl==0) {MCU_H3=0;   H3_um=0;    }
	 
	 
	  H3_um=(H3_um<<3);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 
	 		 for(j=0;j<5;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,HOT_LED[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 	
	 
 	 key_umb=key_staty;		//更新隔断扫描时间			
	 return 0;	
	 goto loop;	
		
	 
	}
	
	if(key==3)
 {
	 elect_time=3;
	 
	 H4_BOOl=!H4_BOOl;
	 
	 if(H4_BOOl==1){ MCU_H4=1;   H4_um=1;     }
	 if(H4_BOOl==0){ MCU_H4=0;   H4_um=0;     }
	 
	 H4_um=(H4_um<<2);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	 	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 	
	 
	  key_umb=key_staty;		//更新隔断扫描时间			
	  return 0;	
	  goto loop;	
		
	 
	}
	
	if(key==2)
 {
	 
	 elect_time=3;
	 H5_BOOl=!H5_BOOl;
	 
	 if(H5_BOOl==1) {MCU_H5=1; H5_um=1;     }
	 if(H5_BOOl==0) {MCU_H5=0; H5_um=0;    }
	 
	 H5_um=(H5_um<<1);
	 
	 HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	  	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	 
	 
	 
 	 key_umb=key_staty;		//更新隔断扫描时间			
	 return 0;	
	 goto loop;	
		
	 
	}
	
	 if(key==1)
 {
	 elect_time=3;

	 H6_BOOl=!H6_BOOl;
	 
	 if(H6_BOOl==1){ MCU_H6=1; H6_um=1;     }
	 if(H6_BOOl==0){ MCU_H6=0; H6_um=0;     }
	 
	  HOT_unm= (H1_um+H2_um+H3_um+H4_um+H5_um+H6_um);
	 
	 	HOT_LED[7]=HOT_unm;
			
	 
	   	for(j=0;j<5;j++){
			 for(i=0;i<8;i++)
					{
						
						USART_SendData(USART2,HOT_LED[i]);
						while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
						
					}
					} 
	 
	 
	 
  // printf("加热按键6通道  ：%d",H6_BOOl);

					
 key_umb=key_staty;		//更新隔断扫描时间	
 return 0;						
 goto loop;
	 
	}
	
	
	
  
//	if(key_umb<=1){
//	                  
//	KEY_Scan_dian();//没有其他按键按下后才扫描电位器开关按键。
//  key_umb=1;
//		
//	}
//	
//	key_umb--;
//	
	
	get_eleck1_6_num();   //更新电脉冲信息到显示屏上。将电脉冲开关量设置进入电针治疗发波
	
	
	
 loop:
	
return 0;
	
	
}

//将上次存储的治疗信息参数更新到屏幕上作为开机显示内容。
void set_ch1_6_to_display(void)
{
	u8 i,j,k;
	
	time_1[7]=tongdao_1.set_time;
	time_2[7]=tongdao_2.set_time;
	time_3[7]=tongdao_3.set_time;
	time_4[7]=tongdao_4.set_time;
	time_5[7]=tongdao_5.set_time;
	time_6[7]=tongdao_6.set_time;
	
	
	zhou_1[7]=tongdao_1.zhou_time;
	zhou_2[7]=tongdao_2.zhou_time;
	zhou_3[7]=tongdao_3.zhou_time;
	zhou_4[7]=tongdao_4.zhou_time;
	zhou_5[7]=tongdao_5.zhou_time;
	zhou_6[7]=tongdao_6.zhou_time;
	
	boxin_1[7]=tongdao_1.bo_xin;
	boxin_2[7]=tongdao_2.bo_xin;
	boxin_3[7]=tongdao_3.bo_xin;
	boxin_4[7]=tongdao_4.bo_xin;
	boxin_5[7]=tongdao_5.bo_xin;
	boxin_6[7]=tongdao_6.bo_xin;
	
	///////////////////////////发生通道1的治疗信息/////////////////////////////////////////////
		for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////发生通道2的治疗信息/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
					
///////////////////////////发生通道3的治疗信息/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_3[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 					
///////////////////////////发生通道4的治疗信息/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_4[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////发生通道5的治疗信息/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
///////////////////////////发生通道6的治疗信息/////////////////////////////////////////////
	   delay_ms(200);
						for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 
	delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,zhou_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 		
		delay_ms(200);
				for(j=0;j<2;j++){
					 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,boxin_6[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
					} 			
					
					
					
					
	
}












//获得设置通道1到通道6的治疗参数的配置信息的键盘码信息。
u8 get_set_num(void)
{
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0,i=0;
	u8 mode;
	mode=1; //等于1支持连续按键操作，等于0不支持连续按键操作
	
	HOT_DQ_OUT=1;
	SELET_DQ_OUT=1;
	SET_DQ_OUT=0;
	delay_ms(10);
	
  k6=KOY6_DQ_IN;  //探穴
  k5=KOY5_DQ_IN;  //周期减
  k4=KOY4_DQ_IN;  //周期加
  k3=KOY3_DQ_IN;  //通道
  k2=KOY2_DQ_IN;  //波形
  k1=KOY1_DQ_IN;  //时间
	
	if(mode)key_up=1;  //是否支持支持连按键操作。
	
	if(key_up&&(k6==0||k5==0||k4==0||k3==0||k2==0||k1==0))
	{
		delay_ms(10);
		key_up=0;
		if(k6==0) {  key=13; delay_ms(200);   //短按返回探穴键盘码
     while(!KOY6_DQ_IN){
			delay_ms(200);
			i++; 
		 }
		 if(i>=8) {  key=19;  }   //长按探穴2秒以上检测到为保存治疗数据的功能。

		 if(i>=15) { key=20; i=0;}   //长按探穴10秒以上为关闭输出短路开路保护提示功能
		 
		return key ; }
		else if (k5==0){key=16;delay_ms(200); return key; }
		else if (k4==0){key=17;delay_ms(200); return key; }
		else if (k3==0){key=14;delay_ms(200); return key; }
		else if (k2==0){key=15;delay_ms(200); return key; }
		else if (k1==0){key=18;delay_ms(200); return key; }
		
	}else if(k6==1&&k5==1&&k4==1&&k3==1&&k2==1&&k1==1)  key_up=1;
	
	return 0;
	
	
	
}



//获得设置通道1到通道6的温针开关配置信息的键盘码信息。
u8 get_hot1_6_num(void)
{
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	u8 mode;
	mode=0; //等于1支持连续按键操作，等于0不支持连续按键操作                
        
	HOT_DQ_OUT=0;
	SELET_DQ_OUT=1;
	SET_DQ_OUT=1;
	delay_ms(10);
	
  k6=KOY6_DQ_IN; //加热6
  k5=KOY5_DQ_IN; //加热5
  k4=KOY4_DQ_IN; //加热4
  k3=KOY3_DQ_IN; //加热3
  k2=KOY2_DQ_IN; //加热2
  k1=KOY1_DQ_IN; //加热1
	
	if(mode)key_up_1=1;  //是否支持支持连按键操作。
	
	if(key_up_1&&(k6==0||k5==0||k4==0||k3==0||k2==0||k1==0))
	{
		delay_ms(10);
		key_up_1=0;
		if(k6==0) {     key=1;while(!KOY6_DQ_IN);return key; }
		else if (k5==0){key=2;while(!KOY5_DQ_IN);return key; }
		else if (k4==0){key=3;while(!KOY4_DQ_IN);return key; }
		else if (k3==0){key=4;while(!KOY3_DQ_IN);return key; }
		else if (k2==0){key=5;while(!KOY2_DQ_IN);return key; }
		else if (k1==0){key=6;while(!KOY1_DQ_IN);return key; }
		
		delay_ms(200);
		
		
	}else if(k6==1&&k5==1&&k4==1&&k3==1&&k2==1&&k1==1)  key_up_1=1;
	
	
	
	return 0;
	
	
	
}

//获得设置通道1到通道6的电脉冲信息的键盘码信息。
u8 get_eleck1_6_num(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 key;
	u8 k6=0,k5=0,k4=0,k3=0,k2=0,k1=0;
	u8 mode;
	mode=0;   //等于1支持连续按键操作，等于0不支持连续按键操作                
 
	
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTE时钟
  //键盘扫描行码初始化配置
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    //加热按键的行码输出公共端 第一版本是PE0 后改称PE0 ; 脉冲电针输出公共端 第一版本是PC13 后改称PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;          //GPIO_Mode_IPU; //设置成上拉输入 后改为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //初始化	

	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //设置键盘公共端输出位 第一版本是PD9 第2版本改成 PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成上拉输入  后改称推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	
	
	
	//HOT_DQ_OUT=1;
	SELET_DQ_OUT=0;
	//SET_DQ_OUT=1;
	delay_ms(10);
	
	
	
	
  k6=KOY6_DQ_IN; //加热6
  k5=KOY5_DQ_IN; //加热5
  k4=KOY4_DQ_IN; //加热4
  k3=KOY3_DQ_IN; //加热3
  k2=KOY2_DQ_IN; //加热2
  k1=KOY1_DQ_IN; //加热1
	
	
	ELCK1_um=(k1<<5);
	ELCK2_um=(k2<<4);
	ELCK3_um=(k3<<3);
	ELCK4_um=(k4<<2);
	ELCK5_um=(k5<<1);
	ELCK6_um=k6;
	
	ELCK_unm=(ELCK1_um+ELCK2_um+ELCK3_um+ELCK4_um+ELCK5_um+ELCK6_um);
	

	
	
//	//判断U1_P是否继续增加，用于判断串口发送是否完成

 if(CK_ELE(ELCK_unm))      //检测键盘码有没有更新
	{	
	    tong_start=ELCK_unm; //将通道电脉冲变量信息传递出去  
		
		  printf("更新电脉冲键盘值\r\n");
		
			ELCK_LED[7]=ELCK_unm;		 
			if( ELCK_LED[7]==0x3f)
			{
				system_start_EL=1; 
				 
			 }
	
	 
		 fa_set_elect();
		 
		
		
	}
	
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    //加热按键的行码输出公共端 第一版本是PE0 后改称PE0 ; 脉冲电针输出公共端 第一版本是PC13 后改称PE6
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;          //GPIO_Mode_IPU; //设置成上拉输入 后改为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);        //初始化	
  GPIO_SetBits(GPIOE,GPIO_Pin_0);
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;    //设置键盘公共端输出位 第一版本是PD9 第2版本改成 PD9     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成上拉输入  后改称推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	
	
	
	
	
//	
//	///////////////////连续对比几次数据都相同则更像发送数列数组内容，不相同则删除/避免吴发送出现误判的情况//////////////////////
//	 ELCK_UF[ELCK_unm_i]=ELCK_unm;
//	 ELCK_unm_i++;
//	 
//	 if(ELCK_unm_i>=2){
//		 ELCK_unm_i=0;
//		 if((ELCK_UF[0]==ELCK_unm)&&(ELCK_UF[1]==ELCK_unm))
//		 {
//			 
//			 ELCK_LED[7]=ELCK_unm;		 
//			if( ELCK_LED[7]==0x3f)
//			 {
//				system_start_EL=1; 
//				 
//			 }
//			 
//			 
//		 }

//		 fa_set_elect();

//	 }
//	


	
	
	return 0;
	
	
	
}

//检测数据内容，键盘值发生变化时
u8 CK_ELE(u8 key)
{
	if(key_ele_unm!=key){
		
		key_ele_unm=key;
		return 1;
		
	}
	
	//key_ele_unm=0;
	return 0;
}






//将输出波形信息周期，设置治疗时间信息对接如

void set_ch1_6_sinc(void)
{
	//对接输出治疗波形
	 set_ch1_bo=tongdao_1.bo_xin;
   set_ch2_bo=tongdao_2.bo_xin;
	 set_ch3_bo=tongdao_3.bo_xin;
	 set_ch4_bo=tongdao_4.bo_xin;
   set_ch5_bo=tongdao_5.bo_xin;
	 set_ch6_bo=tongdao_6.bo_xin;
	//对接输出周期数据
	ch1_unm=(tongdao_1.zhou_time*10);
	ch2_unm=(tongdao_2.zhou_time*10);
	ch3_unm=(tongdao_3.zhou_time*10);
	ch4_unm=(tongdao_4.zhou_time*10);
	ch5_unm=(tongdao_5.zhou_time*10);
	ch6_unm=(tongdao_6.zhou_time*10);
	
	
}


void set_ch1_sinc(void)
{

	ch1_unm=(tongdao_1.zhou_time*10);


}


void set_ch2_sinc(void)
{

	ch2_unm=(tongdao_2.zhou_time*10);


}

void set_ch3_sinc(void)
{

	ch3_unm=(tongdao_3.zhou_time*10);


}


void set_ch4_sinc(void)
{

	ch4_unm=(tongdao_4.zhou_time*10);


}



void set_ch5_sinc(void)
{

	ch5_unm=(tongdao_5.zhou_time*10);


}

void set_ch6_sinc(void)
{

	ch6_unm=(tongdao_6.zhou_time*10);


}




//将1通道至6通道治疗信息存储到W25Q80的芯片内部
void save_ch1_6_24c02(void)
{
	//将通道1到通道6的治疗信息存储在datch1_6unm[]数组里面。
	u8 datch1_6unm[19];
	
	datch1_6unm[0]=tongdao_1.set_time;
	datch1_6unm[1]=tongdao_1.zhou_time;
	datch1_6unm[2]=tongdao_1.bo_xin;
	
	datch1_6unm[3]=tongdao_2.set_time;
	datch1_6unm[4]=tongdao_2.zhou_time;
	datch1_6unm[5]=tongdao_3.bo_xin;
	
	datch1_6unm[6]=tongdao_3.set_time;
	datch1_6unm[7]=tongdao_3.zhou_time;
	datch1_6unm[8]=tongdao_3.bo_xin;
	
	datch1_6unm[9]=tongdao_4.set_time;
	datch1_6unm[10]=tongdao_4.zhou_time;
	datch1_6unm[11]=tongdao_4.bo_xin;
	
	datch1_6unm[12]=tongdao_5.set_time;
	datch1_6unm[13]=tongdao_5.zhou_time;
	datch1_6unm[14]=tongdao_5.bo_xin;
	
	datch1_6unm[15]=tongdao_6.set_time;
	datch1_6unm[16]=tongdao_6.zhou_time;
	datch1_6unm[17]=tongdao_6.bo_xin;

  datch1_6unm[18]=33;
	
	W25QXX_Write(datch1_6unm,100,19);
  delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);
	delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);
	delay_ms(10);
  W25QXX_Write(datch1_6unm,100,19);

	
}


//将1至6通道治疗数据从W25Q80中读取出来
void read_ch1_6_at2402(void)
{
	u8 data_ch1_6_read[19];
	
	//AT24CXX_Read(0,data_ch1_6_read,19);
	
	W25QXX_Read(data_ch1_6_read,100,19);
	
	tongdao_1.set_time=data_ch1_6_read[0];
	tongdao_1.zhou_time=data_ch1_6_read[1];
	tongdao_1.bo_xin=data_ch1_6_read[2];
	
	tongdao_2.set_time=data_ch1_6_read[3];
	tongdao_2.zhou_time=data_ch1_6_read[4];
	tongdao_2.bo_xin=data_ch1_6_read[5];
	
	tongdao_3.set_time=data_ch1_6_read[6];
	tongdao_3.zhou_time=data_ch1_6_read[7];
	tongdao_3.bo_xin=data_ch1_6_read[8];
	
	tongdao_4.set_time=data_ch1_6_read[9];
	tongdao_4.zhou_time=data_ch1_6_read[10];
	tongdao_4.bo_xin=data_ch1_6_read[11];
	
	tongdao_5.set_time=data_ch1_6_read[12];
	tongdao_5.zhou_time=data_ch1_6_read[13];
	tongdao_5.bo_xin=data_ch1_6_read[14];
	
	tongdao_6.set_time=data_ch1_6_read[15];
	tongdao_6.zhou_time=data_ch1_6_read[16];
	tongdao_6.bo_xin=data_ch1_6_read[17];
	
	
	tongdao_1.shen_time = tongdao_1.set_time;
	tongdao_2.shen_time = tongdao_2.set_time;
	tongdao_3.shen_time = tongdao_3.set_time;
	tongdao_4.shen_time = tongdao_4.set_time;
	tongdao_5.shen_time = tongdao_5.set_time;
	tongdao_6.shen_time = tongdao_6.set_time;
	
	
	
	
}



//将治疗时间和剩余治疗时间发生到屏幕上
void disply_time(void)
{
	u8 tong_S=0,i=0;
	tong_S=tong_start;
	

	
	if(!(tong_S&0x20))
	{

		
   if(tongdao_1.shen_time>=1){
	
		ch1_start=2;
		if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_1.shen_time--;  
		}
	 }else {    
	 
		tan_xue_boo=2;
		 
	 ch1_start=0;
	 
	 }
	 
	 time_1[7]=tongdao_1.shen_time;
	 
	 
		for(i=0;i<8;i++)
		{
								
								USART_SendData(USART2,time_1[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}  
	 
	 
	 
	}
	
		if(!(tong_S&0x10))
	{
	
		
		
   if(tongdao_2.shen_time>=1){

		ch2_start=2;
		if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_2.shen_time--;  
		}
	 }else {    
	 
		
		 tan_xue_boo=2;
	 ch2_start=0;
	 
	 }
	 
	 time_2[7]=tongdao_2.shen_time;
	 
	  			  for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_2[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
	 
	 
	}
		if(!(tong_S&0x08))
	{
		
	
   if(tongdao_3.shen_time>=1){
			ch3_start=2;
		 	if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_3.shen_time--; 
			}
	 }else {    
	 
		
		 
	 ch3_start=0;
	 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 
	 time_3[7]=tongdao_3.shen_time;
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_3[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	if(!(tong_S&0x04))
	{
		
		
		
	
   if(tongdao_4.shen_time>=1){
		ch4_start=2;
	if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_4.shen_time--; 
		}
	 }else {    
	 
		
		 
	 ch4_start=0;
	 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 time_4[7]=tongdao_4.shen_time;
	 
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_4[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	if(!(tong_S&0x02))
	{
	
		
		

   if(tongdao_5.shen_time>=1){
	  ch5_start=2;
	if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_5.shen_time--; 
			}
	 }else{
		 
		 
		 ch5_start=0;
	 tan_xue_boo=2;
		 
	 }
	 
	 time_5[7]=tongdao_5.shen_time;
	 
	 		 				 for(i=0;i<8;i++)
							{
								
								USART_SendData(USART2,time_5[i]);
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
								
							}
	 
	 
	 
	}
	
	if(!(tong_S&0x01))
	{
	  
		
		

   if(tongdao_6.shen_time>=1){
		ch5_start=2;
	if(system_start_EL){   //闭锁后治疗剩余时间不减小
		tongdao_6.shen_time--;
	}
	
	 }else{
		 
		 ch6_start=0;
		 tan_xue_boo=2;
	 }
	 
	 
	 
	 
	 time_6[7]=tongdao_6.shen_time;
			 for(i=0;i<8;i++)
			{
				
				USART_SendData(USART2,time_6[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				
			}

	 
	 
	}
	
	
}

