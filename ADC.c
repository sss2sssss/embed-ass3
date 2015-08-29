#include <xc.h>
#include "Param.h"
#include <libpic30.h>
#include "ADC.h"

unsigned int ADCValue=0, temp_out=0;
volatile unsigned int *ADCPtr;

void ADC_Init(void)
{
	ADC_OFF;						//turn off ADC module
	AD1PCFGL = 0xFE;				//bandgap voltage ref disable, AN0 in analog mode for ADC sampling
	
	AD1CON1bits.ADSIDL=0;			//ADC runs even in idle mode
	AD1CON1bits.AD12B=1;			//12-bits
	AD1CON1bits.FORM=0b00;			//output data in unsigned integer format
	AD1CON1bits.SSRC=0b111;			//Internal counter ends sampling and starts conversion (auto-convert)
	AD1CON1bits.ASAM=0;				//sampling begins when SAMP bit = 1
	AD1CON1bits.SAMP=0;				//run sampling when 1
	AD1CON1bits.DONE=0;				//clear sampling Done status bit
	
	AD1CHS0bits.CH0NA=0;			//Vref- = Channel 0 negative input 
	AD1CHS0bits.CH0SA=0b00000;		//AN0  = channel 0 positive input

	AD1CON3bits.ADRC=0;				//use system clock for ADC clock
	AD1CON3bits.SAMC=0b11110; 		//Auto-Sample Time = 30 Tad
	AD1CON3bits.ADCS=0b00000100;	//Tad = 5 Tcy

	AD1CSSL=0;						//all input disable for scanning

	AD1CON2bits.VCFG=0b000;			//AVDD and AVSS are Vref+ and Vref- (3.3V)
	AD1CON2bits.CSCNA=0;			//disable input scanning
	AD1CON2bits.SMPI=0b0000;		//Set AD1IF after 1 samples
	AD1CON2bits.BUFM=0;				//buffer config as one 16-word buffer
	AD1CON2bits.ALTS=0;				//always use MUX A input multiplexer setting
}

void ADC_Convert()
{
	ADCValue = 0;                         //reset ADC value
	ADCPtr = (unsigned int *)&ADC1BUF0;   // initialize ADC1BUF pointer
	IFS0bits.AD1IF = 0;                   // clear ADC interrupt flag
	AD1CON1bits.ASAM = 1;                 //run sampling indefinitely
	while (!IFS0bits.AD1IF){};            //ADC module only proceed when ADC interrupt is triggered
	AD1CON1bits.ASAM = 0;                 //stop sampling
	ADCValue = *ADCPtr;
	int counter=0;
	for(counter=0;counter<=3;counter++)	  //Green LED will blink two time to indicate
	{									  //Complete of ADC
		LED2=~LED2;
		__delay_ms(250);
	}

	temp_out = ((ADCValue/4095.0)*(3.3*100.0));	//temperature in degree centigrade as 10mv=1degree
}