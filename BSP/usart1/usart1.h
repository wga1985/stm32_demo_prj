#ifndef _USART1_H
#define _USART1_H

/*-------------------------------------------------------------------------------------------------------*/

/* USART1 */
#define USART1_GPIO_TX		GPIO_Pin_9
#define USART1_GPIO_RX		GPIO_Pin_10
#define USART1_GPIO			GPIOA

/*-------------------------------------------------------------------------------------------------------*/

typedef enum
{
	BR_1200 = 1200,
	BR_4800 =4800,
	BR_9600 = 9600,
	BR_14400 = 14400,
	BR_19200 = 19200,
	BR_38400 = 38400,
	BR_56000 = 56000,
	BR_57600 = 57600,
	BR_76800 = 76800,	
	BR_115200 = 115200,
	BR_128000 = 128000,
	BR_256000 = 256000,
}USART_BAUDRATE;

/*-------------------------------------------------------------------------------------------------------*/

typedef struct _tag_usart_err
{
	unsigned int ore_err;
	unsigned int noise_err;
	unsigned int frame_err;
	unsigned int parity_err;
}T_UartErr;

/*-------------------------------------------------------------------------------------------------------*/

/* 定义usart1接收回调函数指针*/
typedef void (*Usart1RxCbFunc)(unsigned char *buf, unsigned int len);

int usart1_rx_callback_regiser(Usart1RxCbFunc callbackfunc);
int usart1_rx_callback_unregiser(Usart1RxCbFunc callbackfunc);

/*-------------------------------------------------------------------------------------------------------*/

void usart1_init(int Baudrate);
void usart1_open(void);
void usart1_close(void);
unsigned int usart1_write(unsigned char *src, unsigned int len);
unsigned int usart1_read(unsigned char *dest, unsigned int len);

#endif /* _USART1_H */
