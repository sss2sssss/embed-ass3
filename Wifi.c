#include <xc.h>
#include "Param.h"
#include <libpic30.h>
#include "UART.h"
#include "Wifi.h"
#include "ADC.h"
#include "Param.h"


void WifiReset(void)
{
	WriteStringUART1("AT+RST\r\n");
	__delay_ms(10000);
}

void ConnectAP(void)
{
	WriteStringUART1("AT+CWMODE=1\r\n");
	__delay_ms(10000);
	WriteStringUART1("AT+CWJAP=\"testwifi\",\"sls930622\"\r\n");
	__delay_ms(10000);
}

void TCPServer(void)
{
	WriteStringUART1("AT+CIPMUX=1\r\n");
	__delay_ms(10000);
	WriteStringUART1("AT+CIPSERVER=1,8713\r\n");
	__delay_ms(10000);
}

void StartConnect(void)
{
	ADC_Convert();
	if(temp_out<18)
	{
		WriteStringUART1("AT+CIPSEND=0,30\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Cold");
		__delay_ms(30000);
	}
	else if(temp_out>=18 && temp_out<=24)
	{
		WriteStringUART1("AT+CIPSEND=0,32\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Normal");
		__delay_ms(30000);
	}
	else if(temp_out>24)
	{
		WriteStringUART1("AT+CIPSEND=0,29\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Hot");
		__delay_ms(30000);
	}
}