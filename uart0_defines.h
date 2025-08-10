#ifndef _UART0_DEFINES_H_
#define _UART0_DEFINES_H_

/*
U0LCR, U0LSR, U0DLL, U0DLM, U0THR, U0RBR - SFR's used for UART polling method

The UART0 Divisor Latch is part of the UART0 Baud Rate Generator and holds the value used to divide the clock in order to produce the baud rate clock, which must be 16x the desired baud rate.

DIVISOR = PCLK / (16 * BAUDRATE)

DIVISOR = (U0DLM*256) + U0DLL;
- DIVISOR LOWER BYTE NEED TO LOAD IN TO U0DLL
- DIVISOR HIGHER BYTE NEED TO LOAD IN TO U0DLM
*/

//define required macros for baudrate calculation

#define FOSC			12000000 
#define CCLK 			(5*FOSC)
#define PCLK 			(CCLK/4)
#define BAUDRATE	9600
#define DIVISOR (PCLK / (16 * BAUDRATE))

//U0LCR 
#define BIT_5			0x00
#define BIT_6			0x01
#define BIT_7			0x02
#define BIT_8 		0x03
#define WORD_LENGTH_SELECT BIT_8 
#define DLAB_BIT	7	

//U0LSR
#define RDR_BIT		0
#define THRE_BIT	5		
#define TEMT_BIT	6	

#define TXD0 0
#define RXD0 1

#endif
