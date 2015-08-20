#ifndef	UART_H
#define UART_H

void UART1_Init(void);
void WriteUART1(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void WriteStringUART1(const char * s);

extern int tempRX;

#endif

