#include "stm32f10x.h"

#include <stdio.h>

#include "bsp.h"
#include "led.h"
#include "delay.h"

int main(void)
{
	SystemInit();
	bsp_init();
	led_init();
	delay_init();
	
	while(1)
	{
		led_on();
		DelayMs(500);
		led_off();
		DelayMs(500);
		printf("Hello World!\n\r");
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

