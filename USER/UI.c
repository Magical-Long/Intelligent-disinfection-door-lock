#include "UI.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "lcd_init.h"
#include "key.h"
#include "RC522.h"
#include "Flash.h"
#include "beep.h"
#include "as608.h"
#include "as608user.h"

char password[6] = {'0', '0', '0', '0', '0', '0'}; // Ĭ��������6��0
char developer_password[8] = {'A', '1', '8', 'A', '2', 'B', 'C', 'A'};

uint8_t cardnumber, tempcard;
extern uint8_t UID[4], Temp[4];
extern uint8_t UI0[4]; // ��Ƭ0ID����
extern uint8_t UI1[4]; // ��Ƭ1ID����
extern uint8_t UI2[4]; // ��Ƭ2ID����
extern uint8_t UI3[4]; // ��Ƭ3ID����

// ������־λ
extern bool exsanitize_flag;
extern bool insanitize_flag;
// ��������Ҫ����־λ
extern bool inOpen_flag;
// ����־λ
extern bool Check_flag;
// ���������־λ
bool Type_flag = 0;
// ���ű�־λ
bool Open_flag = 0;
// ָ�Ƴ�Ա
extern SearchResult seach;

void UI_Init(void)
{
	LCD_Init();
	//	RFID_Check();
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
}

void ClearLcd(void)
{
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
}

void ShowMenu(void)
{
	/******************/
	ClearLcd();
	LCD_ShowChinese(0, 0, "�˵���", head_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "1.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "��������", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "����ָ��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 48, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 48, "����", body_col, ground_col, 16, 0);
	LCD_ShowString(45, 48, "IC", body_col, ground_col, 16, 0);
	LCD_ShowChinese(75, 48, "��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 64, "4.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 64, "���ÿ���������", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 80, "5.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 80, "����", body_col, ground_col, 16, 0);
	//	LCD_ShowChinese(20,0,"����������",body_col,ground_col,16,0);
	//	LCD_ShowChar(20,80,':',body_col,ground_col,16,0);
}

/******************************************************************************
	  ����˵��������Ļ�м���ʾ������ʾ
	  ����ֵ��  ��
******************************************************************************/
void Show_Chinese_Tips(u8 *str)
{
	ClearLcd();
	LCD_ShowChinese(0, 0, str, body_col, ground_col, 16, 0);
}

// ��������
//'*'Ϊ���һλ  '#'Ϊȷ�ϼ�
// ����1˵�����óɹ� ����0˵������ʧ�ܣ����ز˵�
bool Set_password(void)
{
	int i = 0;
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '#' && i >= 0)
	{
		if (i >= 6) // �Ѿ�������6λ���롣������Ͳ�����ʾ�ˡ�ֻ���������ȷ��
		{
			key_val = Key_Scan();
			continue; // �����˲�ѭ��
		}
		if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
		{
			LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
			password[i] = key_val;
			i++;
		}
		else if (key_val == '*')
		{

			LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // �Գ�����Χ
			i--;
		}
		key_val = Key_Scan();
	}
	if (i == -1)
		return false;
	Show_Chinese_Tips("���óɹ���");
	LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
	while (Key_Scan() != '#')
		;
	return true;
}

