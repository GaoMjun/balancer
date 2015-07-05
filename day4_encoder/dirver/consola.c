#include "consola.h"

TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
	uint8_t i = 0;
	for(i = 0; i <= BufferLength; i++)
	{
			if(pBuffer1[i] == pBuffer2[i])
				return PASSED;
			else
				return FAILED;
	}
}

