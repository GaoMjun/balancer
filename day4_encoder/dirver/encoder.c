#include "encoder.h"

void timer2_init(void);
void timer4_init(void);
void NVIC_timer2_Configuration(void);
void NVIC_timer4_Configuration(void);


void timer2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//1.config pin
// 	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//2.timer config 
// 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //65535
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//3.encoder config 
// 	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM2, 
							TIM_EncoderMode_TI12,
							TIM_ICPolarity_Rising,
							TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//4.claer all pending interrupt
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	//5.reset counter
	TIM2->CNT = 0x0;

	TIM_Cmd(TIM2,ENABLE);
}

void timer4_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//1.config pin
// 	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//2.timer config 
// 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //65535
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//3.encoder config 
// 	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_EncoderInterfaceConfig(TIM4, 
							TIM_EncoderMode_TI12,
							TIM_ICPolarity_Rising,
							TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	//4.claer all pending interrupt
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

	//5.reset counter
	TIM4->CNT = 0x0;

	TIM_Cmd(TIM4,ENABLE);
}

void encoder_init(void)
{
	timer2_init();
	timer4_init();
}


int32_t get_left_encoder_num(void)
{
	return TIM_GetCounter(TIM2);
}


int32_t get_right_encoder_num(void)
{
	return TIM_GetCounter(TIM4);
}

void NVIC_timer2_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void NVIC_timer4_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void NVIC_encoder_Configuration(void)
{
		NVIC_timer2_Configuration();
		NVIC_timer4_Configuration();
}

