#include <xc.h>
#include "Param.h"
#include <libpic30.h>
#include "System.h"
#include "IOCon.h"
#include "UART.h"
#include "SPI.h"
#include "LCD.h"
#include "Timer1.h"
#include "Wifi.h"
#include "ADC.h"

_FOSCSEL(FNOSC_FRC & IESO_OFF)
_FOSC(OSCIOFNC_ON & IOL1WAY_ON & FCKSM_CSDCMD)
_FWDT(FWDTEN_OFF)
_FICD(JTAGEN_OFF)

void LCD_Show(void);
void PassKey(void);
char KeyPress(void);
void KeyDisplay(char key);

int mode=1;
int key_count=0;
int i;
char key_buffer[4];
char current_key;

int main()
{
	PPS_Unlock();
	MCU_Init();
	IO_Init();
	UART1_Init();
	__delay_ms(1);
	SPI1_Init();
	Timer1_Init();
	ADC_Init();
	PPS_Lock();
	Timer1_ON;
	ADC_ON;
	SPI_ON;
	UART_ON;
	UART_TRANS;
	spistar();
	LCD_Init();

	while(1)
	{
		switch(mode)
		{
			case 1:
				LCD_Show();
				break;
			case 2:
				PassKey();
				break;
			case 3:
				WriteStringUART1("AT+RST\r\n");
				__delay_ms(10000);
				mode=4;
			case 4:
				WriteStringUART1("AT+CWMODE=1\r\n");
				__delay_ms(10000);
				mode=5;
				break;
			case 5:
				WriteStringUART1("AT+CWJAP=\"wifi_zombie\",\"sls930622\"\r\n");
				__delay_ms(10000);
				mode=6;
				break;
			case 6:
				WriteStringUART1("AT+CIPMUX=1\r\n");
				__delay_ms(10000);
				mode=7;
				break;
			case 7:
				WriteStringUART1("AT+CIPSERVER=1,8713\r\n");
				__delay_ms(10000);
				mode=8;
				break;
			case 8:
				ADC_Convert();
				if(temp_out<18)
				{
					WriteStringUART1("AT+CIPSEND=0,30\r\n");
					__delay_ms(5000);
					WriteStringUART1("Now is ");
					WriteUART1dec2string(temp_out);
					WriteStringUART1(" degree celcuis. Cold");
					__delay_ms(10000);
					__delay_ms(10000);
					__delay_ms(10000);
				}
				else if(temp_out>=18 && temp_out<=24)
				{
					WriteStringUART1("AT+CIPSEND=0,32\r\n");
					__delay_ms(5000);
					WriteStringUART1("Now is ");
					WriteUART1dec2string(temp_out);
					WriteStringUART1(" degree celcuis. Normal");
					__delay_ms(10000);
					__delay_ms(10000);
					__delay_ms(10000);
				}
				else if(temp_out>24)
				{
					WriteStringUART1("AT+CIPSEND=0,29\r\n");
					__delay_ms(5000);
					WriteStringUART1("Now is ");
					WriteUART1dec2string(temp_out);
					WriteStringUART1(" degree celcuis. Hot");
					__delay_ms(10000);
					__delay_ms(10000);
					__delay_ms(10000);
				}
				mode=9;
				break;
			case 9:
				WriteStringUART1("AT+CIPCLOSE=0\r\n");
				__delay_ms(10000);
				__delay_ms(10000);
				__delay_ms(10000);
				mode=8;
				break;
		}
	}
}

void LCD_Show(void)
{
	POINT_COLOR = RED;
    LCD_DrawRectangle(0,0,239,319);
    LCD_DrawRectangle(1,1,238,318);
    LCD_DrawRectangle(2,2,237,317);    
    BACK_COLOR = WHITE;
   	LCD_ShowString(88,10,"UEEA4663");
   	LCD_ShowString(35,40,"Embedded System Design");
   	LCD_ShowString(73,70,"Assignment 3");
	LCD_SquareButton(70,159,100,"START",BLACK,100,205);
	if(Touch_SquareDetect(70,159,100))
	{
    	LCD_Clear(YELLOW);
		BACK_COLOR = YELLOW;
		mode=2;
	}
}

