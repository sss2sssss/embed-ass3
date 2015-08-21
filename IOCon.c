#include <xc.h>
#include "IOCon.h"

void IO_Init()
{
	//Assign you pin config at here, example:
    //TRISBbits.TRISB10 = 0;				// LED1 as output @ RB10/pin21
	RPOR1bits.RP3R = 0b00011;				// U1TX
	RPINR18bits.U1RXR = 0b00100;			// U1RX
	TRISBbits.TRISB3 = 0;					// U1TX
	TRISBbits.TRISB4 = 1;					// U1RX

	TRISAbits.TRISA0 = 1;
}