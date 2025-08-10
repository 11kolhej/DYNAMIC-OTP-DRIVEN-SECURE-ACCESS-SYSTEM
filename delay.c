//delay.c
#include "types.h"
void delay_us(u32 dlyUS)
{
	//N *~1us @CCLK=60MHz
	for(dlyUS*=12;dlyUS>0;dlyUS--);
}

void delay_ms(u32 dlyMS)
{
	//N * ~1ms @CCLK=60MHz
	for(dlyMS*=12000;dlyMS>0;dlyMS--);
}

void delay_s(u32 dlyS)
{
	//N * ~1s  @CCLK=60MHz
	for(dlyS*=12000000;dlyS>0;dlyS--);
}
