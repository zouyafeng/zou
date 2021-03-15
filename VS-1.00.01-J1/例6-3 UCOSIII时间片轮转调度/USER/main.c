#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "includes.h"
#include "usart3.h"
#include "BC26.h"
#include "rtc.h" 
#include "ds18b20.h" 
#include "timer.h"
#include "key.h"
#include "adc.h"
#include "exti.h"
#include "24cxx.h"
#include "w25qxx.h"	


////////////////////////版本管理号/////////////////////////////////////
/////////////////////VS 1.00.12.J0////////////////////////////////////////////////


u8 bisuo_boo=1;     //闭锁关闭等于0，闭锁打开1    关闭和打开闭锁管理变量
u8 jh_boot=0;
u8 zd_che_net=0;
u8 time_t=0;

u16 feng_time=0; //定时一分钟变量  实在是200ms定时器 所以定时1分钟为300
u16 elect_time=36; //定时更新屏幕上电脉冲显示图标状态

//对按摩E1波，没多长时间改变一次进行定义，初步设定为1.2秒改变一次，这样就可以在改变一次周期的时候看到一个0.56S的波形
u8 ch1_time_s=0,ch2_time_s=0,ch3_time_s=0,ch4_time_s=0,ch5_time_s=0,ch6_time_s=0;


extern u8 clos_bis_unm;//添加长按探穴按键关闭闭锁报警功能  


extern u8 set_ch1_bo;  //开机默认是连续波输出
extern u8 set_ch2_bo;  //开机默认是连续波输出
extern u8 set_ch3_bo;  //开机默认是连续波输出
extern u8 set_ch4_bo;  //开机默认是连续波输出
extern u8 set_ch5_bo;  //开机默认是连续波输出
extern u8 set_ch6_bo;  //开机默认是连续波输出

extern u16 ch2_unm_E1_t ; //通道2按摩波E1 自动变化周期 
extern u16 ch6_unm_E1_t ; //通道2按摩波E1 自动变化周期 
extern u16 ch5_unm_E1_t ; //通道2按摩波E1 自动变化周期 
extern u16 ch4_unm_E1_t ; //通道2按摩波E1 自动变化周期 
extern u16 ch3_unm_E1_t ; //通道2按摩波E1 自动变化周期 
extern u16 ch1_unm_E1_t ; //通道2按摩波E1 自动变化周期 




u8 zh_send_A2=0;
extern u8  seve_biaoding;

u16 work_REST=0;
u16 LED_PWOR=150; //定义灯的亮度初始化的时候50%的亮度

u8 wendu[9]={0xA5,0x5A,0x05,0x82,0x00,0x33,0x00,0x00};  //发送温度的变量数组
u8 hour_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x22,0x00,0x00}; //发送小时的变量数组
u8 feng_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x24,0x00,0x00}; //发送分钟的变量数组
u8 miao_buf[9]={0xA5,0x5A,0x05,0x82,0x00,0x26,0x00,0x00}; //发送秒钟的变量数组

u8 xian_buf[6]={0xA5,0x5A,0x03,0x80,0x01,0x20};
extern u16 ch1_time,ch2_time,ch3_time,ch4_time,ch5_time,ch6_time;      //定时器里面的变量记录那个通道ms值，使输出每一个通道值在1ms内不会重复输出

extern u8 LED1_EN, LED2_EN, LED3_EN, LED4_EN;

u8 time_num_low=1;
u8 jb_time_num=1;
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define TASK1_TASK_PRIO		4
//任务堆栈大小	
#define TASK1_STK_SIZE 		2048
//任务控制块
OS_TCB Task1_TaskTCB;
//任务堆栈	
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//任务优先级
#define TASK2_TASK_PRIO		5
//任务堆栈大小	
#define TASK2_STK_SIZE 		128
//任务控制块
OS_TCB Task2_TaskTCB;
//任务堆栈	
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
//任务函数
void task2_task(void *p_arg);

extern u8 send_biaodin_date[] ;


OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数
u8 system_int=0;
extern u8 send_zhouqi_date[]; //周期上次数据内容
extern u8 zhouqi_bool;
extern  u8 xin_tiao_time[];  //心跳周期上传数据
extern u8 sever_time ;   //是否收到时间变量 

short temperature;

extern u8 bool_SHU ;

u16 num_set=0;

u8 system_start=0;
u8 system_start_EL=1;  //等于0是启动闭锁功能， 等于1时关闭闭锁功能

extern u16 mang_1,mang_2;


//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer[]={"FFFFFFFFFFFFSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)


