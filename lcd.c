//lcd.c
#include "types.h"
#include <LPC21xx.h>
#include "lcd_defines.h"
#include "defines.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
	//select write operation using rw pin
	SCLRBIT(IOCLR0,LCD_RW);
	//write onto data pins any byte (cmd/ascii/value)
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide to high to low transition on enable pin
	//EN high
	SSETBIT(IOSET0,LCD_EN);
	//>=450ns
	delay_us(1);
	//EN low
	SCLRBIT(IOCLR0,LCD_EN);
	//delay bt/w consective bytes
	delay_ms(2);
}

void CmdLCD(u8 cmd)
{
	//select cmd/ins reg 
	SCLRBIT(IOCLR0,LCD_RS);	
	//and write cmd into it
	WriteLCD(cmd);
}

void InitLCD(void)
{
  //cfg gpio pins connected to LCD_DATA pins as output pins
  WRITEBYTE(IODIR0,LCD_DATA,0XFF);
  //cfg gpio pin connected to LCD_RS as output pin
  SETBIT(IODIR0,LCD_RS);
  //cfg gpio pin connected to LCD_RW as output pin
  SETBIT(IODIR0,LCD_RW);
	//cfg gpio pin connected to LCD_EN as output pin
  SETBIT(IODIR0,LCD_EN);
	//wait for 15ms delay after power on/up
	delay_ms(15);
	//function set cmd
	CmdLCD(0x30);
	//wait for >=4.1ms
	delay_ms(1);
	//function set cmd
	CmdLCD(0x30);
	//wait for >=100us delay
	delay_us(100);
	//function set cmd
	CmdLCD(0x30);
	//establish 8 -bit mode cmd with 2 line
	CmdLCD(MODE_8BIT_2LINE);	//CmdLCD(0x38);
	//Disp + cursor cmd as per user requirement
	CmdLCD(DSP_ON_CUR_BLINK);	//CmdLCD(0x0F);
	//clear screen/dsp cmd
	CmdLCD(CLEAR_LCD);	//CmdLCD(0x01);
	//entry mode/move cursor direction
	CmdLCD(SHIFT_CUR_RIGHT);	//CmdLCD(0x06);
}

void CharLCD(u8 asciiVal)
{
	//select data reg 
	SSETBIT(IOSET0,LCD_RS);	
	//& write asciiVal to ddram via data reg
	WriteLCD(asciiVal);
}	

void SetCursor(u8 lineNo,u8 pos)
{
	if(lineNo==1)
		CmdLCD(GOTO_LINE1_POS0+pos);
	else if(lineNo==2)
		CmdLCD(GOTO_LINE2_POS0+pos);
}	

void StrLCD(s8 *str)
{
	while(*str)
		CharLCD(*str++);
}

//function to display +ve integer number on lcd
void U32LCD(u32 num)
{
	u8 a[10];
	s32 i=0;
	if(num==0)
	{
		//display 0 on lcd
		CharLCD('0');
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
			CharLCD(a[i]);	
	}
}

//function to display -ve integer number on lcd
void S32LCD(s32 num)
{
	if(num<0)
	{
		//display '-' on LCD
		CharLCD('-');		
		//change the sign of the given number
		num=-num;		
	}
	//display updated number after sign change
	U32LCD(num);	
}

void F32LCD(f32 fNum,u32 nDP)
{
	s32 iNum;
	if(fNum<0.0)
	{
		//display '-' on LCD
		CharLCD('-');		
		//change the sign of the given number
		fNum = -fNum;	
	}
	//assign float number to integer variable	
	iNum = fNum;	
	//display integer value on LCD
	U32LCD(iNum);
	//display '.' on LCD
	CharLCD('.');
	//extract digit by digit after decimal point and display it on LCD up to given no.of digits after decimal point
	while(nDP)
	{
		fNum = (fNum-iNum)*10;
		iNum = fNum;
		CharLCD(iNum+0x30);
		nDP--;
	}
}

//to store the user defined character bytes into CGRAM locations
void BuildCGRAM(u8 *p,u8 num)
{
	u32 i;
	//Goto CGRAM start
	CmdLCD(0x40);	
	for(i=0;i<num;i++)
	{
		//write into cgram via data reg
		CharLCD(p[i]);		
	}
	//come back to ddram
	CmdLCD(0x80);
}

