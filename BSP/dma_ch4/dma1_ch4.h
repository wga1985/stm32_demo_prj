#ifndef _DMA_CH4_H
#define _DMA_CH4_H

/*-------------------------------------------------------------------------------------------------------*/

typedef void (*DMA1Ch4TransferCb)(void);

/*-------------------------------------------------------------------------------------------------------*/

void dma1_ch4_lock(void);
void dma1_ch4_unlock(void);
void dma1_ch4_wait_busy(void);
void dma1_ch4_tranfer_half_cb_register(DMA1Ch4TransferCb callback);
void dma1_ch4_tranfer_half_cb_unregister(void);
void dma1_ch4_tranfer_done_cb_register(DMA1Ch4TransferCb callback);
void dma1_ch4_tranfer_done_cb_unregister(void);
unsigned char dma1_ch4_is_occur_err(void);
void dma1_ch4_clear_err(void);

void dma1_ch4_init(void);
void dma1_ch4_transfer_mode(unsigned int mode);
unsigned char dma1_ch4_mode_is_circular(void);
void dma1_ch4_peri2mem_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch4_mem2peri_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch4_mem2mem_start(unsigned char *src, unsigned char *dst, unsigned int len);
void dma1_ch4_stop(void);

void dma1_ch4_memcpy(unsigned char *src,unsigned char *dst,unsigned int len, DMA1Ch4TransferCb callback);

#endif /* _DMA_CH4_H */