//主函数
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u8 rtc_num=0;
	u8 i=0,y=0,i_1,i_2,i_3;
	int k;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE; 
	
  delay_init(); 
	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	off_4051_int();
	
	uart_init(115200); 	//串口初始化
	usart3_init(115200);
	usart2_init(115200);
  LED_Init();

	//LCD_Init();			//LCD初始化	
	//read_apn_power(); //读取上次存储的APN 名字 密码  
	//BC20_init();
	//clean_rebuff1();
	//rtc_num=RTC_Init();	  			//RTC初始化
	
	TIM3_Int_Init(18,3600);    // 定时器配置1ms进入一次中断，用来产生通道1至通道6有规律的输出波形信息   
	
	KEY_Init();
	

	
	EXTIX_Init(); //启动外部探穴功能 
	Adc_Init();   //启动ADC检查开路短路功能初始化配置
	
	W25QXX_Init();			//W25QXX初始化
	
	if(W25QXX_ReadID()==W25Q80)
	{
		printf("系统初始化AT24C20:成功\r\n");
	}else{
		
		printf("系统初始化AT24C20:失败\r\n");
		
	}
	
	
	
//	W25QXX_Write((u8*)TEXT_Buffer,100,SIZE);			//从倒数第100个地址处开始,写入SIZE长度的数据
//	delay_ms(100);
//	W25QXX_Read(datatemp,100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
//	
//	printf("系统初始化AT24C20读取:%s\r\n",datatemp);
//	

//  AT24CXX_Init();			  //IIC初始化 
//	rtc_num=AT24CXX_Check();
//	
//	//if(rtc_num==1) { printf("系统初始化AT24C20:失败\r\n");}
// while(AT24CXX_Check()){
//	 printf("系统初始化AT24C20:失败\r\n");
//	 delay_ms(500);
//	 
// }


	
//	AT24CXX_WriteOneByte(0,25);
//	AT24CXX_WriteOneByte(1,24);
//	AT24CXX_WriteOneByte(2,23);
//	AT24CXX_WriteOneByte(3,22);
//	i=AT24CXX_ReadOneByte(0);
//	i_1=AT24CXX_ReadOneByte(1);
//	i_2=AT24CXX_ReadOneByte(2);
	
	
	
	delay_ms(100);	
	W25QXX_Read(datatemp,100,19);					//从倒数第100个地址处开始,读出SIZE个字节
	
	i=datatemp[18];
	
	if(i==33){ read_ch1_6_at2402();}  //将存储的治疗数据读取到系统里。
	
	
//	i_1=AT24CXX_ReadOneByte(17);
	
	printf("W25qxx读出的数据为:%d,%d\r\n",i,i_1);
	
	
	
	//TIM3_PWM_Init(899,270);	//不分频。PWM频率=72000000/900=80Khz  899   (899999,4)
	
	printf("系统初始化过程");
	delay_ms(2000);
  delay_ms(2000);
	
	for(y=0;y<7;y++){
	for(i=0;i<7;i++)
	{					
			USART_SendData(USART2,xian_buf[i]);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);				
	}
	
}
	

//将上次保存的电针治疗波形信息发送到显示屏上去
 set_ch1_6_to_display();
 set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息







//开机电位器置为关闭状态 不然蜂鸣器会叫  	
	
while(Start_KEY_Scan_dian()){
	
	LEEP=1;

	
}

LEEP=0;

OE_1=0;
POWER_1=1;
	




	
	OSInit(&err);		     //初始化UCOSIII
	OS_CRITICAL_ENTER(); //进入临界区			 
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
								 
}


//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )1,			//20*10=200ms
                (OS_TICK	 )20,          //100*10=1000ms  1×10=20ms一次
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
		
								
	OS_CRITICAL_ENTER();	//进入临界区
								
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  //2个时间片，既2*5=10ms					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//创建TASK2任务
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				         (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,	//2个时间片，既2*5=10ms					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			 
	OS_CRITICAL_EXIT();	//退出临界区
								 
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
								 
								 
}




//串口3收到上位机的执行命令，进行操作  激活 查询 关机 传输数据    查询信号强度 进行数据传输  每个命令都是要进行回复给上位机 
//


