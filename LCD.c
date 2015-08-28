/********************** (C) COPYRIGHT 2015 CHONG HON FONG****************************
*
* @file     LCD.c
* @brief    Functions for LCD display
*
* @author   Chong Hon Fong
* @date     10/08/2015
*
*************************************************************************************/

/* Includes ------------------------------------------------------------------------*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Param.h"
#include <libpic30.h>
#include "LCD.h"
#include "SPI.h"

#ifdef TJCTM24024_SPI
/**
*
*  @brief	Initialize TJCTM24024 TFT LCD in SPI mode
*
*  Call this function to at the start of TJCTM24024 TFT LCD application in SPI mode
*
*
*  @param	void
*  @return	void
*
*/

/************************************LCD Show**************************************/
unsigned int BACK_COLOR, POINT_COLOR; // Background Colours, Point Colours
unsigned char bitdata;
unsigned long int mypow(unsigned char m, unsigned char n);

void LCD_Init(void)
{
    LCD_REST = 0;
    __delay_ms(20);
    LCD_REST = 1;
    __delay_ms(20);
    LCD_CS = 0;                             // Enable Chip Select

    LCD_WR_REG(0xCB);                       // Power Control A
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x02);

    LCD_WR_REG(0xCF);                       // Power Control B
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0XC1);
    LCD_WR_DATA8(0X30);

    LCD_WR_REG(0xE8);                       // Drive Timing Control A
    LCD_WR_DATA8(0x85);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x78);

    LCD_WR_REG(0xEA);                       // Drive Timing Control B
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0xED);                       // Power On Sequence Control
    LCD_WR_DATA8(0x64);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0X12);
    LCD_WR_DATA8(0X81);

    LCD_WR_REG(0xF7);                       // Pump Ratio Control
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC0);                       // Power Control 1
    LCD_WR_DATA8(0x23);                     // VRH[5:0]

    LCD_WR_REG(0xC1);                       // Power Control 2
    LCD_WR_DATA8(0x10);                     // SAP[2:0];BT[3:0]

    LCD_WR_REG(0xC5);                       // VCM Control 1
    LCD_WR_DATA8(0x3e);                     // Regulate Degree of Comparison
    LCD_WR_DATA8(0x28);

    LCD_WR_REG(0xC7);                       // VCM Control 2
    LCD_WR_DATA8(0x86);                     // --

    LCD_WR_REG(0x36);                       // Memory Access Control
    LCD_WR_DATA8(0x48);                     // 48 68 Vertical Screen //28 E8 Horizontal Screen

    LCD_WR_REG(0x3A);                       // COLMOD: Pixel Format Set
    LCD_WR_DATA8(0x55);

    LCD_WR_REG(0xB1);                       // Frame Rate Control (In Normal Mode/Full Colours)
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x18);

    LCD_WR_REG(0xB6);                       // Display Function Control
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x82);
    LCD_WR_DATA8(0x27);

    LCD_WR_REG(0xF2);                       // 3Gamma Function Disable
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x26);                       // Gamma curve selected
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xE0);                       // Set Gamma
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x4E);
    LCD_WR_DATA8(0xF1);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0XE1);                       // Set Gamma
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0xC1);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0x2A);                       // Column Address Set
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0xEF);

    LCD_WR_REG(0x2B);                       // Page Address Set
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x3F);
    
    LCD_WR_REG(0x11);                       // Exit Sleep
    __delay_ms(120);

    LCD_WR_REG(0x29);                       // Display ON
    LCD_WR_REG(0x2c);

    LCD_Clear(WHITE);
}

// Clear LCD function
// Color: Colour want to apply in order to clear LCD
void LCD_Clear(unsigned int Color)
{
    unsigned int i,j;
    unsigned int a;
    a = Color;
    Address_set(0,0,LCD_Width-1,LCD_Height-1);
    for(i=0;i<LCD_Width;i++)
    {
        for (j=0;j<LCD_Height;j++)
        {
            LCD_WR_DATA(Color);
        }
    }
}

void LCD_Writ_Bus(unsigned char da)               // Method 2 that use SPI communication
{
    SPI1_SendReceiveData8(da);
}

void LCD_WR_DATA(unsigned int da)
{
    LCD_DC = 1;
    LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
}

void LCD_WR_DATA8(unsigned char da)               // Send Data - 8-Bit
{
    LCD_DC = 1;
    LCD_Writ_Bus(da);
}

void LCD_WR_REG(unsigned char da)
{
    LCD_DC = 0;
    LCD_Writ_Bus(da);
}

void LCD_WR_REG_DATA(unsigned int reg, unsigned int da)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_WR_REG(0x2a);
    LCD_WR_DATA8(x1>>8);
    LCD_WR_DATA8(x1);
    LCD_WR_DATA8(x2>>8);
    LCD_WR_DATA8(x2);

    LCD_WR_REG(0x2b);
    LCD_WR_DATA8(y1>>8);
    LCD_WR_DATA8(y1);
    LCD_WR_DATA8(y2>>8);
    LCD_WR_DATA8(y2);

    LCD_WR_REG(0x2C);
}

// Show chinese words (Size of 32 x 33) at particular location
// dcolor: content colour
// gbcolor: background colour
void showhanzi(unsigned int x, unsigned int y, unsigned char index)
{
    unsigned char i,j;
    unsigned char *temp = hanzi;
    Address_set(x,y,x+31,y+31);             // Set region
    temp += index*128;
    for(j=0;j<128;j++)
    {
        for(i=0;i<8;i++)
        {
            if((*temp&(1<<i)) != 0)
            {
                LCD_WR_DATA(POINT_COLOR);
            }
            else
            {
                LCD_WR_DATA(BACK_COLOR);
            }
        }
        temp++;
     }
}

// Draw point
// POINT_COLOR: colour of point
// Top left coordinate: (0,0)
// Bottom right coordinate: (240,320)
void LCD_DrawPoint(unsigned int x, unsigned int y)
{
    Address_set(x,y,x,y);                   // Select cursor location
    LCD_WR_DATA(POINT_COLOR);
}

// Draw bigger point
// POINT_COLOR: colour of point
void LCD_DrawPoint_big(unsigned int x, unsigned int y)
{
    LCD_Fill(x-1, y-1, x+1, y+1, POINT_COLOR);
}

// Draw line
// x1,y1: Initial coordinate
// x2,y2: Final coordinate
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int t;
    int xerr = 0,yerr = 0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;

    delta_x = x2 - x1;                      // Compute distance
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if(delta_x > 0)
        incx = 1;                           // Select one step direction
    else if(delta_x == 0)
        incx = 0;                           // Vertical line
    else
        {incx =- 1;delta_x =- delta_x;}

    if(delta_y>0)
        incy = 1;
    else if(delta_y == 0)
        incy = 0;                           // Horizontal line
    else
        {incy =- 1;delta_y =- delta_y;}
    
    if(delta_x > delta_y)
        distance = delta_x;                 // Select basic distance coordinate
    else 
        distance = delta_y;

    for(t=0;t<=distance+1;t++ )             // Draw line out
    {
        LCD_DrawPoint(uRow,uCol);           // Draw point
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

// Draw rectangle
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}

