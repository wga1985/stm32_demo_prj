#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

static void led_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Configuration: PB5 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* GPIOB Configuration: PE5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void led_rcc_config(void)
{
	/*����GPIOB��GPIOF������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);
}

/*-------------------------------------------------------------------------------------------------------*/

void led_init(void)
{
	led_rcc_config();
	led_gpio_config();
	
	/* �ر�����led��	*/	
	GPIO_SetBits(GPIOB,GPIO_Pin_5); //PB.5 �����
	GPIO_SetBits(GPIOE,GPIO_Pin_5); //PE.5 ����� 	
}

