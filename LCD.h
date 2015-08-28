/********************** (C) COPYRIGHT 2015 CHONG HON FONG****************************
*
* @file     LCD.h
* @brief    Functions for LCD display
*
* @author   Chong Hon Fong
* @date     10/08/2015
*
*************************************************************************************/

#ifndef LCD_H
#define	LCD_H

#define TJCTM24024_SPI                    // Mode Select

#ifdef TJCTM24024_SPI
// Define LCD Size
#define LCD_Width 240
#define LCD_Height 320

// LCD Show
extern unsigned int BACK_COLOR, POINT_COLOR; // Background Colours, Point Colours
void LCD_Init(void);
void LCD_Clear(unsigned int Color);
void LCD_WR_DATA(unsigned int da);
void LCD_WR_DATA8(unsigned char da);
void LCD_WR_REG(unsigned char da);
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void showhanzi(unsigned int x, unsigned int y, unsigned char index);
void LCD_DrawPoint(unsigned int x, unsigned int y);
void LCD_DrawPoint_big(unsigned int x, unsigned int y);
// unsigned int LCD_ReadPoint(unsigned int x, unsigned int y); //Read point
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_DrawCircle(unsigned int x0, unsigned int y0, unsigned char r);
void LCD_SquareButton(unsigned char x, unsigned char y, unsigned char width, unsigned char *word, unsigned int color, unsigned int x_st, unsigned int y_st);
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);
void LCD_ShowChar(unsigned int x, unsigned int y, unsigned char num, unsigned char mode);
void LCD_ShowNum(unsigned int x, unsigned int y, unsigned long num, unsigned char len);
void LCD_Show2Num(unsigned int x, unsigned int y, unsigned int num, unsigned char len);
void LCD_ShowString(unsigned int x, unsigned int y, const unsigned char *p);

// LCD Touch
// Define Touch Panel Interface
#define  CMD_RDX  0xD0
#define  CMD_RDY  0x90

struct tp_pix_
{
    unsigned int x;
    unsigned int y;
};

struct tp_pixu32_
{
    unsigned long x;
    unsigned long y;
};

extern struct tp_pix_  tp_pixad,tp_pixlcd;
extern unsigned int vx, vy;
extern unsigned int chx, chy;
extern unsigned int T_x, T_y;
extern unsigned int TP_x, TP_y;
unsigned char tpstate(void);
void spistar(void);
void Touch_Adjust(void);
unsigned char Touch_SquareDetect(unsigned char x, unsigned char y, unsigned char width);
void point(void);

// Colour of Pen
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07         // Brown Red
#define GRAY             0X8430
// GUI Colours

#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
// Above three are panel colours

#define LIGHTGREEN     	 0X841F
#define LGRAY            0XC618         // Panel Colour

#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12

// LCD Font
extern const unsigned char image[];
extern const unsigned char hanzi[];
extern const unsigned char asc2_1608[1520];

#endif

#endif