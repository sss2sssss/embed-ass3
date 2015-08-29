#include <xc.h>
#include "Param.h"
#include <libpic30.h>
#include "UART.h"
#include "Wifi.h"
#include "ADC.h"
#include "Param.h"


void WifiReset(void)		//Reset the WiFi module
{
	WriteStringUART1("AT+RST\r\n");
	__delay_ms(10000);
}

void ConnectAP(void)		//Set WiFi module in AP mode, then connect to the desire Access Point
{
	WriteStringUART1("AT+CWMODE=1\r\n");
	__delay_ms(10000);
	WriteStringUART1("AT+CWJAP=\"testwifi\",\"sls930622\"\r\n");
	__delay_ms(10000);
}

void TCPServer(void)		//Set up multi connection mode, set up TCP server with port 8713
{
	WriteStringUART1("AT+CIPMUX=1\r\n");
	__delay_ms(10000);
	WriteStringUART1("AT+CIPSERVER=1,8713\r\n");
	__delay_ms(10000);
}

void StartConnect(void)		//Temperature sensor start working, then WiFi module start sending the comment to the server
{
	ADC_Convert();
	if(temp_out<18)		//If temperature is lower than 18 degree celcuis
	{
		WriteStringUART1("AT+CIPSEND=0,30\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Cold");
		__delay_ms(30000);
	}
	else if(temp_out>=18 && temp_out<=24)		//If temperature is between 18 to 24 degree celcuis
	{
		WriteStringUART1("AT+CIPSEND=0,32\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Normal");
		__delay_ms(30000);
	}
	else if(temp_out>24)		//If temperature is higher than 24 degree celcuis
	{
		WriteStringUART1("AT+CIPSEND=0,29\r\n");
		__delay_ms(5000);
		WriteStringUART1("Now is ");
		WriteUART1dec2string(temp_out);
		WriteStringUART1(" degree celcuis. Hot");
		__delay_ms(30000);
	}
}