#ifndef _UI_H
#define _UI_H

#include "sys.h"
#include <string.h>
#include <stdbool.h>

//�궨������ֵ���ɫ�ͱ�����ɫ
#define body_col  BLACK   
#define head_col  RED
#define ground_col WHITE

#define MAX_PASSWORD 6
////����Ĭ��������0
//#define DEFAULT_PASSWORD "000000"

extern char password[6];  


void UI_Init(void);
void ShowMenu(void);    //�˵�
void ClearLcd(void);     //����
void Show_Chinese_Tips(u8*str);  //����Ļ�м���ʾ������ʾ
void Input_password(void);
void Open_door(void);   //���ų���
int  Comp_password(void);  //�Ա�����,����ֵΪ1��˵��������ȷ�����Կ���
void Main_UI(void);    //ϵͳĬ��������
void GUI_begin(void);   //����ϵͳ
bool Select_Menu(void);      //ѡ��˵��е�ѡ��,����ֵΪ��־λ 
bool Set_password(void);     //�������룬Ϊ0˵������ʧ�ܣ����ز˵���  Ϊ1˵�����óɹ����Ż�������
bool IC_GUI(void);
void IC_UI(void);
int  Select_IC(void);        //IC�������ѡ�����ѡ�񷵻���
bool RFID_Check(void);	         //������������������ȡ�����
void Read_Card(void);		//�ϵ���flash�ж�����¼��Ŀ�����
bool Add_Card(void);    //д������������IC��
bool Delete_Card(void);       //ɾ����Ƭ
bool Mystr_cmp(char* str1,char* str2,int num);   //�Լ���ƵıȽ��ַ����Ƿ����
bool Finger_GUI(void);     //ָ�ƽ�����GUI
void Finger_UI(void);      //ָ�ƽ����Ľ���
int  Select_Finger(void);  //ѡ��ָ�Ʋ˵�
void Succes_Tips(void);
void Error_Tips(void);
int  Delete_FR(void);      //ɾ��ָ��
void Openio_Init(void);        //����IO�ڵĳ�ʼ��
bool Check_developer(void);    //��ʵ���������
bool Set_developer_password(void);     //���ÿ���������

#endif