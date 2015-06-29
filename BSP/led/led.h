#ifndef __LED_H
#define __LED_H

/*-------------------------------------------------------------------------------------------------------*/

#define ON	1
#define OFF	0

/*-------------------------------------------------------------------------------------------------------*/

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
				else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_5);\
				else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5)

/*-------------------------------------------------------------------------------------------------------*/

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define digitalHi(p,i)			{p->BSRR=i;} //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BRR=i;} //����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;} //�����ת״̬

/*-------------------------------------------------------------------------------------------------------*/

/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_5)
#define LED1_OFF			digitalHi(GPIOB,GPIO_Pin_5)
#define LED1_ON			digitalLo(GPIOB,GPIO_Pin_5)

#define LED2_TOGGLE		digitalToggle(GPIOE,GPIO_Pin_5)
#define LED2_OFF			digitalHi(GPIOE,GPIO_Pin_5)
#define LED2_ON			digitalLo(GPIOE,GPIO_Pin_5)

/*-------------------------------------------------------------------------------------------------------*/

void led_init(void);

#endif /* __LED_H */

