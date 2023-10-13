#include "delay.h"
#include "lcd.h"
#include "lcd_init.h"
#include "UI.h"
#include "key.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "RC522.h"
#include "timer.h"
#include "as608.h"
#include "as608user.h"
#include "exti.h"
#include "beep.h"
#include "Flash.h"
	
#define usart1_baund  57600

int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LED_Init();    //默认初始阶段消毒LED和提示LED均不亮
	Sanitize_LED_Init();
	UI_Init();     //初始化界面
	Key_Init();    //矩阵键盘初始化
	BEEP_Init();   //初始化蜂鸣器
	Openio_Init();  //初始化电磁锁
	//初始化自动消毒模块
	MYEXTI_Init();
	//初始化RC522模块
	RFID_Init();
	Read_Card();
	//初始化AS608指纹模块
  USART1_init(usart1_baund);
	PS_StaGPIO_Init();
	//用于检测人体存在的定时器计数器
	TIM3_Int_Init(60000-1,7200-1);
	while(PS_HandShake(&AS608Addr))
	{
		Show_Chinese_Tips("连接中");
			
	}
	Show_Chinese_Tips("连接成功！");
	
	//初始化定时器2，检查IC卡和指纹
	TIM2_Init(7200-1,20000-1);
	
	
	while(1)
	{
		GUI_begin();
	}
	
	
	
	
}