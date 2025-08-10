//Write an ECP to demonstrate External interrupt 0 in LPC2129?
#include <LPC21xx.h>

#include "types.h"
#include "pin_cfg.h"
#include "pin_cfg_defines.h"
#include "defines.h"

#define EINT3_PIN      20 //select the one port pin which will give support for EINT0
#define EINT3_VIC_CHNO 17	  //assign the EINT0 channel number
#define EINT3_LED      19   	//toggle one LED for every interrupt, so select one port pin

//ISR SYNTAX: void ISR_NAME(void) __irq
void eint0_isr(void) __irq
{
	//toggle EINT_LED
	CPLBIT(IOPIN0,EINT3_LED);	
	//clear eint0 status (through EXTINT SFR)
	EXTINT = 1<<0;
	//clear eint0 status in VIC (through VICVectAddr )
	VICVectAddr = 0;
}	

void Enable_EINT0(void)
{
	//configure P0.1/P0.16 as EINT0 input pin
	CfgPortPin(PORT0,EINT3_PIN,PIN_FUNC4);
	
	//configure VIC (Vector Interrupt controller)
	//def External interrupts (EINT0/EINT1/EINT2/EINT3) is IRQ types (by using VICIntSelect SFR)
	
	//enable EINT0 via VIC (by using VICIntEnable SFR)
	VICIntEnable = 1<<EINT3_VIC_CHNO;
	//enable vectored irq slot 0 for EINT0 (0-15 based on requirement – by using VICVectCntl0 SFR)
	VICVectCntl0 = (1<<5) | (EINT3_VIC_CHNO);
	//load isr address into slot 0 (0-15 based on requirement – by using VICVectAddr0 SFR)
	VICVectAddr0 = (u32)eint0_isr;
	
	//configure External Interrupts Peripheral
	//configure EINT0/EINT1/EINT2/EINT3 as edge triggered (use EXTMODE SFR)
   EXTMODE = 1<<0;
	//def EINT0/EINT1/EINT2/EINT3 is Falling Edge Triggerd (use EXTPOLAR SFR)	
}




