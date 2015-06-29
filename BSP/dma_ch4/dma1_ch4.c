#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

static volatile unsigned char dma1_ch4_spinlock = 0;
static unsigned char dma1_ch4_err = 0;
static DMA1Ch4TransferCb DMA1_CH4_Done_Callback;
static DMA1Ch4TransferCb DMA1_CH4_Half_Callback;

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch4_lock(void)
{
	dma1_ch4_spinlock = 1;
}

void dma1_ch4_unlock(void)
{
	dma1_ch4_spinlock = 0;
}

void dma1_ch4_wait_busy(void)
{
	while(dma1_ch4_spinlock != 0);
}

void dma1_ch4_tranfer_half_cb_register(DMA1Ch4TransferCb callback)
{
	if(callback != NULL)
	{
		DMA1_CH4_Half_Callback = callback;
		DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE); /* 半传输中断 */
		printf("dma1_ch4_tranfer_half_cb_register success.\r\n");
	}
}

void dma1_ch4_tranfer_half_cb_unregister(void)
{
	DMA1_CH4_Half_Callback = NULL;
	DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,DISABLE);
	printf("dma1_ch4_tranfer_half_cb_unregister success.\r\n");
}

void dma1_ch4_tranfer_done_cb_register(DMA1Ch4TransferCb callback)
{
	if(callback != NULL)
	{
		DMA1_CH4_Done_Callback = callback;
		printf("dma1_ch4_tranfer_done_cb_register success.\r\n");
	}
}

void dma1_ch4_tranfer_done_cb_unregister(void)
{
	DMA1_CH4_Done_Callback = NULL;
	printf("dma1_ch4_tranfer_done_cb_unregister success.\r\n");
}

unsigned char dma1_ch4_is_occur_err(void)
{
	return dma1_ch4_err;
}

void dma1_ch4_clear_err(void)
{
	dma1_ch4_err = 0;
}

/*-------------------------------------------------------------------------------------------------------*/

/* DMA 传输模式: DMA_Mode_Normal / DMA_Mode_Circular */
void dma1_ch4_transfer_mode(unsigned int mode)
{
	if(mode == DMA_Mode_Circular){
		DMA1_Channel4->CCR |= mode;
	}else{
		DMA1_Channel4->CCR &= (~DMA_Mode_Circular);
	}
}

unsigned char dma1_ch4_mode_is_circular(void)
{
	return (DMA1_Channel4->CCR & DMA_Mode_Circular);
}

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch4_peri2mem_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel4->CCR &= (~DMA_DIR_PeripheralDST); // DMA_DIR_PeripheralSRC
	DMA1_Channel4->CCR &= (~DMA_PeripheralInc_Enable); // DMA_PeripheralInc_Disable
	DMA1_Channel4->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel4->CCR &= (~DMA_M2M_Enable); // DMA_M2M_Disable
	
	DMA1_Channel4->CPAR = (unsigned int)src; // DMA_PeripheralBaseAddr;
	DMA1_Channel4->CMAR = (unsigned int)dst; // DMA_MemoryBaseAddr;
	DMA1_Channel4->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE); /* 半传输中断  */
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE); /* 传输完成中断*/
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE); /* 错误中断*/
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void dma1_ch4_mem2peri_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel4->CCR |= DMA_DIR_PeripheralDST;
	DMA1_Channel4->CCR &= (~DMA_PeripheralInc_Enable); // DMA_PeripheralInc_Disable
	DMA1_Channel4->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel4->CCR &= (~DMA_M2M_Enable); // DMA_M2M_Disable
	
	DMA1_Channel4->CPAR = (unsigned int)dst; // DMA_PeripheralBaseAddr;
	DMA1_Channel4->CMAR = (unsigned int)src; // DMA_MemoryBaseAddr;
	DMA1_Channel4->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE); /* 半传输中断  */
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE); /* 传输完成中断*/
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE); /* 错误中断*/
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void dma1_ch4_mem2mem_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel4->CCR |= DMA_DIR_PeripheralDST;
	DMA1_Channel4->CCR |= DMA_PeripheralInc_Enable;
	DMA1_Channel4->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel4->CCR |= DMA_M2M_Enable;
	DMA1_Channel4->CCR &= (~DMA_Mode_Circular);
	
	DMA1_Channel4->CPAR = (unsigned int)dst; // DMA_PeripheralBaseAddr;
	DMA1_Channel4->CMAR = (unsigned int)src; // DMA_MemoryBaseAddr;
	DMA1_Channel4->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE); /* 半传输中断  */
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE); /* 传输完成中断*/
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE); /* 错误中断*/
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void dma1_ch4_stop(void)
{
	DMA_Cmd(DMA1_Channel4, DISABLE);
#if 0
	DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,DISABLE);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,DISABLE);

	DMA1_Channel4->CPAR = (unsigned int)0; // DMA_PeripheralBaseAddr;
	DMA1_Channel4->CMAR = (unsigned int)0; // DMA_MemoryBaseAddr;
	DMA1_Channel4->CNDTR = 0; // DMA_BufferSize;
#endif
	dma1_ch4_unlock();
}

/*-------------------------------------------------------------------------------------------------------*/

static void dma1_ch4_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,DISABLE); /* 半传输中断  */
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE); /* 传输完成中断*/
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,DISABLE); /* 错误中断*/
}

void dma1_ch4_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* 开启DMA时钟*/ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)0; /* DMA 外设地址 */
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)0; /* DMA 内存地址*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; /* DMA 传输方向: 内存-> 外设*/
	DMA_InitStructure.DMA_BufferSize = 0; /* DMA 传输长度 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; /* DMA 外设地址固定 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /* DMA 内存地址递增 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* DMA 外设传输单元: Byte */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; /* DMA 内存传输单元: Byte */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* DMA 传输模式: Normal / Circular */
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; /* DMA 优先级: 中 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; /* DMA 传输类型: Peripheral - Mem */
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	dma1_ch4_nvic_config();
}

/*-------------------------------------------------------------------------------------------------------*/

static void __dma1_ch4_err_handle(void)
{
	dma1_ch4_stop();
	dma1_ch4_err++;
	
	printf("\r\n__dma1_ch4_err\r\n");
}

static void __dma1_ch4_tranfer_done(void)
{
	dma1_ch4_unlock();	
	if(DMA1_CH4_Done_Callback != NULL) DMA1_CH4_Done_Callback();
	
	//printf("\r\n__dma1_ch4_tranfer_done\r\n");
}

static void __dma1_ch4_half_done(void)
{
	if(DMA1_CH4_Half_Callback != NULL) DMA1_CH4_Half_Callback();
	
	//printf("\r\n__dma1_ch4_half_done\r\n");
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TE4) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TE4);
		__dma1_ch4_err_handle();
	}
	else if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		__dma1_ch4_tranfer_done();		
	}
	else if(DMA_GetITStatus(DMA1_IT_HT4) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_HT4);
		__dma1_ch4_half_done();
	}	

	DMA_ClearITPendingBit(DMA1_IT_GL4); // clear global interrupt flag
}

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch4_memcpy(unsigned char *src,unsigned char *dst,unsigned int len, DMA1Ch4TransferCb callback)
{
	dma1_ch4_wait_busy();
	dma1_ch4_lock();
	
	dma1_ch4_tranfer_done_cb_register(callback);
	dma1_ch4_transfer_mode(DMA_Mode_Normal);
	dma1_ch4_mem2mem_start(src, dst, len);
}

