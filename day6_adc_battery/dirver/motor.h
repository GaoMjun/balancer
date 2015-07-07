#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f10x.h"

void motor_stop(void);
void motor_init(void);
void set_motor_speed(uint16_t left_motor, uint16_t right_motor);


#endif