// Draw circle at specific location
// (x,y): Centre
// r: Radius
void LCD_DrawCircle(unsigned int x0, unsigned int y0, unsigned char r)
{
    int a,b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r<<1);                        // Determine location of next point
    while(a <= b)
    {
        LCD_DrawPoint(x0-b,y0-a);           // 3
        LCD_DrawPoint(x0+b,y0-a);           // 0
        LCD_DrawPoint(x0-a,y0+b);           // 1
        LCD_DrawPoint(x0-b,y0-a);           // 7
        LCD_DrawPoint(x0-a,y0-b);           // 2
        LCD_DrawPoint(x0+b,y0+a);           // 4
        LCD_DrawPoint(x0+a,y0-b);           // 5
        LCD_DrawPoint(x0+a,y0+b);           // 6
        LCD_DrawPoint(x0-b,y0+a);
        a++;
        // Use Bresenham method to draw circle
        if(di < 0)
            di += 4 * a+6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
        LCD_DrawPoint(x0+a,y0+b);
    }
}

// Draw a square button
// x: Starting x coordinate
// y: Starting y coordinate
// width: Width of button
// word: Word that wish to appear inside button (Need manually calibrate on the alignment of text)
// color: Colour of boundary line
void LCD_SquareButton(unsigned char x, unsigned char y, unsigned char width, unsigned char *word, unsigned int color, unsigned int x_st, unsigned int y_st)
{
    unsigned char i;
    POINT_COLOR = color;
    LCD_DrawLine(x+2,y,x+width-2,y);
    LCD_DrawPoint(x+1,y+1);
    LCD_DrawPoint(x+width-1,y+1);
    for(i=2;i<= width-2;i++)
    {
        LCD_DrawPoint(x,y+i);
        LCD_DrawPoint(x+width,y+i);
    }
    LCD_DrawPoint(x+1,y+width-1);
    LCD_DrawPoint(x+width-1,y+width-1);
    LCD_DrawLine(x+2,y+width,x+width-2,y+width);
    LCD_ShowString(x_st,y_st,word);
}

// Fill colour at specific location
// Size of Region: (xend-xsta) * (yend-ysta)
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
    unsigned int i,j;
    Address_set(xsta, ysta, xend, yend);    // Select cursor location
    for(i=ysta;i<=yend;i++)
    {
        for(j=xsta;j<=xend;j++)
            LCD_WR_DATA(color);             // Select cursor location
    }
}

// Show character at specific location
// num: character want to show: " "--->"~"
// mode: accumulate step method(1) or non accumulate step method(0)
void LCD_ShowChar(unsigned int x, unsigned int y, unsigned char num, unsigned char mode)
{
    unsigned char temp;
    unsigned char pos,t;
    unsigned int x0 = x;
    unsigned int colortemp = POINT_COLOR;
    if(x>LCD_Width-16||y>LCD_Height-16)
        return;
    
    // Select window
    num = num -' ';                         // Get shift value
    Address_set(x,y,x+8-1,y+16-1);          // Select cursor location
    if(!mode)                               // Non accumulate step method
    {
        for(pos=0;pos<16;pos++)
        {
            temp = asc2_1608[(unsigned int)num*16+pos];		 // Use 1608 font type
            for(t=0;t<8;t++)
            {
                if(temp&0x01)
                    POINT_COLOR = colortemp;
                else
                    POINT_COLOR = BACK_COLOR;
                LCD_WR_DATA(POINT_COLOR);
                temp >>= 1;
                x++;
            }
                x = x0;
                y++;
        }
    }
    else                                    // Accumulate step method
    {
        for(pos=0;pos<16;pos++)
        {
            temp = asc2_1608[(unsigned int)num*16+pos];		 // Use 1608 font type
            for(t=0;t<8;t++)
            {
                if(temp&0x01)
                    LCD_DrawPoint(x+t,y+pos);                   // Draw one point
                temp >>= 1;
            }
        }
    }
    POINT_COLOR = colortemp;
}

// Show two numbers
// x,y: Coordinate of starting point
// len: Length of number
// color: Colour
// num: Value from 0 to 4294967295
void LCD_ShowNum(unsigned int x, unsigned int y, unsigned long num, unsigned char len)
{
    unsigned char t,temp;
    unsigned char enshow = 0;
    num = (unsigned int)num;
    for(t=0;t<len;t++)
    {
        temp = (num / mypow(10,len-t-1)) % 10;
        if(enshow == 0 && t<(len-1))
        {
                if(temp == 0)
                {
                    LCD_ShowChar(x+8*t,y,' ',0);
                    continue;
                }
                else
                    enshow = 1;

        }
        LCD_ShowChar(x+8*t,y,temp+48,0);
    }
}

// Show two numbers
// x,y: Coordinate of starting point
// num: Value from 0 to 99
void LCD_Show2Num(unsigned int x, unsigned int y, unsigned int num, unsigned char len)
{
    unsigned char t,temp;
    for(t=0;t<len;t++)
    {
        temp = (num / mypow(10,len-t-1)) % 10;
        LCD_ShowChar(x+8*t,y,temp+'0',0);
    }
}

