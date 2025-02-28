#ifndef OS_CPU_H
#define OS_CPU_H

#include "ch32x035.h"
#include "core_riscv.h"
#include "system_ch32x035.h"
#include "ch32x035_conf.h"
#include "os_task.h"

#define DISABLE_INTERRUPTS                \
      {                                   \
            __asm volatile ("csrw mstatus, %0" : :"r"(0x1800)); \
      }
#define ENABLE_INTERRUPTS                 \
      {                                   \
            __asm volatile ("csrw 0x800, %0" : :"r"(0x1888)); \
      }

#define os_cpu_SWHandler      SW_Handler
#define os_cpu_SysTickHandler SysTick_Handler

/* Make PendSV and SysTick the lowest priority interrupts. */
#define os_cpu_setup_PendSV() NVIC_SetPriority(Software_IRQn,0xf0)     
#define os_cpu_trigger_PendSV() {SysTick->CTLR |= (1<<31);}

/* clear soft interrupt */
extern void os_cpu_clear_sw_irq(void);
/* trigger software interrupt */
extern void os_cpu_trigger_sw_irq(void);

extern void os_cpu_init_and_run(void);



#ifndef OS_CFG_IRQ_STK_SIZE
#warning "Irq stack size is not specified, using default value."
#define OS_CFG_IRQ_STK_SIZE ((size_t)512) /* In byte */
#endif

extern void os_cpu_get_irq_sp();
extern void os_cpu_free_irq_sp();




/* int reg offset table */
/* Low address */
#define __reg_mepc_OFFSET       0x00
#define __reg_mstatus_OFFSET    0x04
#define __reg_x1_OFFSET         0x08
#define __reg_x3_OFFSET         0x0C
#define __reg_x4_OFFSET         0x10
#define __reg_x5_OFFSET         0x14
#define __reg_x6_OFFSET         0x18
#define __reg_x7_OFFSET         0x1C
#define __reg_x8_OFFSET         0x20
#define __reg_x9_OFFSET         0x24
#define __reg_x10_OFFSET        0x28
#define __reg_x11_OFFSET        0x2C
#define __reg_x12_OFFSET        0x30
#define __reg_x13_OFFSET        0x34
#define __reg_x14_OFFSET        0x38
#define __reg_x15_OFFSET        0x3C
#define __reg_x16_OFFSET        0x40
#define __reg_x17_OFFSET        0x44
#define __reg_x18_OFFSET        0x48
#define __reg_x19_OFFSET        0x4C
#define __reg_x20_OFFSET        0x50
#define __reg_x21_OFFSET        0x54
#define __reg_x22_OFFSET        0x58
#define __reg_x23_OFFSET        0x5C
#define __reg_x24_OFFSET        0x60
#define __reg_x25_OFFSET        0x64
#define __reg_x26_OFFSET        0x68
#define __reg_x27_OFFSET        0x6C
#define __reg_x28_OFFSET        0x70
#define __reg_x29_OFFSET        0x74
#define __reg_x30_OFFSET        0x78
#define __reg_x31_OFFSET        0x7C
/* High address */


typedef struct cpu_stk_frame {
      uint32_t          mepc;
      uint32_t          mstatus;
      union { uint32_t  x1,  ra; };
      union { uint32_t  x3,  gp; };
      union { uint32_t  x4,  tp; };
      union { uint32_t  x5,  t0; };
      union { uint32_t  x6,  t1; };
      union { uint32_t  x7,  t2; };
      union { uint32_t  x8,  s0, fp; };
      union { uint32_t  x9,  s1; };
      union { uint32_t x10,  a0; };
      union { uint32_t x11,  a1; };
      union { uint32_t x12,  a2; };
      union { uint32_t x13,  a3; };
      union { uint32_t x14,  a4; };
      union { uint32_t x15,  a5; };
      union { uint32_t x16,  a6; };
      union { uint32_t x17,  a7; };
      union { uint32_t x18,  s2; };
      union { uint32_t x19,  s3; };
      union { uint32_t x20,  s4; };
      union { uint32_t x21,  s5; };
      union { uint32_t x22,  s6; };
      union { uint32_t x23,  s7; };
      union { uint32_t x24,  s8; };
      union { uint32_t x25,  s9; };
      union { uint32_t x26, s10; };
      union { uint32_t x27, s11; };
      union { uint32_t x28,  t3; };
      union { uint32_t x29,  t4; };
      union { uint32_t x30,  t5; };
      union { uint32_t x31,  t6; };
  
#if ARCH_RISCV_FPU
      /* float reg table */

#endif
} cpu_stk_frame_t;

/* Stack frame init*/
uint32_t *os_cpu_init_stk_frame(uint32_t *p_top_of_stack, task_func_t pf_task, void *p_arg );


#endif