#ifndef   AS608USER_H_
#define   AS608USER_H_

#include "delay.h"
#include <stdbool.h>

void Add_FR(void);	//¼ָ��
void Del_FR(u16 FR_ID);	//ɾ��ָ��
bool press_FR(void);//ˢָ�� ������ֵΪ1˵��Ϊָ����ȷ
void ShowErrMessage(u8 ensure);   //��ʾ������Ϣ


  

#endif