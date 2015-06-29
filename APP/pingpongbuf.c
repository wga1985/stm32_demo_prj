#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

#define PINGPONG_BUFSIZE	(20)
static unsigned char pingpongbuf[2][PINGPONG_BUFSIZE];
//static unsigned char pingpongbuf0[PINGPONG_BUFSIZE];
//static unsigned char pingpongbuf1[PINGPONG_BUFSIZE];

typedef enum{PINGPONG_BUF_NO0 = 0, PINGPONG_BUF_NO1 = 1}PINGPONG_BUF_NO;
static PINGPONG_BUF_NO PingPongBuf_Free_No = PINGPONG_BUF_NO0;
static bool PINGPONG_BUF_OK;

#define DMA_Circular	1

/*-------------------------------------------------------------------------------------------------------*/

void pingpong_buf_init(void)
{
	PingPongBuf_Free_No = PINGPONG_BUF_NO1;
	PINGPONG_BUF_OK = FALSE;
}

static void usart1_dma_tx_done_callback(void)
{
	if(!dma1_ch4_mode_is_circular())
	{
		USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
		dma1_ch4_stop();
	}
	
	//printf("\r\nusart1_dma_tx_done_callback\r\n");
}

void pingpong_usart1_rx_dma_int(void)
{
	if(!dma1_ch5_mode_is_circular())
	{
		USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
		DMA_Cmd(DMA1_Channel5, DISABLE);
	}
	
#if DMA_Circular
	if(PINGPONG_BUF_NO0 == PingPongBuf_Free_No)
	{
		DMA1_Channel5->CMAR = (unsigned int)&pingpongbuf[0]; // DMA_MemoryBaseAddr;
		PingPongBuf_Free_No = PINGPONG_BUF_NO1;
		//usart1_dma_write(pingpongbuf[1], PINGPONG_BUFSIZE, usart1_dma_tx_done_callback);
	}
	else
	{
		DMA1_Channel5->CMAR = (unsigned int)pingpongbuf[1]; // DMA_MemoryBaseAddr;
		PingPongBuf_Free_No = PINGPONG_BUF_NO0;
		//usart1_dma_write(pingpongbuf[1], PINGPONG_BUFSIZE, usart1_dma_tx_done_callback);
	}
	PINGPONG_BUF_OK = TRUE;
#else // DMA Mode Normal
	if(PINGPONG_BUF_NO0 == PingPongBuf_Free_No)
	{
		PingPongBuf_Free_No = PINGPONG_BUF_NO1;
		//usart1_dma_write(pingpongbuf[1], PINGPONG_BUFSIZE, usart1_dma_tx_done_callback);
		#if 1
		DMA1_Channel5->CMAR = (unsigned int)pingpongbuf[0]; // DMA_MemoryBaseAddr;
		DMA1_Channel5->CNDTR = PINGPONG_BUFSIZE; // DMA_BufferSize;
		DMA_Cmd(DMA1_Channel5, ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		#else
		dma1_ch5_peri2mem_start((unsigned char *)(&USART1->DR), pingpongbuf[0], PINGPONG_BUFSIZE);
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		#endif
		
	}
	else
	{
		PingPongBuf_Free_No = PINGPONG_BUF_NO0;
		//usart1_dma_write(pingpongbuf[0], PINGPONG_BUFSIZE, usart1_dma_tx_done_callback);
		#if 1
		DMA1_Channel5->CMAR = (unsigned int)pingpongbuf[1]; // DMA_MemoryBaseAddr;
		DMA1_Channel5->CNDTR = PINGPONG_BUFSIZE; // DMA_BufferSize;
		DMA_Cmd(DMA1_Channel5, ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		#else
		dma1_ch5_peri2mem_start((unsigned char *)(&USART1->DR), pingpongbuf[1], PINGPONG_BUFSIZE);
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		#endif		
	}
	PINGPONG_BUF_OK = TRUE;
#endif
}

static void pingpong_dma1_ch5_init(void)
{
#if DMA_Circular
	dma1_ch5_init();
	usart1_dma_circular_read(pingpongbuf[0], PINGPONG_BUFSIZE, pingpong_usart1_rx_dma_int);
	pingpong_buf_init();
#else // DMA Mode Normal
	dma1_ch5_init();

	dma1_ch5_wait_busy();
	dma1_ch5_lock();
	dma1_ch5_tranfer_done_cb_register(pingpong_usart1_rx_dma_int);
	dma1_ch5_transfer_mode(DMA_Mode_Normal);
	dma1_ch5_peri2mem_start((unsigned char *)(&USART1->DR), pingpongbuf[0], PINGPONG_BUFSIZE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	pingpong_buf_init();
#endif
}

void pingpong_buf_test(void)
{
	dma1_ch4_init();
	pingpong_dma1_ch5_init();
		
	while(1)
	{
		if(PINGPONG_BUF_OK == TRUE)
		{
			PINGPONG_BUF_OK = FALSE;
			if(PINGPONG_BUF_NO0 == PingPongBuf_Free_No)
			{
				//usart1_write(pingpongbuf0, PINGPONG_BUFSIZE);
				printf("11 - %s\r\n", pingpongbuf[0]);
			}
			else
			{
				//usart1_write(pingpongbuf1, PINGPONG_BUFSIZE);
				printf("22 - %s\r\n", pingpongbuf[1]);
			}
		}
	}
}

