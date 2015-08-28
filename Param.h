#ifndef PARAM_H
#define	PARAM_H

//define your pin name at here, example
//#define    LED1    LATBbits.LATB10
//#define    LED2    LATBbits.LATB11

#define FOSC    (7370000ULL)
#define FCY     (FOSC/2)

#define		LED1		LATAbits.LATA1
#define		LED2		LATBbits.LATB15
#define 	LCD_CS      LATBbits.LATB5
#define 	LCD_REST    LATBbits.LATB6
#define 	LCD_DC      LATBbits.LATB7
#define 	LCD_SCK     PORTBbits.RB9

#define 	DCLK        LATBbits.LATB10
#define		CS          LATBbits.LATB11
#define 	DIN         LATBbits.LATB12
#define 	DOUT        PORTBbits.RB13
#define 	Penirq      PORTBbits.RB14

#define 	BIT(x,n)    (((x) >> (n)) & 1)
#define 	bit7        BIT(bitdata, 7)
#define 	bit6        BIT(bitdata, 6)
#define 	bit5        BIT(bitdata, 5)
#define 	bit4        BIT(bitdata, 4)
#define 	bit3        BIT(bitdata, 3)
#define 	bit2        BIT(bitdata, 2)
#define 	bit1        BIT(bitdata, 1)
#define 	bit0        BIT(bitdata, 0)

extern unsigned int temp_out;

#endif