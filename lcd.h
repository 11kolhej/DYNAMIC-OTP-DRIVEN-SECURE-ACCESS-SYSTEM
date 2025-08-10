//lcd.h
#ifndef _LCD_H
#define _LCD_H

#include "types.h"
void WriteLCD(u8 byte);
void CmdLCD(u8 cmd);
void InitLCD(void);
void CharLCD(u8 asciiVal);
void StrLCD(s8 *);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32,u32);
void SetCursor(u8,u8);
void BuildCGRAM(u8 *p,u8);

#endif
