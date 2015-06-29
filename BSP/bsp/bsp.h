#ifndef __BSP_H
#define __BSP_H

void BSP_Init(void);
void NVIC_Config(void);

void Print_STM32_ChipID(void);
void Print_STM32_ClocksFreq(void);
void STM32_ResetCheck(void);
void Big_endian_OR_Little_endian(void);

#endif // __BSP_H
