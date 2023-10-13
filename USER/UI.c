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

char password[6] = {'0', '0', '0', '0', '0', '0'}; // 默认密码是6个0
char developer_password[8] = {'A', '1', '8', 'A', '2', 'B', 'C', 'A'};

uint8_t cardnumber, tempcard;
extern uint8_t UID[4], Temp[4];
extern uint8_t UI0[4]; // 卡片0ID数组
extern uint8_t UI1[4]; // 卡片1ID数组
extern uint8_t UI2[4]; // 卡片2ID数组
extern uint8_t UI3[4]; // 卡片3ID数组

// 消毒标志位
extern bool exsanitize_flag;
extern bool insanitize_flag;
// 室内有人要出标志位
extern bool inOpen_flag;
// 检查标志位
extern bool Check_flag;
// 开门种类标志位
bool Type_flag = 0;
// 开门标志位
bool Open_flag = 0;
// 指纹成员
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
	LCD_ShowChinese(0, 0, "菜单：", head_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "1.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "设置密码", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "设置指纹", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 48, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 48, "设置", body_col, ground_col, 16, 0);
	LCD_ShowString(45, 48, "IC", body_col, ground_col, 16, 0);
	LCD_ShowChinese(75, 48, "卡", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 64, "4.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 64, "设置开发者密码", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 80, "5.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 80, "返回", body_col, ground_col, 16, 0);
	//	LCD_ShowChinese(20,0,"请输入密码",body_col,ground_col,16,0);
	//	LCD_ShowChar(20,80,':',body_col,ground_col,16,0);
}

/******************************************************************************
	  函数说明：在屏幕中间显示汉字提示
	  返回值：  无
******************************************************************************/
void Show_Chinese_Tips(u8 *str)
{
	ClearLcd();
	LCD_ShowChinese(0, 0, str, body_col, ground_col, 16, 0);
}

// 设置密码
//'*'为清除一位  '#'为确认键
// 返回1说明设置成功 返回0说明设置失败，返回菜单
bool Set_password(void)
{
	int i = 0;
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '#' && i >= 0)
	{
		if (i >= 6) // 已经设置了6位密码。在输入就不能显示了。只能清除或者确认
		{
			key_val = Key_Scan();
			continue; // 跳出此层循环
		}
		if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
		{
			LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
			password[i] = key_val;
			i++;
		}
		else if (key_val == '*')
		{

			LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // 试出来范围
			i--;
		}
		key_val = Key_Scan();
	}
	if (i == -1)
		return false;
	Show_Chinese_Tips("设置成功！");
	LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
	while (Key_Scan() != '#')
		;
	return true;
}

