#ifndef __LED_H
#define __LED_H

/*-------------------------------------------------------------------------------------------------------*/

#define ON	1
#define OFF	0

/*-------------------------------------------------------------------------------------------------------*/

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
				else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_5);\
				else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5)

/*-------------------------------------------------------------------------------------------------------*/

/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)			{p->BSRR=i;} //设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;} //输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;} //输出反转状态

/*-------------------------------------------------------------------------------------------------------*/

/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_5)
#define LED1_OFF			digitalHi(GPIOB,GPIO_Pin_5)
#define LED1_ON			digitalLo(GPIOB,GPIO_Pin_5)

#define LED2_TOGGLE		digitalToggle(GPIOE,GPIO_Pin_5)
#define LED2_OFF			digitalHi(GPIOE,GPIO_Pin_5)
#define LED2_ON			digitalLo(GPIOE,GPIO_Pin_5)

/*-------------------------------------------------------------------------------------------------------*/

void led_init(void);

#endif /* __LED_H */

