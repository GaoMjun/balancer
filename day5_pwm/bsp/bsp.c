#include "stm32f10x.h"

void bsp_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |
												 RCC_APB1Periph_TIM3 |
												 RCC_APB1Periph_TIM4 |
												 RCC_APB1Periph_USART2 |
												 RCC_APB1Periph_USART3 |
												 RCC_APB1Periph_I2C1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
												 RCC_APB2Periph_GPIOB |
												 RCC_APB2Periph_AFIO |
												 RCC_APB2Periph_USART1 |
												 RCC_APB2Periph_SPI1 |
												 RCC_APB2Periph_ADC1, ENABLE);
}


