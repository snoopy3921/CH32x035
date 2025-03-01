#include "os_cpu.h"
#include "os_task.h"
#include "os_kernel.h"


static uint8_t __attribute__((aligned(4))) irq_stack_space[OS_CFG_IRQ_STK_SIZE];
uint8_t* irq_stack_ptr = (irq_stack_space + OS_CFG_IRQ_STK_SIZE); 


/* clear soft interrupt */
void os_cpu_clear_sw_irq(void)
{
    SysTick->CTLR &= ~(1<<31);
}

/* trigger software interrupt */
void os_cpu_trigger_sw_irq(void)
{
    SysTick->CTLR |= (1<<31);
}

void os_cpu_get_irq_sp()
{
	__asm volatile("csrrw sp, mscratch, sp  \n");
}
void os_cpu_free_irq_sp()
{
	__asm volatile("csrrw sp, mscratch, sp \n");
}

/*Passing SystemCoreClock to init tick at every 1ms*/
static void os_cpu_systick_init_freq(uint32_t cpu_freq)
{
	volatile uint32_t ticks = cpu_freq / 1000;
	/* Clear counting flag status */
	SysTick->SR = 0;
	/* Couting up init, auto-reload enable */
	SysTick->CTLR |= (1 << 5) | (1 << 4) | (1 << 3);
	/* HCLK as clock source */
	SysTick->CTLR |= (1 << 2);
	/* Reset counter */
	SysTick->CNT = (uint32_t)0;
	/* Set compare value */
	SysTick->CMP = (uint32_t)ticks - 1;
	/* Start systick timer */
	SysTick->CTLR = 0xf;
}

void os_cpu_init_and_run(void)
{
	__asm volatile(
		"lw t0, irq_stack_ptr	\n" 
	);
	// __asm volatile(
	// 	"addi t0, t0, -512	\n" 
	// );
	__asm volatile(
		"csrw mscratch,t0		\n" 
	);
	
	
	irq_stack_ptr = (uint8_t *)(((uint32_t) irq_stack_ptr) - sizeof(uint32_t));

	irq_stack_ptr = (uint8_t *)(((uint32_t) irq_stack_ptr) & 0xFFFFFFFC);
	
	NVIC_SetPriority(Software_IRQn,0x00);
	NVIC_SetPriority(SysTick_IRQn, 0x01);   
	os_cpu_systick_init_freq(SystemCoreClock);
	NVIC_EnableIRQ(Software_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);	
	os_cpu_start_first_task();
}



/**
 * The stack frame when init for RiscV QingKe V4
 * 
 * The first param of pf_task(void *p_arg) is p_arg, goes into a0
 */
uint32_t *os_cpu_init_stk_frame(uint32_t *p_top_of_stack, task_func_t pf_task, void *p_arg )
{
	/* Move down the sp */
	p_top_of_stack  -= (uint32_t)(sizeof(cpu_stk_frame_t)/sizeof(uint32_t));

	/* Assign the stack frame */
	cpu_stk_frame_t *p_stack_frame = (cpu_stk_frame_t *)p_top_of_stack;

// 	uint32_t gp = 0;
//     __asm volatile("mv %0, gp":"=r"(gp));

	p_stack_frame->gp        = (uint32_t)0u;           // global pointer
	p_stack_frame->a0        = (uint32_t)p_arg;        // argument
	p_stack_frame->ra		 = (uint32_t)pf_task;      // task exit
	p_stack_frame->mstatus   = (uint32_t)0x1880;   	   // return to machine mode and enable interrupt
	p_stack_frame->mepc      = (uint32_t)pf_task;      // task entry 

	return (uint32_t *) p_top_of_stack;
}


#ifdef __cplusplus
extern "C"
{
#endif
	__attribute__((interrupt())) 
	void os_cpu_SysTickHandler()
	{
		os_cpu_get_irq_sp();
		DISABLE_INTERRUPTS
		SysTick->SR = 0;
		/* Increment the RTOS tick. */
		if (os_task_increment_tick() == OS_TRUE)
		{
			check_for_stack_overflow();
			/* A context switch is required.  Context switching is performed in
			 * the PendSV interrupt.  Pend the PendSV interrupt. */
			os_cpu_trigger_PendSV();	
		}
		ENABLE_INTERRUPTS
		os_cpu_free_irq_sp();
	}

#ifdef __cplusplus
}
#endif
