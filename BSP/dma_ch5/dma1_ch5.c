#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

static volatile unsigned char dma1_ch5_spinlock = 0;
static unsigned char dma1_ch5_err = 0;
static DMA1Ch5TransferCb DMA1_CH5_Done_Callback;
static DMA1Ch5TransferCb DMA1_CH5_Half_Callback;

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch5_lock(void)
{
	dma1_ch5_spinlock = 1;
}

void dma1_ch5_unlock(void)
{
	dma1_ch5_spinlock = 0;
}

void dma1_ch5_wait_busy(void)
{
	while(dma1_ch5_spinlock != 0);
}

void dma1_ch5_tranfer_half_cb_register(DMA1Ch5TransferCb callback)
{
	if(callback != NULL)
	{
		DMA1_CH5_Half_Callback = callback;
		DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,ENABLE); /* �봫���ж� */
		printf("dma1_ch5_tranfer_half_cb_register success.\r\n");
	}
}

void dma1_ch5_tranfer_half_cb_unregister(void)
{
	DMA1_CH5_Half_Callback = NULL;
	DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,DISABLE);
	printf("dma1_ch5_tranfer_half_cb_unregister success.\r\n");
}

void dma1_ch5_tranfer_done_cb_register(DMA1Ch5TransferCb callback)
{
	if(callback != NULL)
	{
		DMA1_CH5_Done_Callback = callback;
		printf("dma1_ch5_tranfer_done_cb_register success.\r\n");
	}
}

void dma1_ch5_tranfer_done_cb_unregister(void)
{
	DMA1_CH5_Done_Callback = NULL;
	printf("dma1_ch5_tranfer_done_cb_unregister success.\r\n");
}

unsigned char dma1_ch5_is_occur_err(void)
{
	return dma1_ch5_err;
}

void dma1_ch5_clear_err(void)
{
	dma1_ch5_err = 0;
}

/*-------------------------------------------------------------------------------------------------------*/

/* DMA ����ģʽ: DMA_Mode_Normal / DMA_Mode_Circular */
void dma1_ch5_transfer_mode(unsigned int mode)
{
	if(mode == DMA_Mode_Circular){
		DMA1_Channel5->CCR |= mode;
	}else{
		DMA1_Channel5->CCR &= (~DMA_Mode_Circular);
	}
}

unsigned char dma1_ch5_mode_is_circular(void)
{
	return (DMA1_Channel5->CCR & DMA_Mode_Circular);
}

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch5_peri2mem_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel5->CCR &= (~DMA_DIR_PeripheralDST); // DMA_DIR_PeripheralSRC
	DMA1_Channel5->CCR &= (~DMA_PeripheralInc_Enable); // DMA_PeripheralInc_Disable
	DMA1_Channel5->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel5->CCR &= (~DMA_M2M_Enable); // DMA_M2M_Disable
	
	DMA1_Channel5->CPAR = (unsigned int)src; // DMA_PeripheralBaseAddr;
	DMA1_Channel5->CMAR = (unsigned int)dst; // DMA_MemoryBaseAddr;
	DMA1_Channel5->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,ENABLE); /* �봫���ж�  */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE); /* ��������ж�*/
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,ENABLE); /* �����ж�*/
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void dma1_ch5_mem2peri_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel5->CCR |= DMA_DIR_PeripheralDST;
	DMA1_Channel5->CCR &= (~DMA_PeripheralInc_Enable); // DMA_PeripheralInc_Disable
	DMA1_Channel5->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel5->CCR &= (~DMA_M2M_Enable); // DMA_M2M_Disable
	
	DMA1_Channel5->CPAR = (unsigned int)dst; // DMA_PeripheralBaseAddr;
	DMA1_Channel5->CMAR = (unsigned int)src; // DMA_MemoryBaseAddr;
	DMA1_Channel5->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,ENABLE); /* �봫���ж�  */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE); /* ��������ж�*/
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,ENABLE); /* �����ж�*/
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void dma1_ch5_mem2mem_start(unsigned char *src, unsigned char *dst, unsigned int len)
{
	DMA1_Channel5->CCR |= DMA_DIR_PeripheralDST;
	DMA1_Channel5->CCR |= DMA_PeripheralInc_Enable;
	DMA1_Channel5->CCR |= DMA_MemoryInc_Enable;
	DMA1_Channel5->CCR |= DMA_M2M_Enable;
	DMA1_Channel5->CCR &= (~DMA_Mode_Circular);
	
	DMA1_Channel5->CPAR = (unsigned int)dst; // DMA_PeripheralBaseAddr;
	DMA1_Channel5->CMAR = (unsigned int)src; // DMA_MemoryBaseAddr;
	DMA1_Channel5->CNDTR = len; // DMA_BufferSize;

	//DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,ENABLE); /* �봫���ж�  */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE); /* ��������ж�*/
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,ENABLE); /* �����ж�*/
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void dma1_ch5_stop(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);
#if 0
	DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,DISABLE);
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE);
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,DISABLE);

	DMA1_Channel5->CPAR = (unsigned int)0; // DMA_PeripheralBaseAddr;
	DMA1_Channel5->CMAR = (unsigned int)0; // DMA_MemoryBaseAddr;
	DMA1_Channel5->CNDTR = 0; // DMA_BufferSize;
