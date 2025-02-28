#include "os_kernel.h"

#include "os_cpu.h"
#include "os_msg.h"
#include "os_timer.h"
#include "os_prio.h"
#include "os_task.h"
#include "os_log.h"
#include "task_list.h"

__attribute__((weak)) 
void os_hook_stack_overflow(task_id_t task_id, uint8_t num_of_stk_overflow )
{
    LOG_ERROR("Stack overflown %d : %d", task_id, num_of_stk_overflow);
    for(;;);
}

static volatile uint16_t critical_nesting_count;


void os_critical_enter(void)
{
    DISABLE_INTERRUPTS
    critical_nesting_count++;
}

void os_critical_exit(void)
{
    os_assert(critical_nesting_count, "NESTING CRITICAL UNBALANCED");
    critical_nesting_count--;
    if (critical_nesting_count == 0)
    {
        ENABLE_INTERRUPTS
    }
}


void os_init(void)
{
    if (TASK_EOT_ID < 1u)
    {
        LOG_ERROR("OS_ERR_NO_TASK_AVAILABLE - Entering while loop");
        DISABLE_INTERRUPTS
        while(1);
    }
    critical_nesting_count = (uint16_t)0u;
    os_prio_init();
    os_msg_init();
    os_timer_init();
}

void os_run(void)
{
    os_task_start();
    os_cpu_init_and_run();
}
