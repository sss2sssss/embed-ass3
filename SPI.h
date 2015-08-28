/********************** (C) COPYRIGHT 2015 CHONG HON FONG****************************
*
* @file     SPI.h
* @brief    Functions for serial peripheral interface
*
* @author   Chong Hon Fong
* @date     25/07/2015
*
*************************************************************************************/

#ifndef SPI_H
#define SPI_H

#define SPI_ON      SPI1STATbits.SPIEN = 1
#define SPI_OFF     SPI1STATbits.SPIEN = 0

// Global Function
void PPS_Unlock(void);
void PPS_Lock(void);

void SPI1_Init(void);
void SPI1_SendReceiveData8(unsigned char data);

#endif