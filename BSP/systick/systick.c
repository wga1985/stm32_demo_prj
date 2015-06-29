#include "includes.h"

#if 0 /* 中断方式延时 */
/*-------------------------------------------------------------------------------------------------------*/

static volatile unsigned int TimingDelay = 0;

/*-------------------------------------------------------------------------------------------------------*/
/* 
 *  SysTick初始化时默认配置的优先级是最低的,
 *  在其它中断中使用SysTick延时的时候，要配置优先级高于其它中断的优先级.
 */
void delay_init(void)
{
	/* SystemFrequency / 1000		1ms中断一次
	 * SystemFrequency / 100000	10us中断一次
	 * SystemFrequency / 1000000	1us中断一次
	 */
	//if (SysTick_Config(SystemFrequency / 100000)) // ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 1000000)) // ST3.5.0库版本
	{ 
		while (1); /* Capture error */ 
	}
	
	NVIC_SetPriority (SysTick_IRQn, 0); //设置优先级为最高0
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // 关闭滴答定时器 	
}

void delay_us(volatile unsigned int nus)
{
	TimingDelay = nus;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; // 使能滴答定时器  
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // 关闭滴答定时器 
}

void delay_ms(volatile unsigned int nms)
{
	delay_us(nms * 1000);
}

/*-------------------------------------------------------------------------------------------------------*/

/* 在 SysTick 中断函数 SysTick_Handler()调用 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

#else /* 轮询方式延时 */

/*-------------------------------------------------------------------------------------------------------*/

void delay_init(void)
{
	SysTick->CTRL &= ~(1<<2); //使用HCLK/8作为时钟源,72MHz/8=9MHz
	//SysTick->CTRL = (1<<2); //使用HCLK作为时钟源,72MHz

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //9MHz
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); //72MHz
}

//0 < nus < (2^24-1)/9=1864135
void delay_us(volatile unsigned int nus)
{
	unsigned int temp;
	SysTick->LOAD = 9 * nus; //9->1us
	SysTick->VAL = 0x00; //清空计数器
	SysTick->CTRL |= 0x01; //使能SysTick, 开始倒数

	do{
		temp = SysTick->CTRL; //读取当前倒数计数器
	}while((temp&0x01) && (!(temp&(1<<16)))); //循环读CTRL的bit16, 等待时间到达

	SysTick->CTRL = 0x00; //Disable Systick
	SysTick->VAL = 0x00; //清空计数器
}

//0 < nms < (2^24-1)/9000=1864.135
void delay_ms(volatile unsigned int nms)
{
	unsigned int temp;
	SysTick->LOAD = 9000 * nms; //9000->1ms
	SysTick->VAL = 0x00; //清空计数器
	SysTick->CTRL |= 0x01; //使能SysTick, 开始倒数

	do{
		temp = SysTick->CTRL; //读取当前倒数计数器
	}while((temp&0x01) && (!(temp&(1<<16)))); //循环读CTRL的bit16, 等待时间到达

	SysTick->CTRL = 0x00; //Disable Systick
	SysTick->VAL = 0x00; //清空计数器
}

#endif

void delay_count(volatile unsigned int nCount) //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


#if 0
/*-------------------------------------------------------------------------------------------------------*/

/* 中断中采用轮询的方式延时 ，如使用uCOS时的轮询延时*/

/*-------------------------------------------------------------------------------------------------------*/

static unsigned int fac_us = 0;
static unsigned int fac_ms = 0;

/*-------------------------------------------------------------------------------------------------------*/
/* 
 *  SysTick初始化时默认配置的优先级是最低的,
 */
void delay_init(void)
{
	/* SystemFrequency / 1000		1ms中断一次
	 * SystemFrequency / 100000	10us中断一次
	 * SystemFrequency / 1000000	1us中断一次
	 */
	//if (SysTick_Config(SystemFrequency / 100000)) // ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 1000000)) // ST3.5.0库版本
	{ 
		while (1); /* Capture error */ 
	}
	
	//NVIC_SetPriority (SysTick_IRQn, 0); //设置优先级为最高0
	//SysTick->CTRL &= 0xFD; //关中断

	fac_us = SystemCoreClock / 1000000; /* 每us计数fac_us */
	fac_ms = fac_us / 1000; /* 每ms计数fac_ms */	
}

void delay_us(volatile unsigned int nus)
{
	unsigned int reload, ticks;
	unsigned int told, tnow, tcnt = 0;

	reload = SysTick->LOAD;
	ticks = nus * fac_us;
	told = SysTick->VAL;
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told)
			{
				tcnt += told - tnow; // SYSTICK是一个递减的计数器
			}
			else
			{
				tcnt += told + (reload - tnow);
			}
			told = tnow;
			if(tcnt >= ticks) break;
		}
	}
}

void delay_ms(volatile unsigned int nms)
{
	delay_us(nms * 1000);
}
#endif

