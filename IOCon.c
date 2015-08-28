#include <xc.h>
#include "IOCon.h"

void IO_Init()
{
	//Assign you pin config at here, example:
    //TRISBbits.TRISB10 = 0;				// Anything
	RPOR2bits.RP4R = 0b00011;				// U1TX
	RPINR18bits.U1RXR = 0b00011;			// U1RX
	TRISBbits.TRISB4 = 0;					// U1TX
	TRISBbits.TRISB3 = 1;					// U1RX

    RPOR4bits.RP8R = 0b00111;       		// SD01
    RPOR4bits.RP9R = 0b01000;       		// SCK1OUT
    TRISBbits.TRISB8 = 0;           		// SD01
    TRISBbits.TRISB9 = 0;           		// SCK1OUT

    RPINR22bits.SCK2R = 0b01010;			// SCK2IN
    RPINR23bits.SS2R = 0b01011;				// SSI2
    TRISBbits.TRISB10 = 1;          		// SCK2IN
    TRISBbits.TRISB11 = 1;          		// SSI2

	TRISAbits.TRISA0 = 1;					// ADC

	TRISAbits.TRISA1 = 0;					// LED1
    TRISBbits.TRISB15 = 0;					// LED2

    TRISBbits.TRISB5 = 0;                   // LCD_CS
	TRISBbits.TRISB6 = 0;                   // LCD_REST
    TRISBbits.TRISB7 = 0;                   // LCD_DC

    TRISBbits.TRISB10 = 0;                  // DCLK
    TRISBbits.TRISB11 = 0;                  // CS
    TRISBbits.TRISB12 = 0;                  // DIN
    TRISBbits.TRISB13 = 1;                  // DOUT
    TRISBbits.TRISB14 = 1;                  // IRQ
}