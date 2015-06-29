#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

T_UartErr Uart1Err = {0};

#define RX_BUF_SIZE	(128)
static unsigned int uart1_index = 0;
static unsigned char uart1_rxbuf[RX_BUF_SIZE] = {0};

static Usart1RxCbFunc Usart1RxCallbackFun = NULL;

/*-------------------------------------------------------------------------------------------------------*/

int usart1_rx_callback_regiser(Usart1RxCbFunc func)
{
	if(Usart1RxCallbackFun == NULL)
	{
		Usart1RxCallbackFun = func;
		printf("Usart1RxCbFuncRegister Success.\r\n");
		return 0;
	}

	return -1;
}

int usart1_rx_callback_unregiser(Usart1RxCbFunc func)
{
	if(Usart1RxCallbackFun != NULL)
	{
		Usart1RxCallbackFun = NULL;
		printf("Usart1RxCbFuncUnregister Success.\r\n");
		return 0;
	}
	return -1;
}

static void __usart1_rx_finish(unsigned char *buf, unsigned int len)
{
	printf("USART1 Receive len: %d, buf: %s\r\n", len, buf);

	if(Usart1RxCallbackFun != NULL)
	{
		Usart1RxCallbackFun(buf, len);
	}
	uart1_index = 0;
	memset(uart1_rxbuf, 0, RX_BUF_SIZE);
}

/*-------------------------------------------------------------------------------------------------------*/

static void __usart1_rx_irq_func(void)
{
	unsigned char rxchar;
	rxchar = USART_ReceiveData(USART1);
	uart1_rxbuf[uart1_index++] = rxchar;
}

static void __usart1_err_handle(void)
{
	unsigned char rxchar;
	rxchar = USART_ReceiveData(USART1);
	printf("usart1 err char: %d\r\n", rxchar);
}

/*-------------------------------------------------------------------------------------------------------*/

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{

		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);

		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		// tranfer finish
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		__usart1_rx_irq_func();
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	/* 置位TE将使得USART在第一个数据帧前发送一空闲帧 */
	/* 空闲帧是在数据传输过程中插入一个间隔 , 用于发送大于1个字符的间隔中断*/
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{				
		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		USART_ReceiveData(USART1);//空读清状态
		__usart1_rx_finish(uart1_rxbuf, uart1_index);
		//printf("usart1 idle frame!\r\n");
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
	{
		Uart1Err.ore_err++;
		__usart1_err_handle();
		printf("usart1 overflow err!\r\n");
		USART_ClearITPendingBit(USART1, USART_IT_ORE);
	}
	if(USART_GetITStatus(USART1, USART_IT_NE) != RESET)
	{
		Uart1Err.noise_err++;
		__usart1_err_handle();
		printf("usart1 noise err!\r\n");
		USART_ClearITPendingBit(USART1, USART_IT_NE);
	}
	if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
	{
		Uart1Err.frame_err++;
		__usart1_err_handle();
		printf("usart1 frame err!\r\n");
		USART_ClearITPendingBit(USART1, USART_IT_FE);
	}
	if(USART_GetITStatus(USART1, USART_IT_FE) != RESET)
	{
		Uart1Err.parity_err++;
		__usart1_err_handle();
		printf("usart1 parity err!\r\n");
		USART_ClearITPendingBit(USART1, USART_IT_FE);
	}
}

/*-------------------------------------------------------------------------------------------------------*/

static void usart1_mode_config(int Baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART1 mode config */
	USART_DeInit(USART1); //复位串口
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_PE, ENABLE);
	USART_ITConfig(USART1, USART_IT_ERR, ENABLE);	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC); //清除发送标志位
}

/*-------------------------------------------------------------------------------------------------------*/

static void usart1_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*-------------------------------------------------------------------------------------------------------*/

static void usart1_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART Tx/Rx PIN */		
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
}

/*-------------------------------------------------------------------------------------------------------*/

static void usart1_rcc_config(void)
{
	/* Enable UART GPIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);
	/* Enable UART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

/*-------------------------------------------------------------------------------------------------------*/

unsigned int usart1_write(unsigned char *src, unsigned int len)
{
	while(len--)
	{
		USART_SendData(USART1, *src++);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return len;
}

void usart1_init(int Baudrate)
{
	usart1_rcc_config();
	usart1_gpio_config();
	usart1_nvic_config();
	usart1_mode_config(Baudrate);
	printf("[usart1_init] baudrate: %d\r\n", Baudrate);
}

void usart1_open(void)
{
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC); //清除发送标志位
	printf("[usart1_open] \r\n");
}

void usart1_close(void)
{
	USART_Cmd(USART1, DISABLE);
	printf("[usart1_close] r\n");
}

/*-------------------------------------------------------------------------------------------------------*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printfset to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#if 1
/* 需使用microLib */
/* 重定向c库函数printf到UART */
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return (ch);
}

/* 重定向c库函数scanf到UART */
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}

#else
/*-------------------------------------------------------------------------------------------------------*/

/* 加入以下代码,支持printf函数,而不需要选择use MicroLIB */
#pragma import(__use_no_semihosting)             
/* 标准库需要的支持函数 */
struct __FILE
{
	int handle;
}; 

FILE __stdout;
FILE __stdin;

/* 定义_sys_exit()以避免使用半主机模式 */
_sys_exit(int x)
{
	x = x;
}

/* 重定义fputc函数 */
int fputc(int ch, FILE *f)
{      
	USART_SendData(USART1, (unsigned char)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return (ch);
}

/* 重定向c库函数scanf到UART */
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}

#endif