int Comp_password(void)
{
	int status = 0; // 1:������ȷ  0:�������  -1:����˵�
	while (1)
	{
		int i = 0;
		u8 key_val = '\0';
		char myword[6];
		key_val = Key_Scan();
		while (key_val != '#' && i >= 0)
		{

			// ��������
			if (exsanitize_flag) // �ⲿ
			{
				EXSanitize();
				exsanitize_flag = 0;
			}

			if (insanitize_flag) // �ڲ�
			{
				INSanitize();
				insanitize_flag = 0;
			}

			if (Check_flag)
			{
				if (press_FR())
				{
					Type_flag = 1;
					Open_flag = 1;
				}
				if (RFID_Check())
				{
					Open_flag = 1;
					Type_flag = 0;
				}
				Check_flag = 0;
			}
			// IC��ʶ���ָ��ʶ��
			if (Open_flag||(inOpen_flag==1))
			{
				Open_flag = 0;
				inOpen_flag = 0;
				if (Type_flag)
				{
					Show_Chinese_Tips("�ȶԳɹ���");
					LCD_ShowChinese16x16(0, 16, "��Ӧ", body_col, ground_col, 16, 0);
					LCD_ShowString(33, 16, "ID:", body_col, ground_col, 16, 0);
					LCD_ShowIntNum(66, 16, seach.pageID, 3, body_col, ground_col, 16);
				}
				delay_ms(500);
				return 1;
			}

			if (i >= 6) // �Ѿ�������6λ���롣������Ͳ�����ʾ�ˡ�ֻ���������ȷ��
			{
				key_val = Key_Scan();
				continue; // �����˲�ѭ��
			}
			if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
			{
				LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
				myword[i] = key_val;
				i++;
			}
			else if (key_val == '*') // ���1λ
			{
				LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // �Գ�����Χ
				i--;
			}
			key_val = Key_Scan();
		}
		if (i < 0)
		{
			status = -1;
			return status;
		}
		else if (Mystr_cmp(password, myword, 6))
		{
			Show_Chinese_Tips("������ȷ��");
			status = 1;
		}
		else
		{
			Show_Chinese_Tips("�������");
			status = 0;
		}
		LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
		while (Key_Scan() != '#')
			;
		return status;
	}
}

