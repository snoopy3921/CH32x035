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

// __attribute__((naked))
// static void os_cpu_start_first_task(void)
// {
// 	DISABLE_INTERRUPTS
// 	__asm volatile(
// 		"lw      t0, tcb_curr_ptr	\n"
// 	);
// 	__asm volatile(
// 		"lw      sp, 0x00(t0) 		\n"
// 	);
// 		/* Restore context */
// 	__asm volatile(
// 		"lw      t0,   0x00(sp)		\n" /* t0 = sp */
// 	);
// 	__asm volatile(
// 		"csrw    mepc, t0			\n" /* pc = *sp */
// 	);
// 	// __asm volatile(
// 	// 	"lw      t0,   0x04(sp)		\n" /* t0 = sp + 4 = mstatus*/
// 	// );
// 	// __asm volatile(
// 	// 	"csrw    mstatus, t0 		\n" /* mstatus = t0 = sp + 4 */
// 	// );
// 	__asm volatile(
// 		/* Load registers from stack */
// 		"lw x1, 0x08(sp)			\n" /* Load x1 from offset 0x08 */
// 		"lw x3, 0x0C(sp)			\n" /* Load x3 from offset 0x0C */
// 		"lw x4, 0x10(sp)			\n" /* Load x4 from offset 0x10 */
// 		"lw x5, 0x14(sp)			\n" /* Load x5 from offset 0x14 */
// 		"lw x6, 0x18(sp)			\n" /* Load x6 from offset 0x18 */
// 		"lw x7, 0x1C(sp)			\n" /* Load x7 from offset 0x1C */
// 		"lw x8, 0x20(sp)			\n" /* Load x8 from offset 0x20 */
// 		"lw x9, 0x24(sp)			\n" /* Load x9 from offset 0x24 */
// 		"lw x10, 0x28(sp)			\n" /* Load x10 from offset 0x28 */
// 		"lw x11, 0x2C(sp)			\n" /* Load x11 from offset 0x2C */
// 		"lw x12, 0x30(sp)			\n" /* Load x12 from offset 0x30 */
// 		"lw x13, 0x34(sp)			\n" /* Load x13 from offset 0x34 */
// 		"lw x14, 0x38(sp)			\n" /* Load x14 from offset 0x38 */
// 		"lw x15, 0x3C(sp)			\n" /* Load x15 from offset 0x3C */
// 		"lw x16, 0x40(sp)			\n" /* Load x16 from offset 0x40 */
// 		"lw x17, 0x44(sp)			\n" /* Load x17 from offset 0x44 */
// 		"lw x18, 0x48(sp)			\n" /* Load x18 from offset 0x48 */
// 		"lw x19, 0x4C(sp)			\n" /* Load x19 from offset 0x4C */
// 		"lw x20, 0x50(sp)			\n" /* Load x20 from offset 0x50 */
// 		"lw x21, 0x54(sp)			\n" /* Load x21 from offset 0x54 */
// 		"lw x22, 0x58(sp)			\n" /* Load x22 from offset 0x58 */
// 		"lw x23, 0x5C(sp)			\n" /* Load x23 from offset 0x5C */
// 		"lw x24, 0x60(sp)			\n" /* Load x24 from offset 0x60 */
// 		"lw x25, 0x64(sp)			\n" /* Load x25 from offset 0x64 */
// 		"lw x26, 0x68(sp)			\n" /* Load x26 from offset 0x68 */
// 		"lw x27, 0x6C(sp)			\n" /* Load x27 from offset 0x6C */
// 		"lw x28, 0x70(sp)			\n" /* Load x28 from offset 0x70 */
// 		"lw x29, 0x74(sp)			\n" /* Load x29 from offset 0x74 */
// 		"lw x30, 0x78(sp)			\n" /* Load x30 from offset 0x78 */
// 		"lw x31, 0x7C(sp)			\n" /* Load x31 from offset 0x7C */
		
// 		"add    sp, sp, 128			\n" /* Adjust stack pointer */
// 	);
// 	ENABLE_INTERRUPTS
// 	__asm volatile(
// 		"mret						\n" 
// 	);
// }

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
	SysTick->CMP = (uint32_t)ticks * 200;
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




