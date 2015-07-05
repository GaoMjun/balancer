#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "stm32f10x.h"

void encoder_init(void);
int32_t get_left_encoder_num(void);
int32_t get_right_encoder_num(void);
void NVIC_encoder_Configuration(void);



#endif
