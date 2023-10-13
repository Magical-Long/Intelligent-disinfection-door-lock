#include "as608user.h"
#include "UI.h"
#include "lcd.h"
#include "lcd_init.h"
#include "key.h"
#include "usart.h"
#include "as608.h"

SysPara AS608Para;//指纹模块AS608参数
extern vu16 USART1_RX_STA;       //接收状态标记	  
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 N;//模块内有效模板个数
SearchResult seach;



void Add_FR(void )     //添加指纹
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
				LCD_ShowChinese(0,0,"请按一次手指!",body_col,ground_col,16,0);
				ensure=PS_GetImage();
			  while(ensure!=0x00)
				{
					ensure=PS_GetImage();
				}
				ensure=PS_GenChar(CharBuffer1);//生成特征
        if(ensure==0x00)
				{		
    			ClearLcd();
		    	Show_Chinese_Tips("输入成功!");//指纹正确
				  i=0;
				  processnum=1;//跳到第二步
				}	
				else
				{
					Show_Chinese_Tips("输入失败!");//指纹正确
					processnum=0;
				}									
				delay_ms(1000);		
				break;
			
			case 1:
				i++;
				ClearLcd();
				LCD_ShowChinese(0,0,"请再按一次手指!           ",body_col,ground_col,16,0);//再按一次手指
				ensure=PS_GetImage();
			  while(ensure!=0x00)
				{
					ensure=PS_GetImage();
				}
				ensure=PS_GenChar(CharBuffer2);//生成特征	
        if(ensure==0x00)
				{		
    			ClearLcd();
		    	Show_Chinese_Tips("输入成功!");//指纹正确
				  i=0;
				  processnum=2;//跳到第三步
				}	
				else
				{
					Show_Chinese_Tips("输入失败!");//指纹正确
					processnum=0;
				}			
        delay_ms(1000);				
				break;

			case 2:
				Show_Chinese_Tips("正在对比指纹");
				ensure=PS_Match();
			  delay_ms(500);
				if(ensure==0x00) 
				{
					ClearLcd();
					Show_Chinese_Tips("对比成功!");//两次指纹是一样的
					processnum=3;//跳到第四步
				}
				else 
				{
					Show_Chinese_Tips("对比失败");
					LCD_ShowChinese(0,16,"请重新按!",body_col,ground_col,16,0);//对比失败，请重新按手指
			//		ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				ClearLcd();
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					ClearLcd();
					Show_Chinese_Tips("录入成功!");//生成指纹模板成功
					processnum=4;//跳到第五步
				}else {processnum=0;Show_Chinese_Tips("载入失败！");}
				delay_ms(1000);
				break;
				
			case 4:	
				ClearLcd();
				Show_Chinese_Tips("请输入编号!");//输入ID并按“Enter”保存
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
					Show_Chinese_Tips("添加失败!");
					break;
				}
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
					ClearLcd();				
					Show_Chinese_Tips("添加成功!");//添加指纹成功
					PS_ValidTempleteNum(&N);//读库指纹个数
					LCD_ShowChinese(0,16,"可用指纹余量为",body_col,ground_col,16,0);
					LCD_ShowIntNum(112+1,16,AS608Para.PS_max-N,3,body_col,ground_col,16);//显示剩余指纹个数
					delay_ms(1500);//延时后清除显示	
					ClearLcd();
					return ;
				}else {processnum=0;}//ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(800);

	}
}
void Del_FR(u16 FR_ID)	    //删除指纹
{
	u8  ensure;
	Show_Chinese_Tips("删除指纹");
	ensure=PS_DeletChar(FR_ID,1);//删除单个指纹
	if(ensure==0)
	{
		Show_Chinese_Tips("删除成功！");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&N);//读库指纹个数
}

bool press_FR(void)   //刷指纹
{
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{						
				return 1;
			}
			else 
				return 0;		
	  }
	}
  return 0;
}


//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	ClearLcd();
	Show_Chinese_Tips((u8*)EnsureMessage(ensure));
}





