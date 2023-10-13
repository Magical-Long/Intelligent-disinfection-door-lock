#include "delay.h"
#include "led.h"


//��ʼ��PC13 PC14Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;				 //LED0-->PC13  LED1-->PC14�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC13
 GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);						 //PC13 �����

}
 
void Sanitize_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 
  GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6);			
}

void EXSanitize(void)                        //�ⲿ������
{
	LEDError=0;
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	LEDError=1;
}

void INSanitize(void)
{
	LEDError=0;
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	LEDError=1;
}