int Comp_password(void)
{
	int status = 0; // 1:密码正确  0:密码错误  -1:进入菜单
	while (1)
	{
		int i = 0;
		u8 key_val = '\0';
		char myword[6];
		key_val = Key_Scan();
		while (key_val != '#' && i >= 0)
		{

			// 消毒部分
			if (exsanitize_flag) // 外部
			{
				EXSanitize();
				exsanitize_flag = 0;
			}

			if (insanitize_flag) // 内部
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
			// IC卡识别和指纹识别
			if (Open_flag||(inOpen_flag==1))
			{
				Open_flag = 0;
				inOpen_flag = 0;
				if (Type_flag)
				{
					Show_Chinese_Tips("比对成功！");
					LCD_ShowChinese16x16(0, 16, "对应", body_col, ground_col, 16, 0);
					LCD_ShowString(33, 16, "ID:", body_col, ground_col, 16, 0);
					LCD_ShowIntNum(66, 16, seach.pageID, 3, body_col, ground_col, 16);
				}
				delay_ms(500);
				return 1;
			}

			if (i >= 6) // 已经输入了6位密码。在输入就不能显示了。只能清除或者确认
			{
				key_val = Key_Scan();
				continue; // 跳出此层循环
			}
			if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
			{
				LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
				myword[i] = key_val;
				i++;
			}
			else if (key_val == '*') // 清除1位
			{
				LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // 试出来范围
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
			Show_Chinese_Tips("密码正确！");
			status = 1;
		}
		else
		{
			Show_Chinese_Tips("密码错误！");
			status = 0;
		}
		LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
		while (Key_Scan() != '#')
			;
		return status;
	}
}

void Open_door(void)
{
	Succes_Tips();
	Show_Chinese_Tips("开门成功！");
	LCD_ShowString(0, 16, "5s", body_col, ground_col, 16, 0);
	LCD_ShowChinese(32, 16, "后关门！", body_col, ground_col, 16, 0);
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
	LCD_ShowChinese(0, 0, "欢迎使用智能门锁！", head_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 62, "请输入密码", body_col, ground_col, 16, 0);
	LCD_ShowChar(80, 62, ':', body_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 16, "按", head_col, ground_col, 16, 0);
	LCD_ShowChar(15, 16, '*', head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 16, "键进入菜单", head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 32, "或清除密码", head_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 48, "按", head_col, ground_col, 16, 0);
	LCD_ShowChar(15, 48, '#', head_col, ground_col, 16, 0);
	LCD_ShowChinese(30, 48, "键确认", head_col, ground_col, 16, 0);
}

void GUI_begin(void)
{
	bool Menu_flag = 0; // 菜单标志位
	int Whole_flag = 0; // 全局标志位，默认为密码错误，即不开门

	delay_ms(500);
	TIM_Cmd(TIM2, ENABLE); // 开启定时器2
	TIM_Cmd(TIM3, ENABLE); // 开启定时器3
	Main_UI();
	Whole_flag = Comp_password();
	while (Whole_flag != 0)
	{
		if (Whole_flag == -1) // 进入菜单
		{
			if (Check_developer())
			{
				while (!Menu_flag) // 阻塞菜单,标志位为0，则重新返回菜单界面
				{
					ShowMenu();
					Menu_flag = Select_Menu();
				}
			}
		}
		else // 密码正确
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
		Show_Chinese_Tips("请设置密码：");
		return Set_password(); // 如果设置密码过程中返回，则返回0，否则设置成功返回1
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
		Show_Chinese_Tips("请设置开发者密码：");
		return Set_developer_password(); // 如果设置密码过程中返回，则返回0，否则设置成功返回1
	}
	else
		return 1; // 直接选择4返回，则返回主界面
}

bool IC_GUI(void) // 返回0时说明要重新回到菜单界面
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
	LCD_ShowChinese(15, 0, "写卡", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "删卡", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "返回", body_col, ground_col, 16, 0);
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
		if (Add_Card() == 0) // 如果添加失败
		{
			return 1; // 继续循环选择IC卡界面
		}
		else
			return -1;
	}
	else if (key_val == '2')
	{
		if (Delete_Card() == 0) // 如果删除失败
		{
			return 1; // 继续循环选择IC卡界面
		}
		else
			return -1; // 删除成功
	}
	else if (key_val == '3') // 选择返回
	{
		return 0;
	}
}

bool RFID_Check()
{
	cardnumber = Rc522Test();								   // 获取卡编号
	if (cardnumber == 0 || cardnumber == 5 || cardnumber == 6) // 如果为0，表示“卡片错误”，系统中没有这张卡
	{

		WaitCardOff(); // 等待卡片移开
		return 0;
	}
	else if (cardnumber == 1 || cardnumber == 2 || cardnumber == 3 || cardnumber == 4) // 如果卡编号为1-4，说明是系统中的4张卡
	{

		Show_Chinese_Tips("卡片正确！");

		WaitCardOff(); // 等待卡片移开
		return 1;
	}
}

// 从flash中读取各卡信息
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