//task1任务函数
void task1_task(void *p_arg)
{
	u8 i,j,task1_num=0;
	u8 key=0; 
	OS_ERR err;
	p_arg = p_arg;

	OSTmrStart(&tmr1,&err);	//开启定时器1
	
  
	
	while(1)
	{
		
	
  system_start=2;    //定时器脉冲输出中开关
		
//	system_start_EL=1; //测试版本关闭闭锁版本	
		
	//USART2_GET();
	

   key_set();
		
//    MCU_O2=0;
		
//		LED_3=0;
//		LED_4=0;
//		MCU_MUS=0;
//		
//		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
//		LED_3=1;
//		LED_4=1;
//	  MCU_MUS=1;
		
		

	 OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s	
		
		
	}
	
	
}

//task2任务函数
void task2_task(void *p_arg)
{
	u8 i,j;
	OS_ERR err;
	u16 temp=0;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	

	while(1)
	{


	//	ch3_time
		
		
	
   //printf("1:%d\r\n",mang_1);
   //printf("2:%d\r\n",mang_2);
		
//	 printf("第二路的值:%d\r\n",ch2_time);
//   printf("第三路的值:%d\r\n",ch3_time);


		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s

		

		
		
	}
	
}




//定时器1的回调函数
void tmr1_callback(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	u8 i,t,a,b,c;
	p_arg = p_arg;
  
	//200ms进入一次定时器 
//	set_ch1_6_sinc(); //将屏幕设置通道信息同步如系统，输出波形内容信息
	   
	on_leep();        //打开蜂鸣器操作函数，启动蜂鸣器 
	//新版本短路开路检测程序，待测试
  check_ch1x6_dk();
	
  LED_G= ~LED_G;
	
 

	//通道2按摩波周期自动变化
	if(set_ch2_bo==4){
		
		ch2_time_s++;
		
		if(ch2_time_s>=6){
		ch2_unm_E1_t=(ch2_unm_E1_t-10);
		if(ch2_unm_E1_t<=12){
			
		ch2_unm_E1_t=560;
		}
		ch2_time_s=0;
	}
		
	}
	
		//通道2按摩波周期自动变化
	if(set_ch6_bo==4){
		
			ch6_time_s++;
		
		if(ch6_time_s>=6){
		ch6_unm_E1_t=(ch6_unm_E1_t-10);
		if(ch6_unm_E1_t<=12){
			
			ch6_unm_E1_t=560;
		}
	  ch6_time_s=0;
	}
		
	}
	
	
			//通道2按摩波周期自动变化
	if(set_ch5_bo==4){
		
		ch5_time_s++;
		if(ch5_time_s>=6){
		ch5_unm_E1_t=(ch5_unm_E1_t-10);
		if(ch5_unm_E1_t<=12){
			
			ch5_unm_E1_t=560;
		}
		ch5_time_s=0;
		
		}
	
		
		
	}
	
			//通道2按摩波周期自动变化
	if(set_ch4_bo==4){
		
		ch4_time_s++;
		if(ch4_time_s>=6){
		ch4_unm_E1_t=(ch4_unm_E1_t-10);
		if(ch4_unm_E1_t<=12){
			
			ch4_unm_E1_t=560;
		}
			ch4_time_s=0;
		
		}
		
		
		
	}
	
				//通道2按摩波周期自动变化
	if(set_ch3_bo==4){
		
		
		ch3_time_s++;
		if(ch3_time_s>=6){
		ch3_unm_E1_t=(ch3_unm_E1_t-10);
		if(ch3_unm_E1_t<=12){
			
			ch3_unm_E1_t=560;
		}
		ch3_time_s=0;
		
		}
		
	}
	
			//通道2按摩波周期自动变化
	if(set_ch1_bo==4){
		
		ch1_time_s++;
		if(ch1_time_s>=6){	
		ch1_unm_E1_t=(ch1_unm_E1_t-10);
		if(ch1_unm_E1_t<=12){
			
			ch1_unm_E1_t=560;
		}
			ch1_time_s=0;
		
		}
		
		

		
		
	}
	
	
 //定时治疗时间1分钟，进入一次时间记录函数将治疗时间发生到现实屏
	
	feng_time++;
	if(feng_time>=300){
	feng_time=0;	
	disply_time();	
		
	}

//定时更新显示屏上电脉冲图标状态
//	elect_time--;
//if(elect_time<=1){
//	elect_time=2;	
//	fa_set_elect();
//		
//	}
	
		//num_set=0;
		
	//	set_ms();
//  ch_X_set_ms(1,2);
 //   ch_X_set_ms(2,1);
//	
//	ch_X_set_ms(3,1);
//	ch_X_set_ms(4,1);
//	ch_X_set_ms(5,1);
//	
// ch_X_set_ms(6,1); 
		
	
//   texi_ch2();
		
//	MCU_O2=~MCU_O2;
		

	
}










