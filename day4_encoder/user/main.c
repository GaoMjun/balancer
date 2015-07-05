#include "stm32f10x.h"

#include <stdio.h>
#include <string.h>

#include "bsp.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"
#include "encoder.h"

#include "consola.h"

char Uart1_Buffer[0xFF] = "0";
char command1[0xFF] = "led on";
char command2[0xFF] = "led off";
uint16_t Uart1_BufferSize = 0;
uint8_t i = 0;
uint8_t commandEnd = 0;

char test[0xFF];
int main(void)
{
	SystemInit();
	bsp_init();
	led_init();
	delay_init();
	uart1_init();
	encoder_init();
	
	NVIC_uart1_Configuration();
	NVIC_encoder_Configuration();
	
	while(1)
	{
		led_on();
		DelayMs(500);
		led_off();
		DelayMs(500);
		
		if(commandEnd)
		{
			commandEnd = 0;
			
			Uart1_Buffer[Uart1_BufferSize-1] = '\0';
			if(0 == strcmp(Uart1_Buffer, command1))
				led_on();
			else if(!strcmp(Uart1_Buffer, command2))
				led_off();
			else
				printf("unknow command \r\n");
			
			Uart1_BufferSize = 0;
		}		
		printf("left:%d    right:%d \r\n", get_left_encoder_num(), get_right_encoder_num());
	}
}
	
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