// Show character string
// x,y: Coordinate of starting point
// *p: Starting address of the character string
// Use 16 font type
void LCD_ShowString(unsigned int x, unsigned int y, const unsigned char *p)
{
    while(*p!='\0')
    {
        if(x > LCD_Width-16)
            {x = 0;y += 16;}
        if(y > LCD_Height-16)
            {y = x = 0; LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x += 8;
        p++;
    }
}

// Function of m^n
unsigned long int mypow(unsigned char m, unsigned char n)
{
    unsigned long int result = 1;

    while(n--)
        result *= m;

    return result;
}

/***********************************LCD Touch**************************************/

// Default value may cause touch screen not sensitive, not recommend to use default settings
// Recommend to use after some adjustment
// Default Variable Value
//unsigned int vx = 15242, vy = 11131;         // Scale factor, this value divide by 1000 means how many AD to represent one pixel
//unsigned int chx = 3898, chy = 145;          // Default AD value when pixel coordinate is 0
// My Variable Value
unsigned int vx = 16000, vy = 11000;         // Scale factor, this value divide by 1000 means how many AD to represent one pixel
unsigned int chx = 3900, chy = 145;          // Default AD value when pixel coordinate is 0

struct tp_pix_ tp_pixad,tp_pixlcd;          // Current coordinate AD value and pixel value

unsigned int ReadFromCharFrom7843();

unsigned char tpstate(void)
{
    return Penirq;
}
//**********************************************************

void spistar(void)                        // Start SPI
{
    CS = 1;
    DCLK = 1;
    DIN = 1;
    DCLK = 1;
}
//**********************************************************

void WriteCharTo7843(unsigned char num)   // SPI Write Data
{
    unsigned char count = 0;
    DCLK = 0;
    
    for(count=0;count<8;count++)
    {   
        num <<= 1;
        DIN = SRbits.C;
        DCLK = 0; Nop(); Nop(); Nop();      // Activate when increase
        DCLK = 1; Nop(); Nop(); Nop();
    }
}
//**********************************************************

unsigned int ReadFromCharFrom7843()      // SPI Read Data
{
    unsigned char count = 0;
    unsigned int Num = 0;
    for(count=0;count<12;count++)
    {
        Num <<= 1;
        DCLK = 1; Nop(); Nop(); Nop();      // Activate when decrease
        DCLK = 0; Nop(); Nop(); Nop();

        if(DOUT)
        {
            Num |= 1;

        }
    }
    return(Num);
}

// Read ADC from 7846/7843/XPT2046/UH7843/UH7846
// 0x90=y   0xd0-x
unsigned int ADS_Read_AD(unsigned char CMD)
{
    unsigned int l;
    CS = 0;
    WriteCharTo7843(CMD);                   // Use control immediate use difference method to read x coordinate
    DCLK = 1; Nop(); Nop(); Nop(); Nop();
    DCLK = 0; Nop(); Nop(); Nop(); Nop();
    l = ReadFromCharFrom7843();
    CS = 1;
    return l;
}

// Read one coordinate value
// Consecutive read data by READ_TIMES, arrange them in ascending order
// Then remove highest and lowest LOST_VAL integer, then get avarage
#define READ_TIMES 15                       // Number of read times
#define LOST_VAL 5                          // Lost value
unsigned int ADS_Read_XY(unsigned char xy)
{
    unsigned int i, j;
    unsigned int buf[READ_TIMES];
    unsigned int sum=0;
    unsigned int temp;

    for(i=0;i<READ_TIMES;i++)
    {
        buf[i]=ADS_Read_AD(xy);
    }

    for(i=0;i<READ_TIMES-1; i++)            // Arrange
    {
        for(j=i+1;j<READ_TIMES;j++)
        {
            if(buf[i] > buf[j])             // Ascending order
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;

    for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
        sum += buf[i];

    temp = sum / (READ_TIMES-2*LOST_VAL);
    return temp;
}

// Read coordinate with wave filtering
// Smallest value cannot less than 100
unsigned char Read_ADS(unsigned int *x, unsigned int *y)
{
    unsigned int xtemp,ytemp;
    xtemp = ADS_Read_XY(CMD_RDX);
    ytemp = ADS_Read_XY(CMD_RDY);

    if(xtemp<100 || ytemp<100)
        return 0;                           // Read failed

    *x = xtemp;
    *y = ytemp;
    return 1;                               // Read succeed
}

// Read ADS7846 twice, consecutive read two valid AD value
// Difference between this two values cannot larger 50
// If satisfy condition, then succeed, else failed
// This function can improve accuracy significantly
#define ERR_RANGE 20                // Range of variation
unsigned char Read_ADS2(unsigned int *x,unsigned int *y)
{
    unsigned int x1,y1;
    unsigned int x2,y2;
    unsigned char flag;
    
    flag = Read_ADS(&x1,&y1);
    
    if(flag == 0)
        return(0);
    
    flag = Read_ADS(&x2,&y2);
    
    if(flag==0)
        return(0);
    
    if(((x2<=x1&&x1<x2+ERR_RANGE) || (x1<=x2&&x2<x1+ERR_RANGE)) && ((y2<=y1&&y1<y2+ERR_RANGE) || (y1<=y2&&y2<y1+ERR_RANGE))) // Two sampling occur within +-ERR_RANGE
    {
        *x = (x1+x2) >> 1;
        *y = (y1+y2) >> 1;
        return 1;
    }
    
    else
        return 0;
}

// Used when adjusting, this function can read coordinate accurately
unsigned char Read_TP_Once(void)
{
    unsigned char re = 0;
    unsigned int x1,y1;

    while(re == 0)
    {
        while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
        __delay_ms(10);
        while(!Read_ADS2(&x1,&y1));
        if(tp_pixad.x==x1 && tp_pixad.y==y1)
        {
            re=1;
        }
    }
    return re;
}

// Function related to LCD
// Draw one touch point
// Use when adjusting
void Draw_Touch_Point(unsigned int x,unsigned int y)
{
    LCD_DrawLine(x-12,y,x+13,y);            // Horizontal line
    LCD_DrawLine(x,y-12,x,y+13);            // Vertical line
    LCD_DrawPoint(x+1,y+1);
    LCD_DrawPoint(x-1,y+1);
    LCD_DrawPoint(x+1,y-1);
    LCD_DrawPoint(x-1,y-1);
//    LCD_DrawCircle(x,y,6);                     // Draw centre point
}

// Convert Result
// Change convertion result according to touch screen parameter, and save into X0 and Y0
unsigned char Convert_Pos(void)
{
    unsigned char l = 0;
    if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))
    {
            l = 1;
            tp_pixlcd.x = tp_pixad.x > chx?((unsigned long)tp_pixad.x-(unsigned long)chx)*1000/vx:((unsigned long)chx-(unsigned long)tp_pixad.x)*1000/vx;
            tp_pixlcd.y = tp_pixad.y > chy?((unsigned long)tp_pixad.y-(unsigned long)chy)*1000/vy:((unsigned long)chy-(unsigned long)tp_pixad.y)*1000/vy;
    }
    return l;
}

// Touch Screen Adjust Code
// Get 4 adjust parameter
#define tp_pianyi 50                        // Adjust coordinate offset
#define tp_xiaozhun 1000                    // Adjust accuracy
void Touch_Adjust(void)
{
    float vx1,vx2,vy1,vy2;                  // Scale factor, this value divide by 1000 means how many AD to represent one pixel
    unsigned int chx1,chx2,chy1,chy2;       // Default AD value when pixel coordinate is 0
    unsigned int lx,ly;
    struct tp_pixu32_ p[4];
    unsigned char cnt = 0;
    cnt = 0;
    POINT_COLOR = BLUE;
    BACK_COLOR = WHITE;
    LCD_Clear(WHITE);                       // Clear screen
    POINT_COLOR = RED;                      // Red colour
    LCD_Clear(WHITE);                       // Clear screen
    Draw_Touch_Point(tp_pianyi,tp_pianyi);  // Draw point 1
    while(1)
    {
        if(Penirq==0)                       // Key is pressed
        {
            if(Read_TP_Once())              // Get single key value
            {
                p[cnt].x=tp_pixad.x;
                p[cnt].y=tp_pixad.y;
                cnt++;
            }
            switch(cnt)
            {
                case 1:
                    LCD_Clear(WHITE);       // Clear screen
                    while(!Penirq)          // Wait key until released
                    {
                    }
                    Draw_Touch_Point(LCD_Width-tp_pianyi-1,tp_pianyi); // Draw point 2
                    break;
                case 2:
                    LCD_Clear(WHITE);       // Clear screen
                    while(!Penirq)          // Wait key until released
                    {
                    }
                    Draw_Touch_Point(tp_pianyi,LCD_Height-tp_pianyi-1); // Draw point 3
                    break;
                case 3:
                    LCD_Clear(WHITE);       // Clear screen
                    while(!Penirq)          // Wait key until released
                    {
                    }
                    Draw_Touch_Point(LCD_Width-tp_pianyi-1,LCD_Height-tp_pianyi-1); // Draw point 4
                    break;
                case 4:                     // All 4 points is get
                    LCD_Clear(WHITE);       // Clear screen
                    while(!Penirq)          // Wait key until released
                    {
                    }
                    vx1 = p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_Width-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_Width-tp_pianyi-tp_pianyi);
                    chx1 = p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
                    vy1 = p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_Height-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_Height-tp_pianyi-tp_pianyi);
                    chy1 = p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000;

                    vx2 = p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_Width-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_Width-tp_pianyi-tp_pianyi);
                    chx2 = p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
                    vy2 = p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_Height-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_Height-tp_pianyi-tp_pianyi);
                    chy2 = p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000;


                    if((vx1>vx2&&vx1>vx2+tp_xiaozhun) || (vx1<vx2&&vx1<vx2-tp_xiaozhun) || (vy1>vy2&&vy1>vy2+tp_xiaozhun) || (vy1<vy2&&vy1<vy2-tp_xiaozhun))
                    {
                        cnt = 0;
                        LCD_Clear(WHITE);   // Clear screen
                        Draw_Touch_Point(tp_pianyi,tp_pianyi); // Draw point
                        continue;
                    }
                    vx = (vx1+vx2) / 2;
                    vy = (vy1+vy2) / 2;
                    chx = (chx1+chx2) / 2;
                    chy = (chy1+chy2) / 2;

                    // Display adjust info
                    LCD_Clear(WHITE);       // Clear screen
                    POINT_COLOR = BLACK;
                    BACK_COLOR = BLUE;

                    lx = 0;ly = 50;
                    LCD_ShowString(lx,ly,"VX1:");lx += 40;LCD_ShowNum(lx,ly,vx1,5);
                    lx =0;ly += 20;
                    LCD_ShowString(lx,ly,"Vy1:");lx += 40;LCD_ShowNum(lx,ly,vy1,5);
                    lx = 0;ly += 20;
                    LCD_ShowString(lx,ly,"CHX1:");lx += 40;LCD_ShowNum(lx,ly,chx1,5);
                    lx = 0;ly += 20;
                    LCD_ShowString(lx,ly,"CHY1:");lx += 40;LCD_ShowNum(lx,ly,chy1,5);

                    lx = 100;ly = 50;
                    LCD_ShowString(lx,ly,"VX2:");lx+=40;LCD_ShowNum(lx,ly,vx2,5);
                    lx = 100;ly += 20;
                    LCD_ShowString(lx,ly,"Vy2:");lx+=40;LCD_ShowNum(lx,ly,vy2,5);
                    lx = 100;ly += 20;
                    LCD_ShowString(lx,ly,"CHX2:");lx+=40;LCD_ShowNum(lx,ly,chx2,5);
                    lx = 100;ly += 20;
                    LCD_ShowString(lx,ly,"CHY2:");lx+=40;LCD_ShowNum(lx,ly,chy2,5);

                    lx = 50;ly = 150;
                    LCD_ShowString(lx,ly,"VX:");lx+=40;LCD_ShowNum(lx,ly,vx,5);
                    lx = 50;ly += 20;
                    LCD_ShowString(lx,ly,"Vy:");lx+=40;LCD_ShowNum(lx,ly,vy,5);
                    lx = 50;ly += 20;
                    LCD_ShowString(lx,ly,"CHX:");lx+=40;LCD_ShowNum(lx,ly,chx,5);
                    lx = 50;ly += 20;
                    LCD_ShowString(lx,ly,"CHY:");lx+=40;LCD_ShowNum(lx,ly,chy,5);

                    lx = 30;ly += 30;
                    LCD_ShowString(lx,ly,"Adjust OK!  Touch Anywhere To Continue");
                    Read_TP_Once();         // Waiting for any keys to continue

                    LCD_Clear(WHITE);       // Clear screen
                    return;                 // Correction completed
            }
        }
    }
}