void PassKey(void)
{
	LCD_SquareButton(0,79,80,"1",BLACK,36,115);
    LCD_SquareButton(80,79,80,"2",BLACK,116,115);
    LCD_SquareButton(160,79,80,"3",BLACK,196,115);
    LCD_SquareButton(0,159,80,"4",BLACK,36,195);
    LCD_SquareButton(80,159,80,"5",BLACK,116,195);
    LCD_SquareButton(160,159,80,"6",BLACK,196,195);
    LCD_SquareButton(0,239,80,"7",BLACK,36,275);
    LCD_SquareButton(80,239,80,"8",BLACK,116,275);
    LCD_SquareButton(160,239,80,"9",BLACK,196,275);
	
	if(key_count<4)
	{
		if(KeyPress())
		{
			current_key=KeyPress();
			__delay_ms(500);
			KeyDisplay(current_key);
			__delay_ms(1000);
			key_count++;
			LCD_Clear(YELLOW);
			BACK_COLOR = YELLOW;
		}
	}
	else
	{
		if(key_buffer[0]=='1'&&key_buffer[1]=='2'&&key_buffer[2]=='3'&&key_buffer[3]=='4')
		{
			LCD_Clear(WHITE);
			BACK_COLOR = WHITE;
			LCD_ShowString(88,10,"Success");
			__delay_ms(2000);
			LCD_Clear(WHITE);
			BACK_COLOR = WHITE;
			for(i=0;i<4;i++)
			{
				key_buffer[i]='\0';
			}
			key_count=0;
			mode=3;
		}
		else
		{
			LCD_Clear(WHITE);
			BACK_COLOR = WHITE;
			LCD_ShowString(88,10,"Fail");
			__delay_ms(2000);
			LCD_Clear(YELLOW);
			BACK_COLOR = YELLOW;
			for(i=0;i<4;i++)
			{
				key_buffer[i]='\0';
			}
			key_count=0;
		}
	}

}

char KeyPress(void)
{
    if(Touch_SquareDetect(0,79,80))
    {
		key_buffer[key_count] = '1';
		return 1;
    }      
            
    else if(Touch_SquareDetect(80,79,80))
    {
		key_buffer[key_count] = '2';
		return 2;
    }        
    
    else if(Touch_SquareDetect(160,79,80))
    {
		key_buffer[key_count] = '3';
		return 3;
    }        
            
    else if(Touch_SquareDetect(0,159,80))
    {
		key_buffer[key_count] = '4';
		return 4;
    }
            
    else if(Touch_SquareDetect(80,159,80))
    {
		key_buffer[key_count] = '5';
		return 5;
    }            
            
    else if(Touch_SquareDetect(160,159,80))
    {
		key_buffer[key_count] = '6';
		return 6;
    }
            
    else if(Touch_SquareDetect(0,239,80))
    {
		key_buffer[key_count] = '7';
		return 7;
    }
            
    else if(Touch_SquareDetect(80,239,80))
    {
		key_buffer[key_count] = '8';
		return 8;
    }
            
    else if(Touch_SquareDetect(160,239,80))
    {
		key_buffer[key_count] = '9';
		return 9;
    }
    
    else
    {
        return 0;
    }
}

void KeyDisplay(char key)
{
    if (key == 1)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 1 is pressed");
    }
    
    else if (key == 2)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 2 is pressed");
    }
    
    else if (key == 3)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 3 is pressed");
    }
    
    else if (key == 4)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 4 is pressed");
    }
    
    else if (key == 5)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 5 is pressed");
    }
    
    else if (key == 6)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 6 is pressed");
    }
    
    else if (key == 7)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 7 is pressed");
    }
    
    else if (key == 8)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 8 is pressed");
    }
    
    else if (key == 9)
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"Key 9 is pressed");
    }
    
    else
    {
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;
        POINT_COLOR = BLACK;
        LCD_ShowString(10,50,"No Key is pressed");
    }
}