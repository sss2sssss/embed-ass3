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

int main()
{
	UART1_Init();
	__delay_ms(5);
	IO_Init();
	MCU_Init();
	ADC_Init();
	ADC_ON;
	//WriteStringUART1("AT+RST\r");


	while(1)
	{
		
	}
}