// #ifdef __cplusplus
// extern "C"
// {
// #endif
// 	__attribute__((interrupt()))
// 	void os_cpu_SWHandler(void)
// 	{
// 		__asm volatile(
// 		    "addi sp, sp, -128\n"
		    
// 		    "csrr t0, mstatus\n"
// 		    "sw   t0, 0x04(sp)\n" // __reg_mstatus_OFFSET
// 		    "csrr t0, mepc\n"
// 		    "sw   t0, 0x00(sp)\n" // __reg_mepc_OFFSET

// 		    "sw   x1, 0x08(sp)\n" // __reg_x1_OFFSET
// 		    "sw   x3, 0x0C(sp)\n" // __reg_x3_OFFSET
// 		    "sw   x4, 0x10(sp)\n" // __reg_x4_OFFSET
// 		    "sw   t0, 0x14(sp)\n" // __reg_x5_OFFSET
// 		    "sw   x6, 0x18(sp)\n"  // __reg_x6_OFFSET
// 		    "sw   x7, 0x1C(sp)\n"  // __reg_x7_OFFSET
// 		    "sw   x8, 0x20(sp)\n"  // __reg_x8_OFFSET
// 		    "sw   x9, 0x24(sp)\n"  // __reg_x9_OFFSET
// 		    "sw   x10, 0x28(sp)\n" // __reg_x10_OFFSET
// 		    "sw   x11, 0x2C(sp)\n" // __reg_x11_OFFSET
// 		    "sw   x12, 0x30(sp)\n" // __reg_x12_OFFSET
// 		    "sw   x13, 0x34(sp)\n" // __reg_x13_OFFSET
// 		    "sw   x14, 0x38(sp)\n" // __reg_x14_OFFSET
// 		    "sw   x15, 0x3C(sp)\n" // __reg_x15_OFFSET
// 		    "sw   x16, 0x40(sp)\n" // __reg_x16_OFFSET
// 		    "sw   x17, 0x44(sp)\n" // __reg_x17_OFFSET
// 		    "sw   x18, 0x48(sp)\n" // __reg_x18_OFFSET
// 		    "sw   x19, 0x4C(sp)\n" // __reg_x19_OFFSET
// 		    "sw   x20, 0x50(sp)\n" // __reg_x20_OFFSET
// 		    "sw   x21, 0x54(sp)\n" // __reg_x21_OFFSET
// 		    "sw   x22, 0x58(sp)\n" // __reg_x22_OFFSET
// 		    "sw   x23, 0x5C(sp)\n" // __reg_x23_OFFSET
// 		    "sw   x24, 0x60(sp)\n" // __reg_x24_OFFSET
// 		    "sw   x25, 0x64(sp)\n" // __reg_x25 "sw   x26, 0x68(sp)\n" // __reg_x26_OFFSET
// 		    "sw   x27, 0x6C(sp)\n" // __reg_x27_OFFSET
// 		    "sw   x28, 0x70(sp)\n" // __reg_x28_OFFSET
// 		    "sw   x29, 0x74(sp)\n" // __reg_x29_OFFSET
// 		    "sw   x30, 0x78(sp)\n" // __reg_x30_OFFSET
// 		    "sw   x31, 0x7C(sp)\n" // __reg_x31_OFFSET
// 		);
// 		__asm volatile(
// 		    /* switch to irq stk */
// 		    "mv t0, sp\n"
// 		);
// 		    __asm volatile(
// 		    "lw sp, irq_stack_ptr\n" /* cpu_init: switch to IRQ stack */
// 		);

// 			__asm volatile(
// 		    "sw t0, 0(sp)\n"
// 		    /* clear software interrupt */
// 		    "call os_cpu_clear_sw_irq\n"
// 		    /* resume sp */
// 		    "lw sp, 0(sp)\n"
// 		);

// 		__asm volatile(
// 		    "la t0, tcb_curr_ptr\n" // t0 = &tcb_curr_ptr
// 		);

