#include "delay.h"
#include "led.h"


//初始化PC13 PC14为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;				 //LED0-->PC13  LED1-->PC14端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC13
 GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);						 //PC13 输出高

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

void EXSanitize(void)                        //外部消毒灯
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
