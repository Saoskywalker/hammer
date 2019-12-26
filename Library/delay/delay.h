#ifndef __DELAY_H
#define __DELAY_H 

#include "N76E003_BSP.h"

void Timer0_Delay100us(UINT32 u32CNT);
void Timer0_Delay1ms(UINT32 u32CNT);
void Timer1_Delay10ms(UINT32 u32CNT);
void Timer2_Delay500us(UINT32 u32CNT);
void Timer3_Delay100ms(UINT32 u32CNT);
void Timer0_Delay40ms(UINT32 u32CNT);
void Timer3_Delay10us(UINT32 u32CNT);
void Timer3_Delay1us(UINT32 u32CNT);
void Timer3_Delay1ms(UINT32 u32CNT);

// void delay_init(void);
#define delay_ms(nms) Timer3_Delay1ms(nms)
#define delay_us(nus) Timer3_Delay1us(nus)

#endif
