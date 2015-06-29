#ifndef _DMA_CH5_H
#define _DMA_CH5_H

/*-------------------------------------------------------------------------------------------------------*/

typedef void (*DMA1Ch5TransferCb)(void);

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch5_lock(void);
void dma1_ch5_unlock(void);
void dma1_ch5_wait_busy(void);
void dma1_ch5_tranfer_half_cb_register(DMA1Ch5TransferCb callback);
void dma1_ch5_tranfer_half_cb_unregister(void);
void dma1_ch5_tranfer_done_cb_register(DMA1Ch5TransferCb callback);
void dma1_ch5_tranfer_done_cb_unregister(void);
unsigned char dma1_ch5_is_occur_err(void);
void dma1_ch5_clear_err(void);

void dma1_ch5_init(void);
void dma1_ch5_transfer_mode(unsigned int mode);
unsigned char dma1_ch5_mode_is_circular(void);
void dma1_ch5_peri2mem_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch5_mem2peri_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch5_mem2mem_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch5_stop(void);

void dma1_ch5_memcpy(unsigned char *src,unsigned char *dst,unsigned int len, DMA1Ch5TransferCb callback);

#endif /* _DMA_CH5_H */