void point(void)                           // Function for drawing
{
    double t = 0;

    while(1)
    {
        if(Penirq == 0)
        {
            t = 0;
            if(Convert_Pos())               // Get coordinate
            {
                LCD_ShowString(10,250,"X:");LCD_ShowNum(30,250,tp_pixad.x,4);
                LCD_ShowString(180,250,"Y:");LCD_ShowNum(200,250,tp_pixad.y,4);
                LCD_DrawPoint_big(tp_pixlcd.x,tp_pixlcd.y);
            }
        }
        
        else
        {
            t++;
            if(t > 65000)
            {
                return;
            }
        }

    }
}

// Detect if touch within a square area
// x: Starting x coordinate
// y: Starting y coordinate
// width: Width of button
unsigned char Touch_SquareDetect(unsigned char x, unsigned char y, unsigned char width)
{
    unsigned int x1 = 586;          // Smallest x coordinate for 240 pixel
    unsigned int x2 = 3516;         // Biggest x coordinate for 240 pixel
    unsigned int ori_diff_x;        
    ori_diff_x = (x2-x1)/240;       // Original difference of x = (3516-586)/240 = 12.20 per pixel
    unsigned int cal_start_x;       // Calculated starting coordinate of x
    unsigned int cal_end_x;         // Calculated ending coordinate of x
    
    unsigned int y1 = 398;          // Smallest y coordinate for 320 pixel
    unsigned int y2 = 3750;         // Biggest y coordinate for 320 pixel
    unsigned int ori_diff_y;        
    ori_diff_y = (y2-y1)/320;       // Original difference of y = (3750-398)/320 = 10.475 per pixel
    unsigned int cal_start_y;       // Calculated starting coordinate of y
    unsigned int cal_end_y;         // Calculated ending coordinate of y
    
    unsigned int a, b, c;
    
    while(1)
    {
        if(_RB14 == 0)
        {
            if(Convert_Pos())               // Get coordinate
            {
                cal_start_x = x2-((x+1)*ori_diff_x);                    // Calculate starting coordinate of x
                cal_end_x = x2-((x+width+1)*ori_diff_x);                // Calculate ending coordinate of x
                cal_start_y = y1+((y+1)*ori_diff_y);                    // Calculate starting coordinate of y
                cal_end_y = y1+((y+width+1)*ori_diff_y);                // Calculate ending coordinate of y
               
                a = (cal_start_x>tp_pixad.x && tp_pixad.x>cal_end_x);   // Check if the touch inside x region
                b = (cal_start_y<tp_pixad.y && tp_pixad.y<cal_end_y);   // Check if the touch inside y region
                c = a && b;                                             // Check if the touch inside the box with length x and width y
                
                if(c)
                {
                   return 1;
                }

                else
                {
                    return;
                }
            }
        }
        else
        {
            return 0;
        }
    }
}

