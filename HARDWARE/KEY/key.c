#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

/**注意矩阵键盘从左到右为1 2 3 4 5 6 7 8  
   其中1 2 3 4对应列1 2 3  A  对应引脚B12 13 14 15
	 5 6 7 8  对应行A B C D            B8 9 10 11
   采用反线法
*/
u8 FLAG = 0;

void Key_Init(void) //IO初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStructure);



}


u8 Key_Scan(void)
{
  u8 temp = 0;
	u8 key_val = '\0';
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);			//拉低行线
	delay_us(10);
 
	temp=(GPIO_ReadInputData(GPIOB) >> 8)&0xff;	
	
	//没有按键按下时扫描
	if (temp == 0xf0) 
	{
			delay_ms(50); 
			GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);			//拉低行线
			delay_us(10);
			temp=(GPIO_ReadInputData(GPIOB) >> 8)&0xff;	
		
			if (temp != 0xf0) //按键按下时，对键值进行赋值
			{
				//第一行
				GPIO_Write(GPIOB,0);
				delay_ms(5);
				GPIO_Write(GPIOB,(uint16_t)(0xFE << 8)); 
				
				if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
				{
						delay_ms(20);//消抖
 
						if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOB) >> 8) & 0XFE);		//对列进行扫描
								switch(temp)
								{
										case 0xEE:  key_val = '1' ;  break;
										case 0xDE:  key_val = '2' ;  break;
										case 0xBE:  key_val = '3' ;  break;
										case 0x7E:  key_val = 'A' ;  break;
										default:    key_val = '\0';  break;
								}
						}
				}
				
				//第二行
				GPIO_Write(GPIOB,0);
				delay_ms(5);
				GPIO_Write(GPIOB,(uint16_t)(0xFD << 8));
				
				if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0)!= 0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOB) >> 8) & 0XFD);
								switch(temp)
								{
										case 0xED:  key_val ='4';    break;
										case 0xDD:  key_val ='5';   break;
										case 0xBD:  key_val ='6';   break;
										case 0x7D:  key_val ='B';   break;
										default:    key_val ='\0';   break;
								}
						}
				}
				
				//第三行
				GPIO_Write(GPIOB,0);
				delay_ms(5);
				GPIO_Write(GPIOB,(uint16_t)(0xFB << 8));
				
				if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOB) >> 8) & 0XFB);
								switch(temp)
								{
										case 0xEB:  key_val = '7';   break;
										case 0xDB:  key_val = '8';    break;
										case 0xBB:  key_val = '9';    break;
										case 0x7B:  key_val = 'C';    break;
										default:    key_val = '\0';   break;
								}
						}
				}
				
				//第四行
				GPIO_Write(GPIOB,0);
				delay_ms(5);
				GPIO_Write(GPIOB,(uint16_t)(0xF7 << 8));
				
				if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) !=0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOB) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOB) >> 8) & 0XF7);
								switch(temp)
								{
										case 0xE7:  key_val = '*';    break;
										case 0xD7:  key_val = '0';    break;
										case 0xB7:  key_val = '#';    break;
										case 0x77:  key_val = 'D';    break;
										default:    key_val = '\0';   break;
								}
						}
					}
				}
			}
	
	return key_val;
 
 
}