#endif
	dma1_ch5_unlock();
}

/*-------------------------------------------------------------------------------------------------------*/

static void dma1_ch5_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,DISABLE); /* �봫���ж�  */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE); /* ��������ж�*/
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,DISABLE); /* �����ж�*/
}

void dma1_ch5_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* ����DMAʱ��*/ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)0; /* DMA �����ַ */
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)0; /* DMA �ڴ��ַ*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; /* DMA ���䷽��: �ڴ�-> ����*/
	DMA_InitStructure.DMA_BufferSize = 0; /* DMA ���䳤�� */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; /* DMA �����ַ�̶� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /* DMA �ڴ��ַ���� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* DMA ���贫�䵥Ԫ: Byte */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; /* DMA �ڴ洫�䵥Ԫ: Byte */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* DMA ����ģʽ: Normal / Circular */
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; /* DMA ���ȼ�: �� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; /* DMA ��������: Peripheral - Mem */
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	dma1_ch5_nvic_config();
}

/*-------------------------------------------------------------------------------------------------------*/

static void __dma1_ch5_err_handle(void)
{
	dma1_ch5_stop();
	dma1_ch5_err++;
	
	printf("\r\n__dma1_ch5_err\r\n");
}

static void __dma1_ch5_tranfer_done(void)
{
	dma1_ch5_unlock();	
	if(DMA1_CH5_Done_Callback != NULL) DMA1_CH5_Done_Callback();
	
	//printf("\r\n__dma1_ch5_tranfer_done\r\n");
}

static void __dma1_ch5_half_done(void)
{
	if(DMA1_CH5_Half_Callback != NULL) DMA1_CH5_Half_Callback();
	
	//printf("\r\n__dma1_ch5_half_done\r\n");
}

void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TE5) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TE5);
		__dma1_ch5_err_handle();
	}
	else if(DMA_GetITStatus(DMA1_IT_TC5) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		__dma1_ch5_tranfer_done();		
	}
	else if(DMA_GetITStatus(DMA1_IT_HT5) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_HT5);
		__dma1_ch5_half_done();
	}	

	DMA_ClearITPendingBit(DMA1_IT_GL5); // clear global interrupt flag
}

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch5_memcpy(unsigned char *src,unsigned char *dst,unsigned int len, DMA1Ch5TransferCb callback)
{
	dma1_ch5_wait_busy();
	dma1_ch5_lock();
	
	dma1_ch5_tranfer_done_cb_register(callback);
	dma1_ch5_transfer_mode(DMA_Mode_Normal);
	dma1_ch5_mem2mem_start(src, dst, len);
}

