#include "delay.h"
#include "lcd.h"
#include "lcd_init.h"
#include "UI.h"
#include "key.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "RC522.h"
#include "timer.h"
#include "as608.h"
#include "as608user.h"
#include "exti.h"
#include "beep.h"
#include "Flash.h"
	
#define usart1_baund  57600

int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LED_Init();    //Ĭ�ϳ�ʼ�׶�����LED����ʾLED������
	Sanitize_LED_Init();
	UI_Init();     //��ʼ������
	Key_Init();    //������̳�ʼ��
	BEEP_Init();   //��ʼ��������
	Openio_Init();  //��ʼ�������
	//��ʼ���Զ�����ģ��
	MYEXTI_Init();
	//��ʼ��RC522ģ��
	RFID_Init();
	Read_Card();
	//��ʼ��AS608ָ��ģ��
  USART1_init(usart1_baund);
	PS_StaGPIO_Init();
	//���ڼ��������ڵĶ�ʱ��������
	TIM3_Int_Init(60000-1,7200-1);
	while(PS_HandShake(&AS608Addr))
	{
		Show_Chinese_Tips("������");
			
	}
	Show_Chinese_Tips("���ӳɹ���");
	
	//��ʼ����ʱ��2�����IC����ָ��
	TIM2_Init(7200-1,20000-1);
	
	
	while(1)
	{
		GUI_begin();
	}
	
	
	
	
}