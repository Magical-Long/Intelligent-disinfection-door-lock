#ifndef __KEYS_H
#define __KEYS_H	 
//	 

//********************************************************************************
//V1.1修改说明
//修改按键扫描函数，使整个代码可以支持SWD下载。
//	 
#include "stm32f10x.h"

#define KEY_HANG 0x000f   //0123
#define KEY1 GPIO_Pin_4   
#define KEY2 GPIO_Pin_5	   
#define KEY3 GPIO_Pin_6	 
#define KEY4 GPIO_Pin_7   

#define KEY1_Input GPIO_ReadInputDataBit(KEY_PROT,KEY1)
#define KEY2_Input GPIO_ReadInputDataBit(KEY_PROT,KEY2)
#define KEY3_Input GPIO_ReadInputDataBit(KEY_PROT,KEY3)
#define KEY4_Input GPIO_ReadInputDataBit(KEY_PROT,KEY4)

#define KEY_PROT GPIOB

extern u8 FLAG;

void Key_Init(void);//IO初始化
//u8 KEY_Scan(void);
//void KEY_Scan(u8 *key);  //按键扫描函数		
u8 Key_Scan(void);

#endif

