#ifndef __KEYS_H
#define __KEYS_H	 
//	 

//********************************************************************************
//V1.1�޸�˵��
//�޸İ���ɨ�躯����ʹ�����������֧��SWD���ء�
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

void Key_Init(void);//IO��ʼ��
//u8 KEY_Scan(void);
//void KEY_Scan(u8 *key);  //����ɨ�躯��		
u8 Key_Scan(void);

#endif

