#ifndef _UART0_H_
#define _UART0_H_

#include "types.h"

//declare all required function declarations
void UART0_Init(void);
void UART0_TxChar(char ch);
char UART0_RxChar(void);
void UART0_TxString(char *str);
void U32UART0(u32 num);
void S32UART0(s32 num);
void F32UART0(f32 fNum,u32 nDP);

#endif
