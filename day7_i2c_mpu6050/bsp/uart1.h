#ifndef __UART1_H_
#define __UART1_H_

#include "stm32f10x.h"

#include "stdio.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

void uart1_init(void);
void NVIC_uart1_Configuration(void);

#endif
