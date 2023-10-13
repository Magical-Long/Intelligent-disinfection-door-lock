#ifndef _UI_H
#define _UI_H

#include "sys.h"
#include <string.h>
#include <stdbool.h>

//宏定义各个字的颜色和背景颜色
#define body_col  BLACK   
#define head_col  RED
#define ground_col WHITE

#define MAX_PASSWORD 6
////密码默认是六个0
//#define DEFAULT_PASSWORD "000000"

extern char password[6];  


void UI_Init(void);
void ShowMenu(void);    //菜单
void ClearLcd(void);     //清屏
void Show_Chinese_Tips(u8*str);  //在屏幕中间显示汉字提示
void Input_password(void);
void Open_door(void);   //开门程序
int  Comp_password(void);  //对比密码,返回值为1则说明密码正确，可以开门
void Main_UI(void);    //系统默认主界面
void GUI_begin(void);   //进入系统
bool Select_Menu(void);      //选择菜单中的选项,返回值为标志位 
bool Set_password(void);     //设置密码，为0说明设置失败，返回菜单；  为1说明设置成功，放回主界面
bool IC_GUI(void);
void IC_UI(void);
int  Select_IC(void);        //IC卡界面的选择，如果选择返回则
bool RFID_Check(void);	         //读卡函数，读卡并获取卡编号
void Read_Card(void);		//上电后从flash中读出已录入的卡数据
bool Add_Card(void);    //写卡函数，增加IC卡
bool Delete_Card(void);       //删除卡片
bool Mystr_cmp(char* str1,char* str2,int num);   //自己设计的比较字符串是否相等
bool Finger_GUI(void);     //指纹解锁的GUI
void Finger_UI(void);      //指纹解锁的界面
int  Select_Finger(void);  //选择指纹菜单
void Succes_Tips(void);
void Error_Tips(void);
int  Delete_FR(void);      //删除指纹
void Openio_Init(void);        //开门IO口的初始化
bool Check_developer(void);    //核实开发者身份
bool Set_developer_password(void);     //设置开发者密码

#endif