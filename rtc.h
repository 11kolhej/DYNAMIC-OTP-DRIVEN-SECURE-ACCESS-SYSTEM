#ifndef _RTC_H
#define _RTC_H

#include "types.h"

void RTC_Init(void);

void RTCGetTime(u32 *,u32 *,u32 *);
void DisplayRTCTime(u32,u32,u32);

void RTCGetDate(u32 *,u32 *,u32 *);
void DisplayRTCDate(u32,u32,u32);

void RTCGetDay(u32 *);
void DisplayRTCDay(u32);

void RTCSetTime(u32,u32,u32);
void RTCSetDate(u32,u32,u32);
void RTCSetDay(u32);


#endif
