#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LEDSucces PCout(13)// PC13    为绿灯
#define LEDError PCout(14)// PC14    为红灯

void LED_Init(void);//初始化
void Sanitize_LED_Init(void);  //消毒灯初始化
void EXSanitize(void);//外部消毒功能
void INSanitize(void);


#endif
