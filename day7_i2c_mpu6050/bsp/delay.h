#ifndef __DELAY_H_
#define __DELAY_H_ 

#include "stm32f10x.h"

void delay_init(void);
void DelayUs(__IO u32 nTime);
void DelayMs(__IO u32 nTime);
void TimingDelay_Decrement(void);
void get_ms(unsigned long *time);

#endif

