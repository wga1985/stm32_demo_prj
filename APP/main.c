#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------*/

int main(void)
{
	BSP_Init();

	//usart1_dma_test();
	pingpong_buf_test();
	
	while(1)
	{
		if(IS_TIMEOUT_NMS(TimerEvId_ID0, 500))
		{
			LED1_TOGGLE;
			LED2_TOGGLE;
		}

		if(IS_TIMEOUT_NMS(TimerEvId_ID1, 3000))
		{
			
		}

		//delay_ms(100);
		//printf("------main_loop------\r\n");
	}
	
	return 0;
}

