#include <xc.h>
#include "Param.h"
#include "Timer1.h"

unsigned int Cycle1=0;
void LED_Blinking();

void Timer1_Init(void)
{
    Timer1_OFF;
    T1CONbits.TSIDL = 0;		// Continue module operation in Idle mode
    T1CONbits.TSYNC = 0;		// do not synchonize internal clock
    T1CONbits.TGATE = 0;		// 0 for normal, 1 for gated
    T1CONbits.TCS = 0;			// 0 for timer, 1 for Asynchronous counter
    T1CONbits.TCKPS = 0b01;		// Select 1:8 Prescaler
    PR1 = 4606;					// 10mS

    IPC0bits.T1IP = 0b001; 		// Set Timer1 Interrupt Priority Level 1
    TMR1 = 0x00; 				// Clear timer register
    IFS0bits.T1IF = 0; 			// Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1; 			// Enable Timer1 interrupt
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;		//reset timer1 interrupt flag to 0
	LED_Blinking();
}

void LED_Blinking()
{
	//Heartbeat always blink at 1 Hz
	Cycle1++;
	if(Cycle1<=50)
		LED1=1;
	else if (Cycle1>50 && Cycle1<=100)
		LED1=0;
	else
		Cycle1=0;	
}