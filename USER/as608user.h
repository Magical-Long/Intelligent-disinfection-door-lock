#ifndef   AS608USER_H_
#define   AS608USER_H_

#include "delay.h"
#include <stdbool.h>

void Add_FR(void);	//录指纹
void Del_FR(u16 FR_ID);	//删除指纹
bool press_FR(void);//刷指纹 ，返回值为1说明为指纹正确
void ShowErrMessage(u8 ensure);   //显示错误信息


  

#endif