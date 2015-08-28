# MPLAB IDE generated this makefile for use with GNU make.
# Project: Assign3.mcp
# Date: Mon Aug 24 22:37:30 2015

AS = xc16-as.exe
CC = xc16-gcc.exe
LD = xc16-ld.exe
AR = xc16-ar.exe
HX = xc16-bin2hex.exe
RM = rm

Assign3.hex : Assign3.cof
	$(HX) "Assign3.cof" -omf=coff

Assign3.cof : IOCon.o main.o System.o UART.o Wifi.o ADC.o
	$(CC) -omf=coff -mcpu=33FJ128MC802 "IOCon.o" "main.o" "System.o" "UART.o" "Wifi.o" "ADC.o" -o"Assign3.cof" -Wl,-L"C:\Program Files (x86)\Microchip\xc16\v1.24\lib",-Tp33FJ128MC802.gld,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,--defsym=__ICD2RAM=1,-Map="Assign3.map",--report-mem

IOCon.o : IOCon.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h IOCon.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "IOCon.c" -o"IOCon.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

main.o : ADC.h Wifi.h Param.h UART.h IOCon.h System.h c:/program\ files\ (x86)/microchip/xc16/v1.24/include/stddef.h c:/program\ files\ (x86)/microchip/xc16/v1.24/include/stdlib.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/libpic30.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h main.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "main.c" -o"main.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

System.o : System.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h System.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "System.c" -o"System.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

UART.o : IOCon.h UART.h c:/program\ files\ (x86)/microchip/xc16/v1.24/include/stddef.h c:/program\ files\ (x86)/microchip/xc16/v1.24/include/stdlib.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/libpic30.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h UART.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "UART.c" -o"UART.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

Wifi.o : Wifi.h UART.h Param.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/libpic30.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h Wifi.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "Wifi.c" -o"Wifi.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

ADC.o : Param.h ADC.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/dsPIC33F/h/p33FJ128MC802.h c:/program\ files\ (x86)/microchip/xc16/v1.24/support/generic/h/xc.h ADC.c
	$(CC) -omf=coff -mcpu=33FJ128MC802 -x c -c "ADC.c" -o"ADC.o" -I"C:\Program Files (x86)\Microchip\xc16\v1.24\inc" -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -g -Wall

clean : 
	$(RM) "IOCon.o" "main.o" "System.o" "UART.o" "Wifi.o" "ADC.o" "Assign3.cof" "Assign3.hex"

