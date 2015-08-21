 #define FOSC    (7370000ULL)
 #define FCY     (FOSC/2)

#include <xc.h>
#include <libpic30.h>
#include "System.h"
#include "IOCon.h"
#include "UART.h"
#include "Param.h"
#include "Wifi.h"
#include "ADC.h"

_FOSCSEL(FNOSC_FRC & IESO_OFF)
_FOSC(OSCIOFNC_ON & IOL1WAY_ON & FCKSM_CSDCMD)
_FWDT(FWDTEN_OFF)
_FICD(JTAGEN_OFF)

int mode=1;

int main()
{
	UART1_Init();
	__delay_ms(1);
	IO_Init();
	MCU_Init();
	ADC_Init();
	//ADC_ON;


	while(1)
	{
		switch(mode)
		{
			case 1:
				WriteStringUART1("AT+RST\r\n");
				__delay_ms(5000);
				mode=2;
			case 2:
				WriteStringUART1("AT+CWMODE=1\r\n");
				__delay_ms(5000);
				mode=3;
				break;
			case 3:
				WriteStringUART1("AT+CWJAP=\"wifi_zombie\",\"sls930622\"\r\n");
				//WriteStringUART1("AT+CWJAP=\"onlyforgaming\",\"gnimagrofylno\"\r\n");
				__delay_ms(5000);
				mode=4;
				break;
			case 4:
				__delay_ms(5);
				break;
		}
	}
}