void Open_door(void)
{
	Succes_Tips();
	Show_Chinese_Tips("���ųɹ���");
	LCD_ShowString(0, 16, "5s", body_col, ground_col, 16, 0);
	LCD_ShowChinese(32, 16, "����ţ�", body_col, ground_col, 16, 0);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

void Main_UI(void)
{
	ClearLcd();
	LCD_ShowChinese(0, 0, "��ӭʹ������������", head_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 62, "����������", body_col, ground_col, 16, 0);
	LCD_ShowChar(80, 62, ':', body_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 16, "��", head_col, ground_col, 16, 0);
	LCD_ShowChar(15, 16, '*', head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 16, "������˵�", head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 32, "���������", head_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 48, "��", head_col, ground_col, 16, 0);
	LCD_ShowChar(15, 48, '#', head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 48, "��ȷ��", head_col, ground_col, 16, 0);
}

void GUI_begin(void)
{
	bool Menu_flag = 0; // �˵���־λ
	int Whole_flag = 0; // ȫ�ֱ�־λ��Ĭ��Ϊ������󣬼�������

	delay_ms(500);
	TIM_Cmd(TIM2, ENABLE); // ������ʱ��2
	TIM_Cmd(TIM3, ENABLE); // ������ʱ��3
	Main_UI();
	Whole_flag = Comp_password();
	while (Whole_flag != 0)
	{
		if (Whole_flag == -1) // ����˵�
		{
			if (Check_developer())
			{
				while (!Menu_flag) // �����˵�,��־λΪ0�������·��ز˵�����
				{
					ShowMenu();
					Menu_flag = Select_Menu();
				}
			}
		}
		else // ������ȷ
		{
			Open_door();
		}
		Whole_flag = 0;
	}
}

bool Select_Menu(void)
{
	u8 key_val = '\0';
	key_val = Key_Scan();
	while (key_val != '1' && key_val != '2' && key_val != '3' && key_val != '4' && key_val != '5')
	{
		key_val = Key_Scan();
	}
	if (key_val == '1')
	{
		Show_Chinese_Tips("���������룺");
		return Set_password(); // ���������������з��أ��򷵻�0���������óɹ�����1
	}
	else if (key_val == '2')
	{
		return Finger_GUI();
	}
	else if (key_val == '3')
	{
		return IC_GUI();
	}
	if (key_val == '4')
	{
		Show_Chinese_Tips("�����ÿ��������룺");
		return Set_developer_password(); // ���������������з��أ��򷵻�0���������óɹ�����1
	}
	else
		return 1; // ֱ��ѡ��4���أ��򷵻�������
}

bool IC_GUI(void) // ����0ʱ˵��Ҫ���»ص��˵�����
{
	int flag = 1;
	while (flag == 1)
	{
		IC_UI();
		flag = Select_IC();
	}
	return (-flag);
}

void IC_UI(void)
{
	ClearLcd();
	LCD_ShowString(0, 0, "1.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 0, "д��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "ɾ��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "����", body_col, ground_col, 16, 0);
}

int Select_IC(void)
{
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '1' && key_val != '2' && key_val != '3')
	{
		key_val = Key_Scan();
	}
	if (key_val == '1')
	{
		if (Add_Card() == 0) // ������ʧ��
		{
			return 1; // ����ѭ��ѡ��IC������
		}
		else
			return -1;
	}
	else if (key_val == '2')
	{
		if (Delete_Card() == 0) // ���ɾ��ʧ��
		{
			return 1; // ����ѭ��ѡ��IC������
		}
		else
			return -1; // ɾ���ɹ�
	}
	else if (key_val == '3') // ѡ�񷵻�
	{
		return 0;
	}
}

bool RFID_Check()
{
	cardnumber = Rc522Test();								   // ��ȡ�����
	if (cardnumber == 0 || cardnumber == 5 || cardnumber == 6) // ���Ϊ0����ʾ����Ƭ���󡱣�ϵͳ��û�����ſ�
	{

		WaitCardOff(); // �ȴ���Ƭ�ƿ�
		return 0;
	}
	else if (cardnumber == 1 || cardnumber == 2 || cardnumber == 3 || cardnumber == 4) // ��������Ϊ1-4��˵����ϵͳ�е�4�ſ�
	{

		Show_Chinese_Tips("��Ƭ��ȷ��");

		WaitCardOff(); // �ȴ���Ƭ�ƿ�
		return 1;
	}
}

// ��flash�ж�ȡ������Ϣ
void Read_Card()
{
	UI0[0] = FLASH_R(FLASH_ADDR1);
	UI0[1] = FLASH_R(FLASH_ADDR1 + 2);
	UI0[2] = FLASH_R(FLASH_ADDR1 + 4);
	UI0[3] = FLASH_R(FLASH_ADDR1 + 6);

	UI1[0] = FLASH_R(FLASH_ADDR2);
	UI1[1] = FLASH_R(FLASH_ADDR2 + 2);
	UI1[2] = FLASH_R(FLASH_ADDR2 + 4);
	UI1[3] = FLASH_R(FLASH_ADDR2 + 6);

	UI2[0] = FLASH_R(FLASH_ADDR3);
	UI2[1] = FLASH_R(FLASH_ADDR3 + 2);
	UI2[2] = FLASH_R(FLASH_ADDR3 + 4);
	UI2[3] = FLASH_R(FLASH_ADDR3 + 6);

	UI3[0] = FLASH_R(FLASH_ADDR4);
	UI3[1] = FLASH_R(FLASH_ADDR4 + 2);
	UI3[2] = FLASH_R(FLASH_ADDR4 + 4);
	UI3[3] = FLASH_R(FLASH_ADDR4 + 6);
}

bool Add_Card(void) // ������1ʱ˵��д��ɹ�������0˵������ѡ�����
{
	u8 key_val = '\0';
	Show_Chinese_Tips("��ſ�Ƭ��");
	key_val = Key_Scan();
	while (key_val != '#' && key_val != '*') // д��ģʽ
	{
		if (PcdRequest(REQ_ALL, Temp) == MI_OK)
		{
			if (PcdAnticoll(UID) == MI_OK)
			{
				if (UI0[0] == 0xFF && UI0[1] == 0xFF && UI0[2] == 0xFF && UI0[3] == 0xFF)
					tempcard = 0; // �ж�ϵͳ�����������Ƿ�Ϊ�գ�Ϊ�ղ���д���¿�
				else if (UI1[0] == 0xFF && UI1[1] == 0xFF && UI1[2] == 0xFF && UI1[3] == 0xFF)
					tempcard = 1;
				else if (UI2[0] == 0xFF && UI2[1] == 0xFF && UI2[2] == 0xFF && UI2[3] == 0xFF)
					tempcard = 2;
				else if (UI3[0] == 0xFF && UI3[1] == 0xFF && UI3[2] == 0xFF && UI3[3] == 0xFF)
					tempcard = 3;
				else
					tempcard = 4;

				if (UID[0] == UI0[0] && UID[1] == UI0[1] && UID[2] == UI0[2] && UID[3] == UI0[3]) // �ж��¿��Ƿ��Ѿ�¼��
				{
					tempcard = 5;
				}
				if (UID[0] == UI1[0] && UID[1] == UI1[1] && UID[2] == UI1[2] && UID[3] == UI1[3])
				{
					tempcard = 5;
				}
				if (UID[0] == UI2[0] && UID[1] == UI2[1] && UID[2] == UI2[2] && UID[3] == UI2[3])
				{
					tempcard = 5;
				}
				if (UID[0] == UI3[0] && UID[1] == UI3[1] && UID[2] == UI3[2] && UID[3] == UI3[3])
				{
					tempcard = 5;
				}

				switch (tempcard)
				{
				case 0:
				{
					UI0[0] = UID[0]; // ���¿�����д��UI0[]����
					UI0[1] = UID[1];
					UI0[2] = UID[2];
					UI0[3] = UID[3];
					FLASH_W(FLASH_ADDR1, UI0[0], UI0[1], UI0[2], UI0[3]); // ���¿����ݴ���flash
					Show_Chinese_Tips("��Ƭһд��ɹ���");
					Succes_Tips();
					WaitCardOff(); // �ȴ���Ƭ����
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				case 1:
				{
					UI1[0] = UID[0];
					UI1[1] = UID[1];
					UI1[2] = UID[2];
					UI1[3] = UID[3];
					FLASH_W(FLASH_ADDR2, UI1[0], UI1[1], UI1[2], UI1[3]);
					Show_Chinese_Tips("��Ƭ��д��ɹ���");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				case 2:
				{
					UI2[0] = UID[0];
					UI2[1] = UID[1];
					UI2[2] = UID[2];
					UI2[3] = UID[3];
					FLASH_W(FLASH_ADDR3, UI2[0], UI2[1], UI2[2], UI2[3]);
					Show_Chinese_Tips("��Ƭ��д��ɹ���");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				case 3:
				{
					UI3[0] = UID[0];
					UI3[1] = UID[1];
					UI3[2] = UID[2];
					UI3[3] = UID[3];
					FLASH_W(FLASH_ADDR4, UI3[0], UI3[1], UI3[2], UI3[3]);
					Show_Chinese_Tips("��Ƭ��д��ɹ���");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				case 4:
				{
					Show_Chinese_Tips("��Ƭд��ʧ�ܣ�");
					Error_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				case 5:
				{
					Show_Chinese_Tips("�˿�Ƭ�Ѿ�д�룡");
					Error_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
				}
				break;
				default:
					break;
				}
			}
		}
		key_val = Key_Scan();
	}
	if (key_val == '#')
		return 1; // ȷ�Ϻ��˳�����Ϊ1������ѡ������IC���˵�����0
	else
		return 0;
}

void Succes_Tips(void)
{
	BEEP = 1;
	LEDSucces = 0;
	delay_ms(200);
	BEEP = 0;
	delay_ms(110);
	BEEP = 1;
	delay_ms(200);
	BEEP = 0;

	LEDSucces = 1;
}

void Error_Tips(void)
{
	BEEP = 1;
	LEDError = 0;

	delay_ms(500);
	BEEP = 0;
	LEDError = 1;
}

bool Delete_Card(void) // ������1ʱ˵��ɾ���ɹ�������0˵������ѡ�����
{
	u8 key_val1, key_val2;
	bool flag = 0;
	ClearLcd();
	LCD_ShowChinese(0, 0, "������ɾ���Ŀ�Ƭ��", body_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 32, "��", body_col, ground_col, 16, 0);
	LCD_ShowChar(17, 32, 'A', body_col, ground_col, 16, 0);
	LCD_ShowChinese(34, 32, "��ɾ��ȫ��ָ��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "<1-4>", body_col, ground_col, 16, 0);
	key_val1 = Key_Scan();
	while (key_val1 == '\0')
		key_val1 = Key_Scan();
	while (!flag && key_val1 != '*')
	{
		key_val1 = Key_Scan();
		switch (key_val1)
		{
		case '1':
		{
			Show_Chinese_Tips("ȷ��Ҫɾ����Ƭһ��");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
			{
				key_val2 = Key_Scan();
			}
			if (key_val2 == '#') // ��ɾ��ģʽ�°���3����ɾ����Ӧ�Ŀ�Ƭ
			{
				FLASH_Clear(FLASH_ADDR1);
				UI0[0] = 0xFF;
				UI0[1] = 0xFF;
				UI0[2] = 0xFF;
				UI0[3] = 0xFF;
				Show_Chinese_Tips("ɾ���ɹ���");
				Succes_Tips();
				delay_ms(1500);
				flag = 1; // �˳�����
			}
			else // ����ɾ����Ƭ����
			{
				flag = 0;
			}
		}
		break;
		case '2':
		{
			Show_Chinese_Tips("ȷ��Ҫɾ����Ƭ����");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
				key_val2 = Key_Scan();
			if (key_val2 == '#')
			{
				FLASH_Clear(FLASH_ADDR2);
				UI1[0] = 0xFF;
				UI1[1] = 0xFF;
				UI1[2] = 0xFF;
				UI1[3] = 0xFF;
				Show_Chinese_Tips("ɾ���ɹ���");
				Succes_Tips();
				delay_ms(1500);
				flag = 1;
			}
			else
				flag = 0;
		}
		break;
		case '3':
		{
			Show_Chinese_Tips("ȷ��Ҫɾ����Ƭ����");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
				key_val2 = Key_Scan();
			if (key_val2 == '#')
			{
				FLASH_Clear(FLASH_ADDR3);
				UI2[0] = 0xFF;
				UI2[1] = 0xFF;
				UI2[2] = 0xFF;
				UI2[3] = 0xFF;
				Show_Chinese_Tips("ɾ���ɹ���");
				Succes_Tips();
				delay_ms(1500);
				flag = 1;
			}
			else
				flag = 0;
		}
		break;
		case '4':
		{
			Show_Chinese_Tips("ȷ��Ҫɾ����Ƭ����");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
				key_val2 = Key_Scan();
			if (key_val2 == '#')
			{
				FLASH_Clear(FLASH_ADDR4);
				UI3[0] = 0xFF;
				UI3[1] = 0xFF;
				UI3[2] = 0xFF;
				UI3[3] = 0xFF;
				Show_Chinese_Tips("ɾ���ɹ���");
				Succes_Tips();
				delay_ms(1500);
				flag = 1;
			}
			else
				flag = 0;
		}
		break;
		case 'A':
		{
			Show_Chinese_Tips("ȷ��Ҫɾ��ȫ����Ƭ��");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
			{
				key_val2 = Key_Scan();
			}
			if (key_val2 == '#') // ��ɾ��ģʽ�°���3����ɾ����Ӧ�Ŀ�Ƭ
			{
				FLASH_Clear(FLASH_ADDR1);
				UI0[0] = 0xFF;
				UI0[1] = 0xFF;
				UI0[2] = 0xFF;
				UI0[3] = 0xFF;

				FLASH_Clear(FLASH_ADDR2);
				UI1[0] = 0xFF;
				UI1[1] = 0xFF;
				UI1[2] = 0xFF;
				UI1[3] = 0xFF;

				FLASH_Clear(FLASH_ADDR3);
				UI2[0] = 0xFF;
				UI2[1] = 0xFF;
				UI2[2] = 0xFF;
				UI2[3] = 0xFF;

				FLASH_Clear(FLASH_ADDR4);
				UI3[0] = 0xFF;
				UI3[1] = 0xFF;
				UI3[2] = 0xFF;
				UI3[3] = 0xFF;

				Show_Chinese_Tips("ɾ���ɹ���");
				Succes_Tips();
				delay_ms(1500);
				flag = 1; // �˳�����
			}
		}
		break;
		default:
			break;
		}
	}
	if (flag == 0)
		return 0; // ���ؽ���
	else
		return 1;
}

// ����ֻ��nλ������ֻ��Ҫ���n�μ���
bool Mystr_cmp(char *str1, char *str2, int num)
{
	int i = 0;
	for (i = 0; i < num; i++)
	{
		if (str1[i] != str2[i])
			return false; // ֻҪǰ�����ַ�����һ������Ⱦ�ֱ�ӷ���0
	}
	return true;
}

bool Finger_GUI(void)
{
	int flag = 1;
	while (flag == 1)
	{
		Finger_UI();
		flag = Select_Finger();
	}
	if (flag == 0)
		return 0; // ����0˵������
	else
		return 1;
}

void Finger_UI(void)
{
	ClearLcd();
	LCD_ShowString(0, 0, "1.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 0, "¼��ָ��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "ɾ��ָ��", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "����", body_col, ground_col, 16, 0);
}

int Select_Finger(void)
{
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '1' && key_val != '2' && key_val != '3')
	{
		key_val = Key_Scan();
	}
	if (key_val == '1')
	{
		Add_FR();
		return -1;
	}
	else if (key_val == '2')
	{
		return (-(Delete_FR()));
	}
	else if (key_val == '3') // ѡ�񷵻�
	{
		return 0;
	}
}

int Delete_FR(void)
{
	u8 key_v = '\0';
	u8 i = 0;
	u16 ID = 0;
	Show_Chinese_Tips("����ɾ����ָ�Ʊ�ţ�");
	LCD_ShowChinese(0, 16, "��", body_col, ground_col, 16, 0);
	LCD_ShowChar(17, 16, 'A', body_col, ground_col, 16, 0);
	LCD_ShowChinese(34, 16, "��ɾ��ȫ��ָ��", body_col, ground_col, 16, 0);

	key_v = Key_Scan();

	while (key_v != '#' && ID <= 300)
	{
		if (((key_v >= '0') && (key_v <= '9')) || key_v == 'A')
		{
			ID = ID * 10 + key_v - '0';
			LCD_ShowChar(0 + 9 * i, 80, key_v, RED, BLACK, 16, 0);
			i++;
		}
		else if (key_v == '*')
		{
			ID = ID / 10;
			LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE);
			i--;
		}
		key_v = Key_Scan();
	}
	if (key_v == '#')
	{
		if (ID == 'A' - '0')
		{
			PS_Empty();
		}
		else
			Del_FR(ID);
		Show_Chinese_Tips("ɾ���ɹ���");
		return 1;
	}
	else
	{
		Show_Chinese_Tips("�����Ч��");
		Error_Tips();
		delay_ms(500);
		return -1;
	}
}

void Openio_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // ʹ��PC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  // �����趨������ʼ��GPIOC13
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

bool Check_developer(void)
{
	/*���ӿ����ߺ�ʵ���*/
	int i = 0;
	u8 key_val;
	char myword[8];
	ClearLcd();
	LCD_ShowChinese(0, 0, "�����뿪��������", body_col, ground_col, 16, 0);
	while (1)
	{
		key_val = Key_Scan();
		while (key_val != '#' && i >= 0)
		{
			if (i >= 8) // �Ѿ�������8λ���롣������Ͳ�����ʾ�ˡ�ֻ���������ȷ��
			{
				key_val = Key_Scan();
				continue; // �����˲�ѭ��
			}
			if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
			{
				LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
				myword[i] = key_val;
				i++;
			}
			else if (key_val == '*')
			{
				LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // �Գ�����Χ
				i--;
			}
			key_val = Key_Scan();
		}
		if (i == -1)
			return 0;
		else if (Mystr_cmp(developer_password, myword, 8))
		{
			Show_Chinese_Tips("������ȷ��");
			delay_ms(500);
			return 1;
		}
		else
		{
			Show_Chinese_Tips("�������");
			LCD_ShowChinese(0, 17, "�������������룡", body_col, ground_col, 16, 0);
			ClearLcd();
			LCD_ShowChinese(0, 0, "�����뿪��������", body_col, ground_col, 16, 0);
			i = 0;
		}
	}
}

// ����1˵�����óɹ� ����0˵������ʧ�ܣ����ز˵�
bool Set_developer_password(void)
{
	int i = 0;
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '#' && i >= 0)
	{
		if (i >= 8) // �Ѿ�������8λ���롣������Ͳ�����ʾ�ˡ�ֻ���������ȷ��
		{
			key_val = Key_Scan();
			continue; // �����˲�ѭ��
		}
		if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
		{
			LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
			developer_password[i] = key_val;
			i++;
		}
		else if (key_val == '*')
		{

			LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // �Գ�����Χ
			i--;
		}
		key_val = Key_Scan();
	}
	if (i == -1)
		return false;
	Show_Chinese_Tips("���óɹ���");
	LCD_ShowChinese(0, 16, "��ȷ�ϼ����أ�", body_col, ground_col, 16, 0);
	while (Key_Scan() != '#')
	{
	}

	return true;
}