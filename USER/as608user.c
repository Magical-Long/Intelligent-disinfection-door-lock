#include "as608user.h"
#include "UI.h"
#include "lcd.h"
#include "lcd_init.h"
#include "key.h"
#include "usart.h"
#include "as608.h"

SysPara AS608Para;//ָ��ģ��AS608����
extern vu16 USART1_RX_STA;       //����״̬���	  
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 N;//ģ������Чģ�����
SearchResult seach;



void Add_FR(void )     //���ָ��
{
	u8 times=0;
	u8 i=0,ensure ,processnum=0;
	u8 key_val='\0';
	u16 ID=0;
	
	while(1)
	{
		ClearLcd();
		switch (processnum)
		{
			case 0:
				i++;
				LCD_ShowChinese(0,0,"�밴һ����ָ!",body_col,ground_col,16,0);
				ensure=PS_GetImage();
			  while(ensure!=0x00)
				{
					ensure=PS_GetImage();
				}
				ensure=PS_GenChar(CharBuffer1);//��������
        if(ensure==0x00)
				{		
    			ClearLcd();
		    	Show_Chinese_Tips("����ɹ�!");//ָ����ȷ
				  i=0;
				  processnum=1;//�����ڶ���
				}	
				else
				{
					Show_Chinese_Tips("����ʧ��!");//ָ����ȷ
					processnum=0;
				}									
				delay_ms(1000);		
				break;
			
			case 1:
				i++;
				ClearLcd();
				LCD_ShowChinese(0,0,"���ٰ�һ����ָ!           ",body_col,ground_col,16,0);//�ٰ�һ����ָ
				ensure=PS_GetImage();
			  while(ensure!=0x00)
				{
					ensure=PS_GetImage();
				}
				ensure=PS_GenChar(CharBuffer2);//��������	
        if(ensure==0x00)
				{		
    			ClearLcd();
		    	Show_Chinese_Tips("����ɹ�!");//ָ����ȷ
				  i=0;
				  processnum=2;//����������
				}	
				else
				{
					Show_Chinese_Tips("����ʧ��!");//ָ����ȷ
					processnum=0;
				}			
        delay_ms(1000);				
				break;

			case 2:
				Show_Chinese_Tips("���ڶԱ�ָ��");
				ensure=PS_Match();
			  delay_ms(500);
				if(ensure==0x00) 
				{
					ClearLcd();
					Show_Chinese_Tips("�Աȳɹ�!");//����ָ����һ����
					processnum=3;//�������Ĳ�
				}
				else 
				{
					Show_Chinese_Tips("�Ա�ʧ��");
					LCD_ShowChinese(0,16,"�����°�!",body_col,ground_col,16,0);//�Ա�ʧ�ܣ������°���ָ
			//		ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1000);
				break;

			case 3:
				ClearLcd();
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					ClearLcd();
					Show_Chinese_Tips("¼��ɹ�!");//����ָ��ģ��ɹ�
					processnum=4;//�������岽
				}else {processnum=0;Show_Chinese_Tips("����ʧ�ܣ�");}
				delay_ms(1000);
				break;
				
			case 4:	
				ClearLcd();
				Show_Chinese_Tips("��������!");//����ID������Enter������
			  LCD_ShowString(0,17,"<1-535>",body_col,ground_col,16,0);
			  key_val=Key_Scan();
				while(key_val!='#')
				{
					if(((key_val>='0')&&(key_val<='9')))
			    {
				    LCD_ShowChar(0+9*times,80,key_val,RED,BLACK,16,0);
				    times++;
						ID=ID*10+key_val-'0';
			    }
					key_val=Key_Scan();
				}
				if(ID>535)
				{
					processnum=0;
					Show_Chinese_Tips("���ʧ��!");
					break;
				}
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{			
					ClearLcd();				
					Show_Chinese_Tips("��ӳɹ�!");//���ָ�Ƴɹ�
					PS_ValidTempleteNum(&N);//����ָ�Ƹ���
					LCD_ShowChinese(0,16,"����ָ������Ϊ",body_col,ground_col,16,0);
					LCD_ShowIntNum(112+1,16,AS608Para.PS_max-N,3,body_col,ground_col,16);//��ʾʣ��ָ�Ƹ���
					delay_ms(1500);//��ʱ�������ʾ	
					ClearLcd();
					return ;
				}else {processnum=0;}//ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(800);

	}
}
void Del_FR(u16 FR_ID)	    //ɾ��ָ��
{
	u8  ensure;
	Show_Chinese_Tips("ɾ��ָ��");
	ensure=PS_DeletChar(FR_ID,1);//ɾ������ָ��
	if(ensure==0)
	{
		Show_Chinese_Tips("ɾ���ɹ���");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&N);//����ָ�Ƹ���
}

bool press_FR(void)   //ˢָ��
{
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{						
				return 1;
			}
			else 
				return 0;		
	  }
	}
  return 0;
}


//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	ClearLcd();
	Show_Chinese_Tips((u8*)EnsureMessage(ensure));
}





