#ifndef __CONSOLA_H_
#define __CONSOLA_H_

#include "stm32f10x.h"

typedef enum 
{ 
	FAILED = 0, 
	PASSED = !FAILED
} TestStatus;

TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

#endif
