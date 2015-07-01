#include "led.h"

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void led_on(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

void led_off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}




