// 		__asm volatile(
// 		    "la t1, tcb_high_rdy_ptr\n" // t1 = &tcb_high_rdy_ptr
// 		);

// 		__asm volatile(
// 		    // save sp to tcb_curr_ptr.sp
// 		    "lw t2, (t0)\n"
// 		    "sw sp, (t2)\n"
// 		);

// 		__asm volatile(
// 		    // switch task
// 		    // tcb_curr_ptr = tcb_high_rdy_ptr
// 		    "lw t1, (t1)\n"
// 		    "sw t1, (t0)\n"
// 		    // load new task sp
// 		    "lw sp, (t1)\n"
// 		);

// 		__asm volatile(
// 		    /* new thread restore */
// 		    "lw t0, 0x04(sp)\n" // __reg_mstatus_OFFSET
// 		    "csrw mstatus, t0\n"
// 		    "lw t0, 0x00(sp)\n" // __reg_mepc_OFFSET
// 		    "csrw mepc, t0\n"

// 		    "lw  x1,  0x08(sp)\n" // __reg_x1_OFFSET
// 		    "lw  x3,  0x0C(sp)\n" // __reg_x3_OFFSET
// 		    "lw  x4,  0x10(sp)\n" // __reg_x4_OFFSET
// 		    "lw  x5,  0x14(sp)\n" // __reg_x5_OFFSET
// 		    "lw  x6,  0x18(sp)\n" // __reg_x6_OFFSET
// 		    "lw  x7,  0x1C(sp)\n" // __reg_x7_OFFSET
// 		    "lw  x8,  0x20(sp)\n" // __reg_x8_OFFSET
// 		    "lw  x9,  0x24(sp)\n" // __reg_x9_OFFSET
// 		    "lw  x10, 0x28(sp)\n" // __reg_x10_OFFSET
// 		    "lw  x11, 0x2C(sp)\n" // __reg_x11_OFFSET
// 		    "lw  x12, 0x30(sp)\n" // __reg_x12_OFFSET
// 		    "lw  x13, 0x34(sp)\n" // __reg_x13_OFFSET
// 		    "lw  x14, 0x38(sp)\n" // __reg_x14_OFFSET
// 		    "lw  x15, 0x3C(sp)\n" // __reg_x15_OFFSET
// 		    "lw  x16, 0x40(sp)\n" // __reg_x16_OFFSET
// 		    "lw  x17, 0x44(sp)\n" // __reg_x17_OFFSET
// 		    "lw  x18, 0x48(sp)\n" // __reg_x18_OFFSET
// 		    "lw  x19, 0x4C(sp)\n" // __reg_x19_OFFSET
// 		    "lw  x20, 0x50(sp)\n" // __reg_x20_OFFSET
// 		    "lw  x21, 0x54(sp)\n" // __reg_x21_OFFSET
// 		    "lw  x22, 0x58(sp)\n" // __reg_x22_OFFSET
// 		    "lw  x23, 0x5C(sp)\n" // __reg_x23_OFFSET
// 		    "lw  x24, 0x60(sp)\n" // __reg_x24_OFFSET
// 		    "lw  x25, 0x64(sp)\n" // __reg_x25_OFFSET
// 		    "lw  x26, 0x68(sp)\n" // __reg_x26_OFFSET
// 		    "lw  x27, 0x6C(sp)\n" // __reg_x27_OFFSET
// 		    "lw  x28, 0x70(sp)\n" // __reg_x28 ```c
// 		    "lw  x29, 0x74(sp)\n" // __reg_x29_OFFSET
// 		    "lw  x30, 0x78(sp)\n" // __reg_x30_OFFSET
// 		    "lw  x31, 0x7C(sp)\n" // __reg_x31_OFFSET
// 		    "addi sp, sp, 128\n"
// 		);

// 		// __asm volatile(
// 		//     "mret 		\n"
// 		// );
// 	}
// #ifdef __cplusplus
// }
// #endif

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
			GPIOB->OUTDR ^= (1 << 12);	
		}
		ENABLE_INTERRUPTS
		os_cpu_free_irq_sp();
	}

#ifdef __cplusplus
}
#endif
