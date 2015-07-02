#include "stm32f10x.h"

#include "delay.h"

static __IO u32 TimingDelay;

void delay_init(void)
{
	if(SysTick_Config(SystemCoreClock / 1000000))
	{
		while(1);
	}
}

void DelayUs(__IO u32 nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

void DelayMs(__IO u32 nTime)
{
	DelayUs(nTime * 1000);
}

void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
		{
			TimingDelay--;
		}
}


