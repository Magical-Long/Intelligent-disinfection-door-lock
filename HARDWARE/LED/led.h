#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LEDSucces PCout(13)// PC13    Ϊ�̵�
#define LEDError PCout(14)// PC14    Ϊ���

void LED_Init(void);//��ʼ��
void Sanitize_LED_Init(void);  //�����Ƴ�ʼ��
void EXSanitize(void);//�ⲿ��������
void INSanitize(void);


#endif
