#include "includes.h"

/*-------------------------------------------------------------------------------------------------------*/

#define SCB_CFSR        (*(volatile const unsigned *)0xE000ED28) /* Configurable Fault Status Register */
#define SCB_HFSR        (*(volatile const unsigned *)0xE000ED2C) /* HardFault Status Register */
#define SCB_MMAR        (*(volatile const unsigned *)0xE000ED34) /* MemManage Fault Address register */
#define SCB_BFAR        (*(volatile const unsigned *)0xE000ED38) /* Bus Fault Address Register */

#define SCB_CFSR_MFSR   (*(volatile const unsigned char*)0xE000ED28)  /* Memory-management Fault Status Register */
#define SCB_CFSR_BFSR   (*(volatile const unsigned char*)0xE000ED29)  /* Bus Fault Status Register */
#define SCB_CFSR_UFSR   (*(volatile const unsigned short*)0xE000ED2A) /* Usage Fault Status Register */

/*-------------------------------------------------------------------------------------------------------*/

static void bus_fault_track(void)
{
    printf("[HardFault] bus fault:\n");
    printf("[HardFault] SCB_CFSR_BFSR:0x%02X ", SCB_CFSR_BFSR);

    if(SCB_CFSR_BFSR & (1<<0))
    {
        /* [0]:IBUSERR */
        printf("[HardFault] IBUSERR ");
    }

    if(SCB_CFSR_BFSR & (1<<1))
    {
        /* [1]:PRECISERR */
        printf("[HardFault] PRECISERR ");
    }

    if(SCB_CFSR_BFSR & (1<<2))
    {
        /* [2]:IMPRECISERR */
        printf("[HardFault] IMPRECISERR ");
    }

    if(SCB_CFSR_BFSR & (1<<3))
    {
        /* [3]:UNSTKERR */
        printf("[HardFault] UNSTKERR ");
    }

    if(SCB_CFSR_BFSR & (1<<4))
    {
        /* [4]:STKERR */
        printf("[HardFault] STKERR ");
    }

    if(SCB_CFSR_BFSR & (1<<7))
    {
        printf("[HardFault] SCB->BFAR:%08X\n", SCB_BFAR);
    }
    else
    {
        printf("\n");
    }
}

static void mem_manage_fault_track(void)
{
    printf("[HardFault] mem manage fault:\n");
    printf("[HardFault] SCB_CFSR_MFSR:0x%02X ", SCB_CFSR_MFSR);

    if(SCB_CFSR_MFSR & (1<<0))
    {
        /* [0]:IACCVIOL */
        printf("[HardFault] IACCVIOL ");
    }

    if(SCB_CFSR_MFSR & (1<<1))
    {
        /* [1]:DACCVIOL */
        printf("[HardFault] DACCVIOL ");
    }

    if(SCB_CFSR_MFSR & (1<<3))
    {
        /* [3]:MUNSTKERR */
        printf("MUNSTKERR ");
    }

    if(SCB_CFSR_MFSR & (1<<4))
    {
        /* [4]:MSTKERR */
        printf("[HardFault] MSTKERR ");
    }

    if(SCB_CFSR_MFSR & (1<<7))
    {
        /* [7]:MMARVALID */
        printf("[HardFault] SCB->MMAR:%08X\n", SCB_MMAR);
    }
    else
    {
        printf("\n");
    }
}

static void usage_fault_track(void)
{
    printf("[HardFault] usage fault:\n");
    printf("[HardFault] SCB_CFSR_UFSR:0x%02X ", SCB_CFSR_UFSR);

    if(SCB_CFSR_UFSR & (1<<0))
    {
        /* [0]:UNDEFINSTR */
        printf("[HardFault] UNDEFINSTR ");
    }

    if(SCB_CFSR_UFSR & (1<<1))
    {
        /* [1]:INVSTATE */
        printf("[HardFault] INVSTATE ");
    }

    if(SCB_CFSR_UFSR & (1<<2))
    {
        /* [2]:INVPC */
        printf("[HardFault] INVPC ");
    }

    if(SCB_CFSR_UFSR & (1<<3))
    {
        /* [3]:NOCP */
        printf("[HardFault] NOCP ");
    }

    if(SCB_CFSR_UFSR & (1<<8))
    {
        /* [8]:UNALIGNED */
        printf("[HardFault] UNALIGNED ");
    }

    if(SCB_CFSR_UFSR & (1<<9))
    {
        /* [9]:DIVBYZERO */
        printf("[HardFault] DIVBYZERO ");
    }

    printf("\n");
}

