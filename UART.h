#ifndef	UART_H
#define UART_H

#define UART_ON			U1MODEbits.UARTEN	= 1
#define UART_OFF		U1MODEbits.UARTEN	= 0
#define UART_TRANS		U1STAbits.UTXEN		= 1

void UART1_Init(void);
void WriteUART1(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void WriteStringUART1(const char * s);

extern int tempRX;

#endif