/************************************LCD Font**************************************/
const unsigned char image[]={ /* 0X00,0X10,0X28,0X00,0X28,0X00,0X01,0X1B,*/
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0X7D,0XEF,
0XBA,0XD6,0XB6,0XB5,0XF3,0X9C,0XB2,0X94,0XB3,0X9C,0XB2,0X94,0X34,0XA5,0XF7,0XBD,
0XFB,0XDE,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XFB,0XDE,0XF3,0X9C,0XCB,0X5A,
0XC7,0X39,0X04,0X21,0X82,0X10,0X42,0X10,0X42,0X10,0X41,0X08,0X83,0X18,0X45,0X29,
0XC7,0X39,0X0C,0X63,0X75,0XAD,0X3C,0XE7,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0XE7,0XB2,0X94,0X08,0X42,0XC3,0X18,0X82,0X10,
0X04,0X21,0X45,0X29,0X86,0X31,0X86,0X31,0X86,0X31,0X86,0X31,0X45,0X29,0X04,0X21,
0X82,0X10,0X41,0X08,0XC3,0X18,0X08,0X42,0XF3,0X9C,0X3C,0XE7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFB,0XDE,0X0C,0X63,0XC3,0X18,0XC3,0X18,0X45,0X29,0XC7,0X39,
0X08,0X42,0X08,0X42,0X08,0X42,0X08,0X42,0X08,0X42,0X08,0X42,0XC7,0X39,0XC7,0X39,
0X86,0X31,0X86,0X31,0X04,0X21,0X41,0X08,0X82,0X10,0XCB,0X5A,0XBA,0XD6,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFB,0XDE,0XCB,0X5A,0X82,0X10,0X45,0X29,0XC7,0X39,0X08,0X42,0X08,0X42,
0X09,0X4A,0X49,0X4A,0X49,0X4A,0X49,0X4A,0X49,0X4A,0X49,0X4A,0X08,0X42,0XC7,0X39,
0XC7,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0X83,0X18,0X00,0X00,0XC8,0X41,0X38,0XC6,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7D,0XEF,0X8E,0X73,0X82,0X10,0X45,0X29,0XC7,0X39,0X08,0X42,0X09,0X4A,0X8A,0X52,
0X30,0X84,0XCF,0X7B,0X8A,0X52,0X49,0X4A,0X4A,0X52,0X49,0X4A,0XCB,0X5A,0XCF,0X7B,
0X0C,0X63,0X08,0X42,0XC7,0X39,0X86,0X31,0X45,0X29,0XC3,0X18,0X00,0X00,0X49,0X4A,
0XBA,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF3,0X9C,0XC3,0X18,0X04,0X21,0XC7,0X39,0X08,0X42,0X49,0X4A,0X49,0X4A,0X72,0X94,
0X7D,0XEF,0X7D,0XEF,0XB2,0X94,0X4A,0X52,0X49,0X4A,0X8A,0X52,0X75,0XAD,0XBE,0XF7,
0XBA,0XD6,0X4D,0X6B,0XC7,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0XC3,0X18,0X41,0X08,
0XCF,0X7B,0X7C,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBA,0XD6,
0X08,0X42,0X82,0X10,0XC7,0X39,0X08,0X42,0X49,0X4A,0X49,0X4A,0X8E,0X73,0XFB,0XDE,
0XFF,0XFF,0XBE,0XF7,0XBA,0XD6,0X8E,0X73,0X08,0X42,0X30,0X84,0X3C,0XE7,0X7D,0XEF,
0XFF,0XFF,0XB6,0XB5,0X49,0X4A,0XC7,0X39,0X86,0X31,0X45,0X29,0X04,0X21,0X41,0X08,
0X45,0X29,0XB6,0XB5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0X71,0X8C,
0X41,0X08,0X45,0X29,0X08,0X42,0X49,0X4A,0X49,0X4A,0X4A,0X52,0XB2,0X94,0XBE,0XF7,
0XBE,0XF7,0XB2,0X94,0XCF,0X7B,0XCF,0X7B,0X49,0X4A,0XB6,0XB5,0XF3,0X9C,0X0C,0X63,
0X38,0XC6,0XBA,0XD6,0X0C,0X63,0X87,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0XC3,0X18,
0X41,0X08,0X30,0X84,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0XE7,0XCB,0X5A,
0X41,0X08,0XC7,0X39,0X08,0X42,0X49,0X4A,0X4A,0X52,0X8A,0X52,0XF3,0X9C,0XFF,0XFF,
0X7D,0XEF,0XC7,0X39,0XC3,0X18,0X0C,0X63,0XCB,0X5A,0XB6,0XB5,0XB2,0X94,0XCB,0X5A,
0X75,0XAD,0XFA,0XD6,0X4D,0X6B,0X87,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0X04,0X21,
0X41,0X08,0X8A,0X52,0X79,0XCE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X38,0XC6,0X86,0X31,
0X04,0X21,0XC8,0X41,0X49,0X4A,0X49,0X4A,0X4A,0X52,0X49,0X4A,0XB1,0X8C,0XBE,0XF7,
0XBE,0XF7,0XB2,0X94,0XCF,0X7B,0XCF,0X7B,0X49,0X4A,0X74,0XA5,0X7D,0XEF,0X7C,0XE7,
0XBE,0XF7,0X79,0XCE,0X0C,0X63,0XC7,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0X04,0X21,
0X82,0X10,0X45,0X29,0X75,0XAD,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X34,0XA5,0X82,0X10,
0X86,0X31,0X08,0X42,0X49,0X4A,0X49,0X4A,0X8A,0X52,0X49,0X4A,0X4D,0X6B,0XBA,0XD6,
0XFF,0XFF,0XFF,0XFF,0X79,0XCE,0X0D,0X63,0XC7,0X39,0XCF,0X7B,0X7D,0XEF,0XFF,0XFF,
0XFF,0XFF,0X75,0XAD,0X08,0X42,0X86,0X31,0XC7,0X39,0X86,0X31,0X45,0X29,0X45,0X29,
0XC3,0X18,0XC3,0X18,0XB2,0X94,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XB2,0X8C,0X41,0X08,
0XC7,0X39,0X08,0X42,0X49,0X4A,0X49,0X4A,0X8A,0X52,0X8A,0X52,0X4A,0X4A,0XD0,0X7B,
0X7A,0XC6,0X7B,0XBE,0X90,0X6B,0XC9,0X39,0X88,0X31,0XC9,0X39,0XB3,0X84,0XBB,0XC6,
0XF8,0XB5,0XCC,0X5A,0X86,0X31,0XC7,0X39,0XC7,0X39,0X86,0X31,0X45,0X29,0X45,0X29,
0XC4,0X20,0X41,0X08,0X30,0X84,0X3C,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0XE7,0X8A,0X4A,0XC3,0X10,
0XC7,0X39,0X08,0X42,0X49,0X4A,0X49,0X4A,0X4A,0X4A,0X4A,0X42,0X09,0X3A,0X08,0X4A,
0X09,0X6B,0X49,0X7B,0XC6,0X7A,0X05,0X83,0X46,0X83,0XC5,0X7A,0XC6,0X72,0X09,0X7B,
0X48,0X5A,0X87,0X31,0X88,0X21,0X88,0X29,0X86,0X31,0X86,0X31,0X45,0X29,0X45,0X29,
0X04,0X21,0X41,0X08,0X4A,0X4A,0XBA,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XC5,0X82,0X50,0X05,0X41,
0XC7,0X29,0X08,0X42,0X49,0X4A,0X4A,0X42,0X49,0X4A,0X09,0X7B,0X88,0X9B,0XC6,0XB3,
0X21,0XD4,0XA0,0XDC,0XE1,0XE4,0X61,0XED,0X61,0XED,0X21,0XED,0XA0,0XE4,0X20,0XDC,
0X80,0XCB,0X43,0XAB,0XC4,0X82,0X06,0X5A,0X47,0X21,0X46,0X29,0X45,0X29,0X04,0X29,
0X04,0X19,0X82,0X10,0X82,0X18,0XF3,0X9C,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0XEF,0X4D,0X93,0X00,0XA0,0X82,0XB8,
0XC7,0X31,0X09,0X32,0X49,0X4A,0X86,0X7A,0X43,0XC3,0X6B,0XED,0XF4,0XF6,0XEB,0XFD,
0X20,0XFD,0X20,0XFD,0X60,0XFD,0XA0,0XFD,0XA0,0XFD,0X60,0XFD,0X60,0XFD,0X20,0XFD,
0XE0,0XFC,0XA0,0XFC,0X60,0XF4,0XC1,0XDB,0X83,0X9A,0XC5,0X49,0X45,0X29,0X04,0X19,
0XC4,0X20,0X82,0X38,0X00,0X50,0XCB,0X6A,0XBA,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XEE,0X04,0XA1,0X00,0XC0,0X00,0XF0,
0XC3,0XA0,0XC8,0X41,0X49,0X42,0X05,0X9B,0X20,0XFC,0XA4,0XFC,0X69,0XFD,0XE8,0XFD,
0X63,0XFD,0X20,0XFD,0X60,0XFD,0X60,0XFD,0X60,0XFD,0X20,0XFD,0X20,0XFD,0XE0,0XFC,
0XE0,0XFC,0XA0,0XFC,0X60,0XFC,0X20,0XFC,0X41,0XD3,0XC5,0X49,0X45,0X19,0XC4,0X38,
0X82,0X68,0X41,0X88,0X00,0X70,0X49,0X5A,0X79,0XCE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XF6,0X82,0XC0,0X00,0XD0,0X86,0XC1,
0X46,0XF1,0X41,0XC8,0X45,0X79,0X89,0X52,0X88,0X62,0X86,0X6A,0XC6,0X7A,0XC4,0XBB,
0XE1,0XFC,0X60,0XFD,0X60,0XFD,0XA0,0XFD,0XA0,0XFD,0X60,0XFD,0X60,0XFD,0XE0,0XFC,
0X60,0XE4,0X03,0X93,0X84,0X72,0X44,0X6A,0XC5,0X41,0X45,0X29,0XC3,0X58,0X41,0XA8,
0X40,0X98,0X00,0XB0,0X00,0X60,0X0C,0X6B,0X79,0XCE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0XEF,0XCE,0X83,0X82,0X88,0X00,0XF8,0XC4,0XD8,
0X0C,0XF3,0X8A,0XFA,0X82,0XE8,0X82,0XB0,0X45,0X69,0XC7,0X51,0X08,0X42,0X08,0X3A,
0X86,0X5A,0X83,0X9B,0XA2,0XBC,0X22,0XCD,0X21,0XCD,0XA1,0XC4,0X22,0XB4,0XC4,0X7A,
0X06,0X3A,0X86,0X29,0X45,0X29,0X05,0X31,0XC4,0X50,0X41,0X90,0X00,0XC0,0X00,0XA8,
0X00,0XA0,0X00,0XA8,0X00,0X30,0X4A,0X4A,0XBA,0XD6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X7D,0XEF,0X8E,0X73,0XC3,0X18,0X05,0X39,0X82,0XA8,0X00,0XF8,
0XC3,0XF8,0X4D,0XFB,0X4D,0XFB,0XC7,0XF9,0XC3,0XF0,0X82,0XD8,0XC3,0XB0,0X04,0X81,
0X45,0X61,0X46,0X51,0X86,0X49,0X86,0X49,0X46,0X41,0X45,0X41,0X45,0X41,0X45,0X41,
0X05,0X49,0X04,0X61,0X82,0X90,0X41,0XB0,0X00,0XD0,0X00,0XC8,0X00,0XA8,0X00,0XA8,
0X00,0XB8,0X41,0X58,0X82,0X10,0X82,0X10,0XB2,0X94,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XBE,0XF7,0XCF,0X7B,0X82,0X10,0X04,0X21,0X86,0X29,0X86,0X41,0X04,0X99,
0X40,0XE8,0X41,0XF8,0X86,0XF9,0XCB,0XFA,0X49,0XFA,0X82,0XF8,0X00,0XF8,0X00,0XF0,
0X00,0XE8,0X41,0XD8,0X41,0XD0,0X41,0XC0,0X41,0XC0,0X41,0XC0,0X41,0XC0,0X41,0XC8,
0X00,0XD0,0X00,0XE0,0X00,0XE0,0X00,0XD8,0X00,0XD0,0X00,0XB8,0X00,0XA8,0X41,0X88,
0X82,0X48,0X82,0X10,0X82,0X10,0X00,0X00,0X45,0X29,0X79,0XCE,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XF3,0X9C,0X82,0X10,0XC3,0X18,0X45,0X29,0X86,0X31,0XC7,0X31,0X30,0X7C,
0XF3,0XDC,0X86,0XE1,0X00,0XF0,0X00,0XF8,0X41,0XF8,0X41,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XE8,0X00,0XE0,0X00,0XE0,0X00,0XD8,0X00,0XC8,0X41,0XA0,0X8A,0X9A,0X0C,0X63,
0X04,0X11,0X82,0X10,0X82,0X10,0X41,0X08,0X00,0X00,0X4D,0X6B,0X7D,0XEF,0XFF,0XFF,
0XFB,0XDE,0X08,0X42,0X42,0X10,0X45,0X29,0X86,0X31,0X86,0X31,0X49,0X4A,0X38,0XBE,
0XFF,0XFF,0X38,0XD6,0X86,0XA9,0X00,0XC8,0X00,0XE0,0X00,0XF0,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF0,0X00,0XF0,
0X00,0XE8,0X00,0XE0,0X00,0XD0,0XC3,0X98,0X8A,0X8A,0XB2,0XA4,0XBA,0XC6,0XF7,0XB5,
0X08,0X42,0X41,0X08,0X82,0X10,0X41,0X08,0X00,0X00,0X45,0X29,0XF7,0XBD,0XFF,0XFF,
0X71,0X8C,0X41,0X08,0X04,0X21,0X45,0X29,0X86,0X31,0X86,0X31,0X0C,0X63,0X3C,0XE7,
0XFF,0XFF,0X79,0XD6,0X46,0XB9,0X00,0XE0,0X42,0XC8,0X82,0XA8,0X82,0XB0,0X41,0XD8,
0X82,0XE8,0X82,0XF0,0X41,0XE8,0X41,0XE8,0X41,0XE8,0X41,0XF0,0X41,0XE8,0X41,0XD8,
0X04,0XC1,0X08,0X92,0X4D,0X8B,0X34,0XA5,0XFB,0XC6,0XFB,0XD6,0XBA,0XCE,0X3C,0XE7,
0X30,0X84,0XC3,0X18,0X41,0X08,0X41,0X08,0X00,0X00,0X41,0X08,0XCF,0X7B,0X7D,0XEF,
0X49,0X4A,0X00,0X00,0X04,0X21,0X45,0X29,0X46,0X31,0X86,0X31,0X30,0X84,0XFF,0XFF,
0XFF,0XF7,0XF7,0XDD,0X09,0XDA,0X83,0XF8,0X01,0XF0,0X42,0XC0,0X82,0X98,0X49,0X9A,
0XF3,0XB4,0XF3,0XCC,0X71,0XBC,0X8E,0XBB,0X8E,0XBB,0X30,0XBC,0X71,0XBC,0XF3,0XBC,
0XB6,0XBD,0XFB,0XCE,0XBE,0XE7,0X7D,0XE7,0X3B,0XDF,0XBA,0XD6,0X79,0XCE,0XFB,0XDE,
0X75,0XAD,0X86,0X31,0X41,0X08,0X41,0X08,0X00,0X00,0X00,0X00,0X49,0X4A,0XFB,0XDE,
0X04,0X21,0X41,0X08,0X04,0X21,0X45,0X29,0X45,0X29,0X87,0X39,0XB2,0X94,0XFF,0XFF,
0XBE,0XF7,0X34,0XDD,0X0C,0XEB,0X09,0XFA,0X00,0XF0,0X01,0XD8,0X00,0XD8,0X8B,0XD2,
0X7D,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XBE,0XFF,0X7D,0XEF,0XFB,0XDE,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0XBA,0XD6,
0X78,0XC6,0XC7,0X39,0X00,0X00,0X41,0X08,0X00,0X00,0X00,0X00,0XC7,0X39,0X79,0XCE,
0X00,0X00,0X82,0X10,0XC3,0X18,0X04,0X21,0X05,0X29,0X86,0X31,0XB3,0X9C,0XFF,0XFF,
0XFF,0XF7,0X75,0XDD,0XC7,0XE9,0XC7,0XF9,0X01,0XF8,0X01,0XF0,0X00,0XE8,0X49,0XE2,
0XFB,0XEE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XBE,0XF7,0X7D,0XEF,0XFB,0XDE,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0XBA,0XD6,
0XB9,0XCE,0X08,0X42,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC7,0X39,0X38,0XC6,
0X00,0X00,0X82,0X10,0X82,0X10,0X04,0X21,0X04,0X21,0X45,0X29,0X30,0X84,0XFF,0XFF,
0XFF,0XFF,0X38,0XDE,0XC4,0XD0,0X00,0XF0,0X01,0XF8,0X00,0XF8,0X00,0XF0,0X08,0XD2,
0XFB,0XE6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XBE,0XF7,0X7D,0XEF,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0X79,0XCE,0XBA,0XD6,
0X79,0XCE,0XC7,0X39,0X41,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X86,0X31,0X38,0XC6,
0X00,0X00,0X00,0X00,0XC3,0X18,0XCB,0X5A,0X86,0X31,0XC3,0X18,0XCB,0X5A,0X7D,0XEF,
0XFF,0XFF,0X7D,0XEF,0XCF,0XBB,0XC3,0XB0,0X41,0XD0,0X41,0XD0,0X82,0XB8,0X4D,0XB3,
0X7D,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XBE,0XF7,0X3D,0XEF,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0X79,0XCE,0XFA,0XD6,
0XF7,0XBD,0X04,0X21,0X86,0X31,0X04,0X21,0X00,0X00,0X00,0X00,0X86,0X31,0X38,0XC6,
0X86,0X31,0XC3,0X18,0XCB,0X5A,0X75,0XAD,0XCF,0X7B,0X41,0X08,0X86,0X31,0XF7,0XBD,
0XFF,0XFF,0XFF,0XFF,0XBE,0XEF,0X74,0XB5,0X30,0X9C,0X30,0X9C,0X72,0XA4,0XBB,0XD6,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0X7D,0XEF,0X3C,0XE7,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0X79,0XCE,0X3C,0XE7,
0X71,0X8C,0X81,0X08,0X0C,0X63,0XCF,0X7B,0X82,0X10,0X00,0X00,0X8A,0X52,0X38,0XC6,
0X75,0XAD,0X71,0X8C,0XB6,0XB5,0X3C,0XE7,0XFB,0XDE,0XC7,0X39,0X00,0X00,0XCF,0X73,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,
0X7D,0XEF,0X7D,0XEF,0X3B,0XDF,0XFA,0XD6,0X79,0XCE,0X79,0XCE,0XFB,0XDE,0XB9,0XCE,
0XC7,0X39,0XC4,0X20,0X71,0X8C,0XBA,0XD6,0X71,0X8C,0XCB,0X5A,0XB2,0X94,0XBA,0XD6,
0XFF,0XFF,0X7D,0XEF,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XB6,0XB5,0X46,0X29,0X05,0X19,
0X75,0XA5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,
0X7D,0XEF,0X3C,0XE7,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0XBA,0XD6,0XFC,0XDE,0X4E,0X63,
0X42,0X08,0X0C,0X63,0XF7,0XBD,0XBE,0XF7,0XFF,0XFF,0XFB,0XDE,0XFB,0XDE,0XBE,0XF7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF4,0X9C,0X04,0X21,
0X05,0X21,0XB6,0XA5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0X7D,0XEF,
0X3C,0XE7,0XFB,0XDE,0XBA,0XD6,0X79,0XCE,0XFB,0XDE,0XBB,0XD6,0XD1,0X73,0X83,0X18,
0X86,0X39,0X34,0X9D,0XBD,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XFF,0X35,0XD6,0XEB,0XCC,0X43,0XB3,
0X40,0X51,0X05,0X19,0XF5,0X8C,0XBE,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XBE,0XF7,0X7D,0XEF,0X7D,0XEF,0X3C,0XE7,
0XFB,0XDE,0XBA,0XDE,0XBA,0XD6,0X3C,0XDF,0X3A,0XBE,0X4F,0X63,0X82,0X49,0X40,0XA3,
0X23,0XB4,0XCC,0X83,0X3A,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XF7,0XB5,0XBD,0X82,0X92,0X20,0XF4,0XA0,0XFC,
0X60,0XE4,0X40,0X82,0X84,0X41,0X8F,0X6B,0X77,0XAD,0X3D,0XE7,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XBE,0XF7,0XBE,0XF7,0XBE,0XF7,0X7D,0XEF,0X7D,0XEF,0X3C,0XE7,0XFB,0XDE,
0XFB,0XDE,0X3D,0XE7,0XBB,0XCE,0X36,0X9D,0X0B,0X6B,0X41,0X6A,0X60,0XC4,0X20,0XFE,
0X60,0XF5,0X00,0X8B,0XC7,0X6A,0X38,0XC6,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0XEF,0X4B,0X7B,0X80,0XB2,0XA0,0XFC,0XA0,0XFC,
0XE0,0XFC,0XE0,0XFC,0XC0,0XCB,0XC1,0X8A,0X45,0X62,0X4D,0X6B,0XB3,0X94,0XF7,0XBD,
0X3D,0XDF,0XFF,0XF7,0XFF,0XFF,0XBE,0XF7,0X7D,0XEF,0X7D,0XEF,0X7D,0XE7,0X3D,0XDF,
0XBA,0XC6,0X75,0XA5,0X8D,0X7B,0X84,0X7A,0X40,0XB3,0XE0,0XEC,0XE0,0XFD,0XE0,0XFD,
0X60,0XF5,0X20,0XE5,0XA0,0XD4,0X0A,0X6B,0XFB,0XDE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7D,0XEF,0XCC,0X93,0X40,0XEB,0X60,0XFC,0XA0,0XFC,
0XE0,0XFC,0X20,0XFD,0X60,0XFD,0X20,0XF5,0XA0,0XD4,0XC0,0XBB,0X42,0X9B,0X45,0X8B,
0X6B,0X9C,0XAE,0X9C,0X71,0X8C,0XB3,0X94,0X33,0X9D,0X34,0XA5,0XF2,0XA4,0XF0,0XB4,
0XCA,0X9B,0X04,0X9B,0X40,0XBB,0X20,0XE4,0X20,0XFD,0XA0,0XFD,0XA0,0XFD,0XE0,0XFD,
0XE0,0XFD,0XE0,0XFD,0X20,0XC4,0X88,0X5A,0X38,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X78,0XD6,0X46,0XAB,0X40,0XDB,0X20,0XF4,
0X60,0XFC,0XA0,0XFC,0XE0,0XFC,0X60,0XFD,0XA0,0XFD,0X60,0XFD,0X20,0XF5,0XA0,0XDC,
0XC0,0XB3,0XC0,0X51,0X86,0X29,0X0D,0X63,0X8F,0X7B,0X0D,0X5B,0XC7,0X41,0X01,0X82,
0X00,0XC3,0XC0,0XE3,0X60,0XFC,0XA0,0XFC,0XE0,0XFC,0XE0,0XFC,0X60,0XF5,0X60,0XF5,
0X20,0XE5,0X80,0X9B,0X86,0X62,0X30,0X84,0X79,0XCE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X38,0XC6,0X2D,0X9C,0X05,0X93,
0X43,0XA3,0X82,0XB3,0XC2,0XBB,0XC2,0XBB,0X22,0XB4,0X82,0XA3,0X42,0X93,0XC3,0X7A,
0X85,0X62,0X0B,0X63,0X71,0X84,0XB6,0XB5,0X79,0XCE,0X79,0XC6,0XB5,0XAD,0X70,0X94,
0X4A,0X8B,0X06,0X83,0X04,0X93,0X04,0X9B,0X43,0X9B,0X43,0X9B,0X43,0X93,0X04,0X83,
0X08,0X73,0X8D,0X73,0XB3,0X94,0X79,0XCE,0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3C,0XDF,0X38,0XBE,
0X75,0XB5,0X33,0XA5,0X33,0XA5,0XF3,0X9C,0XF3,0X9C,0XF3,0X9C,0XF3,0X94,0XF3,0X9C,
0X35,0XA5,0XF8,0XBD,0XFB,0XDE,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7E,0XEF,
0XBB,0XD6,0XF8,0XBD,0XB6,0XAD,0X75,0XAD,0X34,0XA5,0X33,0X9D,0X34,0X9D,0X35,0XA5,
0XB7,0XAD,0X79,0XC6,0X3C,0XE7,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};

