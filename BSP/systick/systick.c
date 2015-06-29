#include "includes.h"

#if 0 /* �жϷ�ʽ��ʱ */
/*-------------------------------------------------------------------------------------------------------*/

static volatile unsigned int TimingDelay = 0;

/*-------------------------------------------------------------------------------------------------------*/
/* 
 *  SysTick��ʼ��ʱĬ�����õ����ȼ�����͵�,
 *  �������ж���ʹ��SysTick��ʱ��ʱ��Ҫ�������ȼ����������жϵ����ȼ�.
 */
void delay_init(void)
{
	/* SystemFrequency / 1000		1ms�ж�һ��
	 * SystemFrequency / 100000	10us�ж�һ��
	 * SystemFrequency / 1000000	1us�ж�һ��
	 */
	//if (SysTick_Config(SystemFrequency / 100000)) // ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 1000000)) // ST3.5.0��汾
	{ 
		while (1); /* Capture error */ 
	}
	
	NVIC_SetPriority (SysTick_IRQn, 0); //�������ȼ�Ϊ���0
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // �رյδ�ʱ�� 	
}

void delay_us(volatile unsigned int nus)
{
	TimingDelay = nus;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; // ʹ�ܵδ�ʱ��  
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk; // �رյδ�ʱ�� 
}

void delay_ms(volatile unsigned int nms)
{
	delay_us(nms * 1000);
}

/*-------------------------------------------------------------------------------------------------------*/

/* �� SysTick �жϺ��� SysTick_Handler()���� */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

#else /* ��ѯ��ʽ��ʱ */

/*-------------------------------------------------------------------------------------------------------*/

void delay_init(void)
{
	SysTick->CTRL &= ~(1<<2); //ʹ��HCLK/8��Ϊʱ��Դ,72MHz/8=9MHz
	//SysTick->CTRL = (1<<2); //ʹ��HCLK��Ϊʱ��Դ,72MHz

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //9MHz
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); //72MHz
}

//0 < nus < (2^24-1)/9=1864135
void delay_us(volatile unsigned int nus)
{
	unsigned int temp;
	SysTick->LOAD = 9 * nus; //9->1us
	SysTick->VAL = 0x00; //��ռ�����
	SysTick->CTRL |= 0x01; //ʹ��SysTick, ��ʼ����

	do{
		temp = SysTick->CTRL; //��ȡ��ǰ����������
	}while((temp&0x01) && (!(temp&(1<<16)))); //ѭ����CTRL��bit16, �ȴ�ʱ�䵽��

	SysTick->CTRL = 0x00; //Disable Systick
	SysTick->VAL = 0x00; //��ռ�����
}

//0 < nms < (2^24-1)/9000=1864.135
void delay_ms(volatile unsigned int nms)
{
	unsigned int temp;
	SysTick->LOAD = 9000 * nms; //9000->1ms
	SysTick->VAL = 0x00; //��ռ�����
	SysTick->CTRL |= 0x01; //ʹ��SysTick, ��ʼ����

	do{
		temp = SysTick->CTRL; //��ȡ��ǰ����������
	}while((temp&0x01) && (!(temp&(1<<16)))); //ѭ����CTRL��bit16, �ȴ�ʱ�䵽��

	SysTick->CTRL = 0x00; //Disable Systick
	SysTick->VAL = 0x00; //��ռ�����
}

#endif

void delay_count(volatile unsigned int nCount) //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


#if 0
/*-------------------------------------------------------------------------------------------------------*/

/* �ж��в�����ѯ�ķ�ʽ��ʱ ����ʹ��uCOSʱ����ѯ��ʱ*/

/*-------------------------------------------------------------------------------------------------------*/

static unsigned int fac_us = 0;
static unsigned int fac_ms = 0;

/*-------------------------------------------------------------------------------------------------------*/
/* 
 *  SysTick��ʼ��ʱĬ�����õ����ȼ�����͵�,
 */
void delay_init(void)
{
	/* SystemFrequency / 1000		1ms�ж�һ��
	 * SystemFrequency / 100000	10us�ж�һ��
	 * SystemFrequency / 1000000	1us�ж�һ��
	 */
	//if (SysTick_Config(SystemFrequency / 100000)) // ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 1000000)) // ST3.5.0��汾
	{ 
		while (1); /* Capture error */ 
	}
	
	//NVIC_SetPriority (SysTick_IRQn, 0); //�������ȼ�Ϊ���0
	//SysTick->CTRL &= 0xFD; //���ж�

	fac_us = SystemCoreClock / 1000000; /* ÿus����fac_us */
	fac_ms = fac_us / 1000; /* ÿms����fac_ms */	
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
				tcnt += told - tnow; // SYSTICK��һ���ݼ��ļ�����
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