bool Add_Card(void) // 返回是1时说明写入成功，返回0说明返回选择界面
{
	u8 key_val = '\0';
	Show_Chinese_Tips("请放卡片！");
	key_val = Key_Scan();
	while (key_val != '#' && key_val != '*') // 写卡模式
	{
		if (PcdRequest(REQ_ALL, Temp) == MI_OK)
		{
			if (PcdAnticoll(UID) == MI_OK)
			{
				if (UI0[0] == 0xFF && UI0[1] == 0xFF && UI0[2] == 0xFF && UI0[3] == 0xFF)
					tempcard = 0; // 判断系统各卡数据区是否为空，为空才能写入新卡
				else if (UI1[0] == 0xFF && UI1[1] == 0xFF && UI1[2] == 0xFF && UI1[3] == 0xFF)
					tempcard = 1;
				else if (UI2[0] == 0xFF && UI2[1] == 0xFF && UI2[2] == 0xFF && UI2[3] == 0xFF)
					tempcard = 2;
				else if (UI3[0] == 0xFF && UI3[1] == 0xFF && UI3[2] == 0xFF && UI3[3] == 0xFF)
					tempcard = 3;
				else
					tempcard = 4;

				if (UID[0] == UI0[0] && UID[1] == UI0[1] && UID[2] == UI0[2] && UID[3] == UI0[3]) // 判断新卡是否已经录入
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
					UI0[0] = UID[0]; // 将新卡数据写入UI0[]数组
					UI0[1] = UID[1];
					UI0[2] = UID[2];
					UI0[3] = UID[3];
					FLASH_W(FLASH_ADDR1, UI0[0], UI0[1], UI0[2], UI0[3]); // 将新卡数据存入flash
					Show_Chinese_Tips("卡片一写入成功！");
					Succes_Tips();
					WaitCardOff(); // 等待卡片移走
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
				}
				break;
				case 1:
				{
					UI1[0] = UID[0];
					UI1[1] = UID[1];
					UI1[2] = UID[2];
					UI1[3] = UID[3];
					FLASH_W(FLASH_ADDR2, UI1[0], UI1[1], UI1[2], UI1[3]);
					Show_Chinese_Tips("卡片二写入成功！");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
				}
				break;
				case 2:
				{
					UI2[0] = UID[0];
					UI2[1] = UID[1];
					UI2[2] = UID[2];
					UI2[3] = UID[3];
					FLASH_W(FLASH_ADDR3, UI2[0], UI2[1], UI2[2], UI2[3]);
					Show_Chinese_Tips("卡片三写入成功！");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
				}
				break;
				case 3:
				{
					UI3[0] = UID[0];
					UI3[1] = UID[1];
					UI3[2] = UID[2];
					UI3[3] = UID[3];
					FLASH_W(FLASH_ADDR4, UI3[0], UI3[1], UI3[2], UI3[3]);
					Show_Chinese_Tips("卡片四写入成功！");
					Succes_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
				}
				break;
				case 4:
				{
					Show_Chinese_Tips("卡片写入失败！");
					Error_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
				}
				break;
				case 5:
				{
					Show_Chinese_Tips("此卡片已经写入！");
					Error_Tips();
					WaitCardOff();
					LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
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
		return 1; // 确认后退出返回为1，返回选择设置IC卡菜单返回0
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

bool Delete_Card(void) // 返回是1时说明删除成功，返回0说明返回选择界面
{
	u8 key_val1, key_val2;
	bool flag = 0;
	ClearLcd();
	LCD_ShowChinese(0, 0, "请输入删除的卡片号", body_col, ground_col, 16, 0);
	LCD_ShowChinese(0, 32, "按", body_col, ground_col, 16, 0);
	LCD_ShowChar(17, 32, 'A', body_col, ground_col, 16, 0);
	LCD_ShowChinese(34, 32, "键删除全部指纹", body_col, ground_col, 16, 0);
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
			Show_Chinese_Tips("确定要删除卡片一吗？");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
			{
				key_val2 = Key_Scan();
			}
			if (key_val2 == '#') // 在删卡模式下按下3键，删除对应的卡片
			{
				FLASH_Clear(FLASH_ADDR1);
				UI0[0] = 0xFF;
				UI0[1] = 0xFF;
				UI0[2] = 0xFF;
				UI0[3] = 0xFF;
				Show_Chinese_Tips("删除成功！");
				Succes_Tips();
				delay_ms(1500);
				flag = 1; // 退出界面
			}
			else // 返回删除卡片界面
			{
				flag = 0;
			}
		}
		break;
		case '2':
		{
			Show_Chinese_Tips("确定要删除卡片二吗？");
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
				Show_Chinese_Tips("删除成功！");
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
			Show_Chinese_Tips("确定要删除卡片三吗？");
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
				Show_Chinese_Tips("删除成功！");
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
			Show_Chinese_Tips("确定要删除卡片四吗？");
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
				Show_Chinese_Tips("删除成功！");
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
			Show_Chinese_Tips("确定要删除全部卡片吗？");
			key_val2 = Key_Scan();
			while (key_val2 != '#' && key_val2 != '*')
			{
				key_val2 = Key_Scan();
			}
			if (key_val2 == '#') // 在删卡模式下按下3键，删除对应的卡片
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

				Show_Chinese_Tips("删除成功！");
				Succes_Tips();
				delay_ms(1500);
				flag = 1; // 退出界面
			}
		}
		break;
		default:
			break;
		}
	}
	if (flag == 0)
		return 0; // 返回界面
	else
		return 1;
}

