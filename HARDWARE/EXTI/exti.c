#include "led.h"
#include "delay.h"
#include "exti.h"
#include "sys.h"
#include <stdbool.h>

bool exsanitize_flag = 0; // �ⲿ�����ı�־λ  1������   0��������
bool insanitize_flag = 0; // �ڲ������ı�־λ
bool intimcmd_flag = 0;
//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEKս��STM32������
// �ⲿ�ж� ��������
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// �޸�����:2012/9/3
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////
// �ⲿ�ж�11�������
void MYEXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // ʹ��PinA�͸���ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ���ó��������룬Ĭ�ϵ͵�ƽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  //

	// GPIOA.11 15�ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11 | GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); // ����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // ��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

// �ⲿ�жϷ������
void EXTI15_10_IRQHandler(void)
{
	uint16_t reloadValue = 59999;
	static uint8_t intimes = 0;  //Ϊ0˵����һ�����ˣ�Ϊ1˵��û�ˣ�����,Ϊ2˵����Ҫ��ȥ������
	static bool extimes = 0;	 // ���濪ʼ���ˣ������أ�Ϊ0������Ӧ��Ϊ1��Ӧ����
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
		EXTI_ClearITPendingBit(EXTI_Line11); // ���LINE11�ϵ��жϱ�־λ
	}
	else if (EXTI_GetFlagStatus(EXTI_Line15) != RESET )
	{
		if(intimes==0)
		{
			intimes = 1;
		}
		else if(intimes==1)
		{
			// ���㶨ʱ��������
			TIM_SetCounter(TIM3, 0);
			// �����жϴ�����ֵΪ��װ��ֵ��ȥ1
			
			TIM_SetAutoreload(TIM3,reloadValue);
			// ʹ�ܶ�ʱ���ж�
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			
			// ������ʱ��
			TIM_Cmd(TIM3, ENABLE);
			intimcmd_flag=1;
			intimes = 2;
		}
		else if(intimes==2)
		{
			insanitize_flag = 1;
			// ���㶨ʱ��������
			TIM_SetCounter(TIM3, 0);
			// �����жϴ�����ֵΪ��װ��ֵ��ȥ1
			
			TIM_SetAutoreload(TIM3,reloadValue);
			// ʹ�ܶ�ʱ���ж�
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			
			// ������ʱ��
			TIM_Cmd(TIM3, ENABLE);
			intimes = 1;
		}

		EXTI_ClearITPendingBit(EXTI_Line15); // ���LINE15�ϵ��жϱ�־λ
	}

}