static void hard_fault_track(void)
{
    if(SCB_HFSR & (1UL<<1))
    {
        /* [1]:VECTBL, Indicates hard fault is caused by failed vector fetch. */
        printf("[HardFault] failed vector fetch\n");
    }

    if(SCB_HFSR & (1UL<<30))
    {
        /* [30]:FORCED, Indicates hard fault is taken because of bus fault,
                        memory management fault, or usage fault. */
        if(SCB_CFSR_BFSR)
        {
            bus_fault_track();
        }

        if(SCB_CFSR_MFSR)
        {
            mem_manage_fault_track();
        }

        if(SCB_CFSR_UFSR)
        {
            usage_fault_track();
        }
    }

    if(SCB_HFSR & (1UL<<31))
    {
        /* [31]:printfEVT, Indicates hard fault is triggered by debug event. */
        printf("[HardFault] debug event\n");
    }
}

void hard_fault_handler_c (unsigned int *sp) 
{ 
    unsigned int stacked_r0; 
    unsigned int stacked_r1; 
    unsigned int stacked_r2; 
    unsigned int stacked_r3; 
    unsigned int stacked_r12; 
    unsigned int stacked_lr; 
    unsigned int stacked_pc; 
    unsigned int stacked_xpsr;


    stacked_r0 = ((unsigned long) *sp++); 
    stacked_r1 = ((unsigned long) *sp++); 
    stacked_r2 = ((unsigned long) *sp++); 
    stacked_r3 = ((unsigned long) *sp++); 
    stacked_r12 = ((unsigned long) *sp++); 
    stacked_lr = ((unsigned long) *sp++); 
    stacked_pc = ((unsigned long) *sp++); 
    stacked_xpsr = ((unsigned long) *sp++);

    printf("\r\n[Hard fault handler - all numbers in hex]\r\n"); 
    printf("R0  = 0x%08x\r\n", stacked_r0); 
    printf("R1  = 0x%08x\r\n", stacked_r1); 
    printf("R2  = 0x%08x\r\n", stacked_r2); 
    printf("R3  = 0x%08x\r\n", stacked_r3); 
    printf("R12 = 0x%08x\r\n", stacked_r12); 
    printf("LR[R14] = 0x%08x subroutine call return address\r\n", stacked_lr); 
    printf("PC[R15] = 0x%08x program counter\r\n", stacked_pc); 
    printf("xPSR = 0x%08x\r\n", stacked_xpsr);
    printf("BFAR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED38)))); 
    printf("CFSR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED28)))); 
    printf("HFSR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED2C)))); 
    printf("DFSR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED30)))); 
    printf("AFSR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED3C)))); 
    printf("SHCSR = 0x%08x\r\n", (*((volatile unsigned long *)(0xE000ED24))));

	hard_fault_track();
	
}

/*-------------------------------------------------------------------------------------------------------*/

__asm void hardfault(void)
{    
#if 1
    PRESERVE8
    IMPORT hard_fault_handler_c
    
    TST LR, #0x04 // test EXC_RETURN bit 2
    ITE EQ  // 如果为0
    MRSEQ R0, MSP //  则使用的是主堆栈，故把MSP的值取出
    MRSNE R0, PSP  // 否则, 使用的是进程堆栈，故把MSP的值取出
    B hard_fault_handler_c
#else
    IMPORT hw_hard_fault_exception
    ; get current context
    TST     lr, #0x04               ; if(!EXC_RETURN[2])
    MRSNE   r0, msp                 ; get fault context from handler.
    MRSEQ   r0, psp                 ; get fault context from thread.

    STMFD   r0!, {r4 - r11}         ; push r4 - r11 register
    STMFD   r0!, {lr}               ; push exec_return register

    MSRNE   msp, r0                 ; update stack pointer to MSP.
    MSREQ   psp, r0                 ; update stack pointer to PSP.

    PUSH    {lr}
    BL      hw_hard_fault_exception
    POP     {lr}

    ORR     lr, lr, #0x04
    BX      lr
    ENDP

    END
#endif
}

/*-------------------------------------------------------------------------------------------------------*/

void HardFault_Handler(void)
{
	hardfault();
	while (1)
	{
	}
}

