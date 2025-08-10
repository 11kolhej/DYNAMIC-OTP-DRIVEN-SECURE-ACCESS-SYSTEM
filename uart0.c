#include <lpc214x.h>

#include "delay.h"
#include "types.h"
#include "defines.h"
#include "pin_cfg.h"
#include "pin_cfg_defines.h"
#include "uart0_defines.h"

// Initialize Serial Interface (for UART0)
void UART0_Init(void) 
{
  // Configure P0.0 as TXD0 and P0.1 as RXD0
  // Select UART0 function for P0.0 and P0.1
	CfgPortPin(PORT0,TXD0,PIN_FUNC2);
	CfgPortPin(PORT0,RXD0,PIN_FUNC2);

	//configure UART0 peripheral
  // configure UART0 for 8-bit data, 1 stop bit, no parity, enable DLAB using U0LCR SFR
	WRITEBYTE(U0LCR,0,(WORD_LENGTH_SELECT|(1<<DLAB_BIT)));
  // Set the low byte of the divisor value using U0DLL SFR
	WRITEBYTE(U0DLL,0,(DIVISOR&0xFF));
  // Set the high byte of the divisor value using U0DLM SFR
	WRITEBYTE(U0DLM,0,((DIVISOR>>8)&0xFF));
  // Disable DLAB using U0LCR SFR
	CLRBIT(U0LCR,DLAB_BIT);	
}

//Write character to Serial Port  
void UART0_TxChar(char ch) 
{
	// Transmit ch variable value use U0THR SFR
	U0THR = ch;
  // Wait until U0THR and the U0TSR are empty (use U0LSR SFR)   
	while(READBIT(U0LSR,TEMT_BIT) == 0);
}

// Read character from Serial Port
char UART0_RxChar(void) 
{
  // Wait until U0RBR register contains valid data (use U0LSR SFR)
	while(READBIT(U0LSR,RDR_BIT) == 0);
  // collect received character through U0RBR SFR value and return 
	return U0RBR;
}

//Write string to Serial Port  
void UART0_TxString(char *str) 
{
	while(*str)
		UART0_TxChar(*str++);
}


//Write +ve integer value to  Serial Port  
void U32UART0(u32 num)
{
		u8 a[10];
	s32 i=0;
	if(num==0)
	{
		//display 0 on lcd
		UART0_TxChar('0');
	}
	else
	{
		//extract digit by digit and store it into array
		while(num)
		{
			a[i++] = (num%10)+48;
			num = num/10;
		}
		//display digit by digit on LCD 
		for(--i;i>=0;i--)
			UART0_TxChar(a[i]);	
	}


}

//Write -ve integer value to  Serial Port  
void S32UART0(s32 num)
{
	if(num<0)
	{
		//display '-' on LCD
		UART0_TxChar('-');		
		//change the sign of the given number
		num=-num;		
	}
	//display updated number after sign change
	U32UART0(num);
}

//Write float value to  Serial Port  
void F32UART0(f32 fNum,u32 nDP)
{
	s32 iNum;
	if(fNum<0.0)
	{
		//display '-' on LCD
		UART0_TxChar('-');		
		//change the sign of the given number
		fNum = -fNum;	
	}
	//assign float number to integer variable	
	iNum = fNum;	
	//display integer value on LCD
	U32UART0(iNum);
	//display '.' on LCD
	UART0_TxChar('.');
	//extract digit by digit after decimal point and display it on LCD up to given no.of digits after decimal point
	while(nDP)
	{
		fNum = (fNum-iNum)*10;
		iNum = fNum;
		UART0_TxChar(iNum+0x30);
		nDP--;
	}
}

