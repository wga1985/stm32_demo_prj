#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

volatile unsigned int g_Tim2Array[(unsigned char)TimerEvId_IDMAX] = {0};

#define iTimerMAX	5
static Timer_t TimerList[iTimerMAX];

/*-------------------------------------------------------------------------------------------------------*/

int timer2_callback_unregister(Timer_t timer)
{
	unsigned int iTimer;
	for(iTimer = 0; iTimer < iTimerMAX; iTimer++)
	{
		if(TimerList[iTimer].callbackfunc != NULL)
		{
			TimerList[iTimer].callbackfunc = NULL;
			printf("timer2_callback_unregister success\r\n");
			return 0;
		}
	}
	return -1;
}

int timer2_callback_register(Timer_t timer)
{
	unsigned int iTimer;
	for(iTimer = 0; iTimer < iTimerMAX; iTimer++)
	{
		if(TimerList[iTimer].callbackfunc == NULL)
		{
			TimerList[iTimer] = timer;
			printf("timer2_callback_register success\r\n");
			return 0;
		}
	}
	return -1;
}

/*-------------------------------------------------------------------------------------------------------*/

static void __timer2_timeout_callback(void)
{
	unsigned int iTimer;
	Timer_t *timer;	
	for(iTimer = 0; iTimer < (unsigned char)iTimerMAX; iTimer++)
	{
		timer = &TimerList[iTimer];
		if(timer->callbackfunc != NULL)
		{
			timer->timerarg.timerTick++;
			if(timer->timerarg.timerTick >=  timer->timerarg.timerdelay)
			{
				timer->callbackfunc(timer->timerarg); //callback func
				timer->timerarg.timerTick = 0; //reload timer
			}
		}
	}
}

/*-------------------------------------------------------------------------------------------------------*/

static void __timer2_timeout_func(void)
{
	unsigned char index;	
	for(index = 0; index < (unsigned char)TimerEvId_IDMAX; index++)
	{
		g_Tim2Array[index]++;
	}
}

/*-------------------------------------------------------------------------------------------------------*/

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //定时器溢出中断
	{
		__timer2_timeout_func();
		__timer2_timeout_callback();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清中断
	}
}

/*-------------------------------------------------------------------------------------------------------*/
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = (1/(72MHZ /72)) * 1000 = 1ms;  Timer2_Init(72, 1000)
 * 中断周期为 = (1/(72MHZ /psc)) * arr
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
/*-------------------------------------------------------------------------------------------------------*/
static void timer2_mode_config(unsigned short psc, unsigned short arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM2);
	/* 72MHZ /(psc * arr) (MHz) */
	TIM_TimeBaseStructure.TIM_Period = (arr - 1); //自动重装载寄存器周期的值(计数值) /* 累计 TIM_Period个频率后产生一个更新或者中断 */	 
	TIM_TimeBaseStructure.TIM_Prescaler = (psc -1); // Timer时钟预分频数: 72MHz/psc
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); //清定时器标志	
	TIM_SetCounter(TIM2, 0);	
	TIM_Cmd(TIM2, ENABLE);
}

static void timer2_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //  指定抢占式优先级别 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //  指定响应优先级别 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void timer2_rcc_config(void)
{
	/* TIM2 CLK 为APB1*2 = 36MHz*2 = 72MHz */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

/*-------------------------------------------------------------------------------------------------------*/

void timer2_init(unsigned short psc, unsigned short arr)
{
	timer2_rcc_config();
	timer2_nvic_config();
	timer2_mode_config(psc, arr);
	printf("timer2 init.\r\n");
}

void timer2_open(void)
{
	TIM_SetCounter(TIM2, 0);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	printf("timer2 open.\r\n");
}

void timer2_close(void)
{
	TIM_Cmd(TIM2, DISABLE);	
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	TIM_SetCounter(TIM2, 0);
	printf("timer2 close.\r\n");
}

