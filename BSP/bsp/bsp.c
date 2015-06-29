#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

const char *COMPILED_DATE = __DATE__; // 获得编译日期
const char *COMPILED_TIME = __TIME__; // 获得编译时间

/*-------------------------------------------------------------------------------------------------------*/

/* 每个STM32 芯片都有唯一的 96_bit unique ID */
void Print_STM32_ChipID(void)
{
	unsigned int ChipUniqueID0, ChipUniqueID1, ChipUniqueID2;
	unsigned short FlashCapacity;

	ChipUniqueID0 = *(volatile unsigned int *)(0X1FFFF7F0); // 高字节
	ChipUniqueID1 = *(volatile unsigned int *)(0X1FFFF7EC);
	ChipUniqueID2 = *(volatile unsigned int *)(0X1FFFF7E8); // 低字节

	FlashCapacity = *(volatile unsigned short *)(0X1FFFF7E0); //stm32 Flash Capacity

	printf("+---------------------------------------------------------+\r\n");
	printf("ST STM32 (Cortex-M3)\r\n");	
	printf("STM32 Chip Unique ID: %X-%X-%X\r\n", ChipUniqueID0, ChipUniqueID1, ChipUniqueID2);
	printf("STM32 Flash Capacity: %dK\r\n", FlashCapacity);	
	printf("COMPILED_DATE: %s\r\n", COMPILED_DATE);
	printf("COMPILED_TIME: %s\r\n", COMPILED_TIME);	
	printf("+---------------------------------------------------------+\r\n");
}

/*-------------------------------------------------------------------------------------------------------*/

void Print_STM32_ClocksFreq(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	printf("+---------------------------------------------------------+\r\n");
	printf("ST STM32 CLOCK:\r\n");
	printf("SYSCLK = %dMHz\r\n", RCC_Clocks.SYSCLK_Frequency/1000000);
	printf("HCLK = %dMHz\r\n", RCC_Clocks.HCLK_Frequency/1000000);
	printf("PCLK1 = %dMHz\r\n", RCC_Clocks.PCLK1_Frequency/1000000);
	printf("PCLK2 = %dMHz\r\n", RCC_Clocks.PCLK2_Frequency/1000000);
	printf("ADCCLK = %dMHz\r\n", RCC_Clocks.ADCCLK_Frequency/1000000);
	printf("+---------------------------------------------------------+\r\n");
	printf("\r\n");
}

void STM32_ResetCheck(void)
{
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		printf("Power On Reset....\r\n");
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		printf("NRST Pin Reset....\r\n");
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		printf("Software Reset....\r\n");
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		printf("Independent Watcdog Reset....\r\n");
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
		printf("Window Watchdog Reset....\r\n");
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
	{
		printf("Low-Power Reset....\r\n");
	}
	RCC_ClearFlag();
}

void BKP_Test(void)
{
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		printf("BKP_DR1 != 0xA5A5\r\n");
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		//BKP_DeInit();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		BKP_DeInit();
		printf("BKP_DR1 = 0xA5A5\r\n");
	}
}

void Big_endian_OR_Little_endian(void)
{
	/* 大端模式: 高字节存储在低地址，低字节存储在高地址 */
	/* 小端模式: 高字节存储在高地址，低字节存储在低地址 */
	short s = 0x1234;
	char *p = (char *)&s;
	//printf("0x%X, [%p]: 0x%X, [%p]: 0x%X\r\n", s, p, p[0], p+1, p[1]);
	if((p[0] == 0x34) && (p[1] == 0x12))
	{
		printf("MCU: Little_endian\r\n");
	}
	else
	{
		printf("MCU: Big_endian\r\n");
	}
}

void Print_STM32F10X_STDPERIPH_VERSION(void)
{
	unsigned int version = 0;
	version = __STM32F10X_STDPERIPH_VERSION;
	printf("STM32F10x Standard Peripheral Library version number: %d.%d.%d.%d\r\n",
		__STM32F10X_STDPERIPH_VERSION_MAIN,
		__STM32F10X_STDPERIPH_VERSION_SUB1,
		__STM32F10X_STDPERIPH_VERSION_SUB2,
		__STM32F10X_STDPERIPH_VERSION_RC);
}

/*-------------------------------------------------------------------------------------------------------*/

void NVIC_Config(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif

	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}

/*-------------------------------------------------------------------------------------------------------*/

void BSP_Init(void)
{
	SystemInit();
	NVIC_Config();
	delay_init();
	usart1_init(115200);
	Print_STM32F10X_STDPERIPH_VERSION();
	STM32_ResetCheck();
	Big_endian_OR_Little_endian();
	Print_STM32_ChipID();	
	Print_STM32_ClocksFreq();
	
	led_init();
	timer2_init(72,1000); // 1ms
}

