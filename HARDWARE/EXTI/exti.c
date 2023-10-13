#include "led.h"
#include "delay.h"
#include "exti.h"
#include "sys.h"
#include <stdbool.h>

bool exsanitize_flag = 0; // 外部消毒的标志位  1：消毒   0：不消毒
bool insanitize_flag = 0; // 内部消毒的标志位
bool intimcmd_flag = 0;
//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK战舰STM32开发板
// 外部中断 驱动代码
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 修改日期:2012/9/3
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////
// 外部中断11服务程序
void MYEXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // 使能PinA和复用时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 设置成下拉输入，默认低电平
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  //

	// GPIOA.11 15中断线以及中断初始化配置   下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11 | GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); // 根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // 抢占优先级0，
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}

// 外部中断服务程序
void EXTI15_10_IRQHandler(void)
{
	uint16_t reloadValue = 59999;
	static uint8_t intimes = 0;  //为0说明第一次有人，为1说明没人，消毒,为2说明人要出去，开门
	static bool extimes = 0;	 // 外面开始有人，上升沿，为0，不反应，为1反应消毒
	if (EXTI_GetFlagStatus(EXTI_Line11) != RESET)
	{
		if(extimes==0)
		{
			extimes = 1;
		}
		else if(extimes==1)
		{
			exsanitize_flag = 1;
			extimes = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line11); // 清除LINE11上的中断标志位
	}
	else if (EXTI_GetFlagStatus(EXTI_Line15) != RESET )
	{
		if(intimes==0)
		{
			intimes = 1;
		}
		else if(intimes==1)
		{
			// 清零定时器计数器
			TIM_SetCounter(TIM3, 0);
			// 设置中断触发阈值为重装载值减去1
			
			TIM_SetAutoreload(TIM3,reloadValue);
			// 使能定时器中断
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			
			// 启动定时器
			TIM_Cmd(TIM3, ENABLE);
			intimcmd_flag=1;
			intimes = 2;
		}
		else if(intimes==2)
		{
			insanitize_flag = 1;
			// 清零定时器计数器
			TIM_SetCounter(TIM3, 0);
			// 设置中断触发阈值为重装载值减去1
			
			TIM_SetAutoreload(TIM3,reloadValue);
			// 使能定时器中断
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			
			// 启动定时器
			TIM_Cmd(TIM3, ENABLE);
			intimes = 1;
		}

		EXTI_ClearITPendingBit(EXTI_Line15); // 清除LINE15上的中断标志位
	}

}