const unsigned char hanzi[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0xFE,0xFF,0x01,0x00,0xFE,0xFF,0x01,0x00,0x06,0xC0,0x00,0x00,0x06,0xC0,0x00,0x00,0x06,0xC0,0x00,0x00,0xFE,0xFF,0x00,0x00,0x06,0xC0,0x00,0x00,0x06,0xC0,0x00,0x00,0x06,0xC0,0x00,0x00,0xFE,0xFF,0x00,0x00,0x06,0xC0,0x00,0x00,0x06,0xC0,0x00,0x00,0x02,0x00,0x00,0x30,0x30,0x0C,0x1C,0xF0,0x7F,0xFC,0x3F,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0xF0,0x3F,0xFC,0x1F,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0x30,0x30,0x0C,0x1C,0xF0,0x3F,0xFC,0x1F,0xF0,0x3F,0xFC,0x1F,0x30,0x30,0x0C,0x1C,0x30,0x10,0x04,0x0C,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x08,0x08,0xC0,0xF1,0xFF,0x1F,0xC0,0xF0,0xFF,0x1F,0xC0,0x48,0x4C,0x1C,0xC4,0xDC,0xCC,0x1D,0xCC,0xDC,0x8D,0x1D,0xDC,0x8E,0x8D,0x1D,0xD8,0x06,0x0E,0x1F,0xF8,0xC3,0xEF,0x1D,0xF8,0xF1,0xEC,0x1C,0xD0,0x60,0x1D,0x1C,0xC0,0x1C,0x77,0x0C,0xFE,0x9F,0xE7,0x00,0x34,0x83,0xE3,0x0C,0x30,0xC3,0xFF,0x1F,0x30,0xC3,0x61,0x00,0x30,0xE3,0x61,0x00,0x30,0xE3,0x61,0x06,0x30,0xF3,0xFF,0x0F,0x30,0xDB,0x61,0x00,0x30,0xFB,0x61,0x04,0x38,0xDB,0x61,0x0E,0x18,0xCF,0xFF,0x0F,0x18,0xC7,0x61,0x00,0x98,0xC7,0x61,0x00,0x0C,0xC3,0x61,0x38,0x04,0xC2,0xFF,0x3F,0x06,0xC0,0x01,0x00,0x02,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,
};
const unsigned char asc2_1608[1520]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x18,0x18,0x00,0x00,
0x00,0x48,0x6C,0x24,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x24,0x24,0x24,0x7F,0x12,0x12,0x12,0x7F,0x12,0x12,0x12,0x00,0x00,
0x00,0x00,0x08,0x1C,0x2A,0x2A,0x0A,0x0C,0x18,0x28,0x28,0x2A,0x2A,0x1C,0x08,0x08,
0x00,0x00,0x00,0x22,0x25,0x15,0x15,0x15,0x2A,0x58,0x54,0x54,0x54,0x22,0x00,0x00,
0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0A,0x76,0x25,0x29,0x11,0x91,0x6E,0x00,0x00,
0x00,0x06,0x06,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00,
0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00,
0x00,0x00,0x00,0x00,0x08,0x08,0x6B,0x1C,0x1C,0x6B,0x08,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x7F,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x04,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x00,
0x00,0x00,0x80,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x00,
0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
0x00,0x00,0x00,0x08,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x20,0x20,0x10,0x08,0x04,0x42,0x7E,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x20,0x18,0x20,0x40,0x40,0x42,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x20,0x30,0x28,0x24,0x24,0x22,0x22,0x7E,0x20,0x20,0x78,0x00,0x00,
0x00,0x00,0x00,0x7E,0x02,0x02,0x02,0x1A,0x26,0x40,0x40,0x42,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x38,0x24,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
0x00,0x00,0x00,0x7E,0x22,0x22,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x64,0x58,0x40,0x40,0x24,0x1C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x04,
0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00,
0x00,0x00,0x00,0x3C,0x42,0x42,0x46,0x40,0x20,0x10,0x10,0x00,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x1C,0x22,0x5A,0x55,0x55,0x55,0x55,0x2D,0x42,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x08,0x08,0x18,0x14,0x14,0x24,0x3C,0x22,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x1E,0x22,0x42,0x42,0x42,0x22,0x1F,0x00,0x00,
0x00,0x00,0x00,0x7C,0x42,0x42,0x01,0x01,0x01,0x01,0x01,0x42,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x1F,0x22,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1F,0x00,0x00,
0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00,
0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x02,0x02,0x07,0x00,0x00,
0x00,0x00,0x00,0x3C,0x22,0x22,0x01,0x01,0x01,0x71,0x21,0x22,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00,
0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11,0x0F,
0x00,0x00,0x00,0x77,0x22,0x12,0x0A,0x0E,0x0A,0x12,0x12,0x22,0x22,0x77,0x00,0x00,
0x00,0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x7F,0x00,0x00,
0x00,0x00,0x00,0x77,0x36,0x36,0x36,0x36,0x2A,0x2A,0x2A,0x2A,0x2A,0x6B,0x00,0x00,
0x00,0x00,0x00,0xE3,0x46,0x46,0x4A,0x4A,0x52,0x52,0x52,0x62,0x62,0x47,0x00,0x00,
0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00,
0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x42,0x3E,0x02,0x02,0x02,0x02,0x07,0x00,0x00,
0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x4D,0x53,0x32,0x1C,0x60,0x00,
0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x3E,0x12,0x12,0x22,0x22,0x42,0xC7,0x00,0x00,
0x00,0x00,0x00,0x7C,0x42,0x42,0x02,0x04,0x18,0x20,0x40,0x42,0x42,0x3E,0x00,0x00,
0x00,0x00,0x00,0x7F,0x49,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x42,0x22,0x24,0x24,0x14,0x14,0x18,0x08,0x08,0x00,0x00,
0x00,0x00,0x00,0x6B,0x49,0x49,0x49,0x49,0x55,0x55,0x36,0x22,0x22,0x22,0x00,0x00,
0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00,
0x00,0x00,0x00,0x7E,0x21,0x20,0x10,0x10,0x08,0x04,0x04,0x42,0x42,0x3F,0x00,0x00,
0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00,
0x00,0x00,0x02,0x02,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x40,0x40,
0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00,
0x00,0x38,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0x00,0x06,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x78,0x44,0x42,0x42,0xFC,0x00,0x00,
0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x02,0x02,0x02,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0x60,0x40,0x40,0x40,0x78,0x44,0x42,0x42,0x42,0x64,0xD8,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x02,0x02,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0xF0,0x88,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x22,0x22,0x1C,0x02,0x3C,0x42,0x42,0x3C,
0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00,
0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x38,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x1E,
0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x72,0x12,0x0A,0x16,0x12,0x22,0x77,0x00,0x00,
0x00,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x92,0x92,0x92,0x92,0x92,0xB7,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x26,0x42,0x42,0x42,0x22,0x1E,0x02,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x40,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x4C,0x04,0x04,0x04,0x04,0x1F,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x42,0x02,0x3C,0x40,0x42,0x3E,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x3E,0x08,0x08,0x08,0x08,0x08,0x30,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x14,0x08,0x08,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEB,0x49,0x49,0x55,0x55,0x22,0x22,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x24,0x18,0x18,0x18,0x24,0x6E,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x14,0x18,0x08,0x08,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x22,0x10,0x08,0x08,0x44,0x7E,0x00,0x00,
0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0xC0,0x00,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
0x00,0x06,0x08,0x08,0x08,0x08,0x08,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x06,0x00,
0x0C,0x32,0xC2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

#endif