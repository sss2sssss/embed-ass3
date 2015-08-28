/********************** (C) COPYRIGHT 2015 CHONG HON FONG****************************
*
* @file     SPI.c
* @brief    Functions for serial peripheral interface
*
* @author   Chong Hon Fong
* @date     25/07/2015
*
*************************************************************************************/

/* Includes ------------------------------------------------------------------------*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Param.h"
#include <libpic30.h>
#include "SPI.h"


void PPS_Unlock(void)
{
    _IOLOCK = 0;          // Disable IOLOCK
}

void PPS_Lock(void)
{
    _IOLOCK = 1;          // Enable IOLOCK
}

void SPI1_Init(void)
{
    // SPI 1 Configurations
    SPI_OFF;         // Set SPI1 OFF
    SPI1STATbits.SPISIDL = 0;       // Continue module operation in Idle mode
    SPI1CON1bits.DISSCK = 0;        // Enable internal SPI clock
    SPI1CON1bits.DISSDO = 0;        // Set SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16 = 0;        // Set communication is byte-wide (8 bits)
    SPI1CON1bits.SMP = 1;           // Set input data sampled at end of data output time
    SPI1CON1bits.CKE = 1;           // Set serial output data changes on transition from active clock state to idle clock state
    SPI1CON1bits.SSEN = 0;          // SS1 pin is not used by the module. Pin controlled by port function
    SPI1CON1bits.CKP = 0;           // Idle state for clock is a low level; active state is a high level
    SPI1CON1bits.MSTEN = 1;         // Set Master mode
    SPI1CON1bits.SPRE = 0b101;      // Secondary prescale 4:1
    SPI1CON1bits.PPRE = 0b11;       // Primary prescale 1:1
    SPI1CON2bits.FRMEN = 0;         // Disable SPI1 frame support
    
    // SPI Clock Speed, Fsck = Fcy/(Primary prescale * Secondary prescale)

    SPI1STATbits.SPIROV = 0;        // Clear SPI1 Receive Overflow Flag
    IFS0bits.SPI1IF = 0;            // Clear SPI1 Interrupt Flag
    IEC0bits.SPI1IE = 0;            // Disable SPI1 Interrupt
    IPC2bits.SPI1IP = 0b100;        // Set SPI1 Interrupt Priority = 4
}

void SPI1_SendReceiveData8(unsigned char data)
{
    while(LCD_SCK==1);
    SPI1BUF = data;
    while(!SPI1STATbits.SPIRBF);
    data = SPI1BUF;
}