#include <xc.h>
#include "Param.h"
#include <libpic30.h>
#include <stdlib.h>
#include "UART.h"

int tempRX;

void UART1_Init()
{	
	//U1BRG Baud rate
	U1BRG				= 23;			//9600 
	
	UART_OFF;							//Disable UART
	U1MODEbits.USIDL	= 0;			//Continue module operation in Idle mode
	U1MODEbits.IREN		= 0;			//IrDA encoder and decoder disabled
	U1MODEbits.RTSMD	= 0;			//UxRTS pin in Flow Control mode
	U1MODEbits.UEN		= 0b00;			//UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by port latches
	U1MODEbits.WAKE		= 0;			//No wake-up enabled
	U1MODEbits.LPBACK	= 0;			//Loopback mode is disabled
	U1MODEbits.ABAUD	= 0;			//Baud rate measurement disabled or completed
	U1MODEbits.URXINV	= 0;			
	U1MODEbits.BRGH		= 0;			//BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
	U1MODEbits.PDSEL	= 0b00;			//8-bit data, no parity
	U1MODEbits.STSEL	= 0;			//One Stop bit	
	
	//U1STA
	U1STAbits.UTXISEL0	= 0;			//Interrupt when a character is transferred to the Transmit Shift Register (this implies there is at least one character open in the transmit buffer)
	U1STAbits.UTXISEL1	= 0;
	U1STAbits.UTXINV	= 0;			
	U1STAbits.UTXBRK	= 0;			//Sync Break transmission disabled or completed
	U1STAbits.UTXEN		= 0;			//Transmit disabled, UxTX pin controlled by PORTx
	U1STAbits.URXISEL	= 0b11;			
	U1STAbits.ADDEN		= 0;			//Address Detect mode disabled
	
	//UART1 Interrupt Priority Configuration
	IPC3bits.U1TXIP		= 0b010;		//UART1 Transmitter Interrupt Priority bits
	IEC0bits.U1TXIE		= 1;			//UART1 Transmitter Interrupt Enable bit
	IFS0bits.U1TXIF		= 0;			//Clear UART1 Transmitter Interrupt
	
	IPC2bits.U1RXIP		= 0b010;		//UART1 Receiver Interrupt Priority bits
	IEC0bits.U1RXIE		= 1;			//UART1 Receiver Interrupt Enable bit
	IFS0bits.U1RXIF		= 0;			//Clear UART1 Receiver Interrupt
}


void WriteUART1(unsigned int data)		//Write ASCII byte by byte
{
    while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 0b00)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}
void WriteUART1dec2string(unsigned int data)	//Write integer value by convert them to ASCII
{
	char buffer [3];
	itoa(buffer,data,10);
	WriteStringUART1(buffer);
}
void WriteStringUART1(const char * s)		//Write String to the target
{
    while(*s)
	{
    	WriteUART1(*s++);
	}
}

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void)
{  
  	IFS0bits.U1TXIF = 0;
} 

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF = 0;
	tempRX = U1RXREG;
}