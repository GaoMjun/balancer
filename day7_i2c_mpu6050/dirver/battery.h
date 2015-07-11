#ifndef __BATTERY_H_
#define __BATTERY_H_

#include "stm32f10x.h"

void adc1_init(void);
void NVIC_adc1_Configuration(void);
uint16_t get_battery_voltage(void);


#endif
