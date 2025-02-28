#include "os_cpu.h"
#include "os_task.h"
#include "os_kernel.h"

#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "system_stm32l1xx.h"
#include "core_cm3.h"
#include "core_cmFunc.h"

/* For strict compliance with the Cortex-M spec the task start address should
have bit-0 clear, as it is loaded into the PC on exit from an ISR. */
#define VALUE_START_ADDRESS_MASK    ((uint32_t)0xfffffffeUL)

/* Constants required to set up the initial stack. */
#define VALUE_INITIAL_XPSR          (0x01000000)


/*Passing SystemCoreClock to init tick at every 1ms*/
void os_cpu_systick_init_freq(uint32_t cpu_freq)
{
	volatile uint32_t ticks = cpu_freq / 1000;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	SysTick->LOAD = ticks - 1; /* set reload register */

	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(SysTick_IRQn, 1);

	SysTick->VAL = 0; /* Load the SysTick Counter Value */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
			    SysTick_CTRL_TICKINT_Msk |
			    SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
}



/**
 * The stack frame when init for cortex m3
 * VALUE_INITIAL_XPSR
 * pf_task    (PC)       
 * 0x0000000E (LR)    
 * R12                
 * R3                
 * R2                 
 * R1                 
 * R0 (p_arg)        
 * R11                
 * R10                
 * R9                 
 * R8                 
 * R7                 
 * R6                 
 * R5                 
 * R4                 
 */
uint32_t *os_cpu_init_stk_frame(uint32_t *p_top_of_stack, task_func_t pf_task, void *p_arg )
{
	*(--p_top_of_stack) = VALUE_INITIAL_XPSR;                             /*Add offset and assign value for xPSR */
	*(--p_top_of_stack) = ((uint32_t)pf_task) & VALUE_START_ADDRESS_MASK; /* PC */
	*(--p_top_of_stack) = (uint32_t)0x000000EU;                           /* LR */
	p_top_of_stack -= 5;                                                  /* R12, R3, R2 and R1. */
	*p_top_of_stack = (uint32_t)p_arg;                                    /* R0 */
	p_top_of_stack -= 8;                                                  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
	return p_top_of_stack; 
}



#ifdef __cplusplus
extern "C"
{
#endif
	void os_cpu_SVCHandler(void)
	{
		__asm volatile(
		    "CPSID   I				\n" // Prevent interruption during context switch
		    "LDR     R1, =tcb_curr_ptr 	\n" // get pointer to TCB current
		    "LDR     R1, [R1]			\n" // get TCB current	= pointer to StkPtr
		    "LDR     R0, [R1]             	\n" // get StkPtr
		    "LDMIA R0!, {R4-R11}        	\n" //
		    "MSR     PSP, R0              	\n" //
		    "ORR LR, #0xD                	\n" // LR = 0xFFFFFFFD return to threadmode
		    "CPSIE   I				\n" //
		    "BX      LR				\n" //
		);
	}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if 1
	void os_cpu_PendSVHandler(void)
	{
		__asm volatile(
		    //"CPSID   I                       		\n"	//Prevent interruption during context switch
		    "MRS     R0, PSP                     		\n" // PSP is process stack pointer
		    "CBZ     R0, OS_CPU_PendSVHandler_nosave   	\n" // Skip register save the first time

		    "SUBS    R0, R0, #0x20               		\n" // Save remaining regs r4-11 on process stack
		    "STM     R0, {R4-R11}				\n" //

		    "LDR     R1, =tcb_curr_ptr               	\n" // OSTCBCur->OSTCBStkPtr = SP;
		    "LDR     R1, [R1]\n"				    //
		    "STR     R0, [R1]                    		\n" // R0 is SP of process being switched out

		    /* At this point, entire context of process has been saved	*/

		    "OS_CPU_PendSVHandler_nosave:			\n" //
		    "LDR     R0, =tcb_curr_ptr               	\n" // OSTCBCur  = OSTCBHighRdy;
		    "LDR     R1, =tcb_high_rdy_ptr			\n" //
		    "LDR     R2, [R1]					\n" //
		    "STR     R2, [R0]					\n" //

		    "LDR     R0, [R2]                    		\n" // R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
		    "LDM     R0, {R4-R11}                		\n" // Restore r4-11 from new process stack
		    "ADDS    R0, R0, #0x20				\n" //
		    "MSR     PSP, R0                     		\n" // Load PSP with new process SP
		    "ORR     LR, LR, #0x04               		\n" // Ensure exception return uses process stack
		    //"CPSIE   I						\n"  	//
		    "BX      LR                          		\n" // Exception return will restore remaining context
		);
	}
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	void os_cpu_SysTickHandler()
	{
		DISABLE_INTERRUPTS
		/* Increment the RTOS tick. */
		if (os_task_increment_tick() == OS_TRUE)
		{
			check_for_stack_overflow();
			/* A context switch is required.  Context switching is performed in
			 * the PendSV interrupt.  Pend the PendSV interrupt. */
			os_cpu_trigger_PendSV();
		}
		ENABLE_INTERRUPTS
	}

#ifdef __cplusplus
}
#endif