// 密码只有n位，所以只需要检测n次即可
bool Mystr_cmp(char *str1, char *str2, int num)
{
	int i = 0;
	for (i = 0; i < num; i++)
	{
		if (str1[i] != str2[i])
			return false; // 只要前六个字符串有一个不相等就直接返回0
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
		return 0; // 返回0说明返回
	else
		return 1;
}

void Finger_UI(void)
{
	ClearLcd();
	LCD_ShowString(0, 0, "1.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 0, "录入指纹", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 16, "2.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 16, "删除指纹", body_col, ground_col, 16, 0);
	LCD_ShowString(0, 32, "3.", body_col, ground_col, 16, 0);
	LCD_ShowChinese(15, 32, "返回", body_col, ground_col, 16, 0);
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
	else if (key_val == '3') // 选择返回
	{
		return 0;
	}
}

int Delete_FR(void)
{
	u8 key_v = '\0';
	u8 i = 0;
	u16 ID = 0;
	Show_Chinese_Tips("输入删除的指纹编号：");
	LCD_ShowChinese(0, 16, "按", body_col, ground_col, 16, 0);
	LCD_ShowChar(17, 16, 'A', body_col, ground_col, 16, 0);
	LCD_ShowChinese(34, 16, "键删除全部指纹", body_col, ground_col, 16, 0);

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
		Show_Chinese_Tips("删除成功！");
		return 1;
	}
	else
	{
		Show_Chinese_Tips("编号无效！");
		Error_Tips();
		delay_ms(500);
		return -1;
	}
}

void Openio_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能PC端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  // 根据设定参数初始化GPIOC13
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

bool Check_developer(void)
{
	/*增加开发者核实身份*/
	int i = 0;
	u8 key_val;
	char myword[8];
	ClearLcd();
	LCD_ShowChinese(0, 0, "请输入开发者密码", body_col, ground_col, 16, 0);
	while (1)
	{
		key_val = Key_Scan();
		while (key_val != '#' && i >= 0)
		{
			if (i >= 8) // 已经设置了8位密码。在输入就不能显示了。只能清除或者确认
			{
				key_val = Key_Scan();
				continue; // 跳出此层循环
			}
			if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
			{
				LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
				myword[i] = key_val;
				i++;
			}
			else if (key_val == '*')
			{
				LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // 试出来范围
				i--;
			}
			key_val = Key_Scan();
		}
		if (i == -1)
			return 0;
		else if (Mystr_cmp(developer_password, myword, 8))
		{
			Show_Chinese_Tips("输入正确！");
			delay_ms(500);
			return 1;
		}
		else
		{
			Show_Chinese_Tips("密码错误！");
			LCD_ShowChinese(0, 17, "请重新输入密码！", body_col, ground_col, 16, 0);
			ClearLcd();
			LCD_ShowChinese(0, 0, "请输入开发者密码", body_col, ground_col, 16, 0);
			i = 0;
		}
	}
}

// 返回1说明设置成功 返回0说明设置失败，返回菜单
bool Set_developer_password(void)
{
	int i = 0;
	u8 key_val;
	key_val = Key_Scan();
	while (key_val != '#' && i >= 0)
	{
		if (i >= 8) // 已经设置了8位密码。在输入就不能显示了。只能清除或者确认
		{
			key_val = Key_Scan();
			continue; // 跳出此层循环
		}
		if (((key_val >= '0') && (key_val <= '9')) || (key_val >= 'A' && key_val <= 'D'))
		{
			LCD_ShowChar(0 + 9 * i, 80, key_val, RED, BLACK, 16, 0);
			developer_password[i] = key_val;
			i++;
		}
		else if (key_val == '*')
		{

			LCD_Fill(9 * i - 9, 80, 9 * i + 9, 96, WHITE); // 试出来范围
			i--;
		}
		key_val = Key_Scan();
	}
	if (i == -1)
		return false;
	Show_Chinese_Tips("设置成功！");
	LCD_ShowChinese(0, 16, "按确认键返回！", body_col, ground_col, 16, 0);
	while (Key_Scan() != '#')
	{
	}

	return true;
}