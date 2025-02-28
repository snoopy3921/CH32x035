#include "main.h"
#include "ch32x035_gpio.h"
#include "ch32x035.h"
#include "bsp_cfg.h"

static volatile uint32_t val = SysTick->CNT;

void systick_init_test(uint32_t cpu_freq)
{
    volatile uint32_t ticks = cpu_freq / 1000;
    /* Clear counting flag status */
    SysTick->SR &= ~(1 << 0);
    /* Couting up init, auto-reload enable */
    SysTick->CTLR |= (1 << 5) | (1 << 4) | (1 << 3);
    /* HCLK as clock source */
    SysTick->CTLR |= (1 << 2);
    /* Reset counter */
    SysTick->CNT = (uint32_t) 0;
    /* Set compare value */
    SysTick->CMP = (uint32_t) ticks - 1;
    /* Software interrupt disable */
    SysTick->CTLR &= ~(1 << 31);
    /* Timer interrupt enable */
    SysTick->CTLR |= (1 << 1);
    /* Start systick timer */
    SysTick->CTLR |= (1 << 0);
}


// #ifdef __cplusplus
// extern "C"
// {
// #endif	
//     __attribute__((interrupt("WCH-Interrupt-fast")))
//     void SysTick_Handler(void)
// 	{
//         GPIOB->OUTDR ^= (1 << 12);
//         /* Clear counting flag status */
//         SysTick->SR = 0;
// 	};
// #ifdef __cplusplus
// }
// #endif


void task_1(void *p_arg)
{
    while (1)
    {
        BSP_PRINT("Task 1 \r\n");
        //GPIOB->OUTDR ^= (1 << 12);
        for(volatile int i = 0; i < 1000000; i++);
    }
}
void task_2(void *p_arg)
{
    while (1)
    {
        BSP_PRINT("Task 2 \r\n");
        for(volatile int i = 0; i < 1000000; i++);
    }   
}
void task_3(void *p_arg)
{
    
}

int main()
{  
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    bsp_console_init();

    // ENABLE_INTERRUPTS
    // systick_init_test(SystemCoreClock);
    // NVIC_SetPriority(SysTick_IRQn, 1);
    // NVIC_EnableIRQ(SysTick_IRQn);

    BSP_PRINT("Hello \n");
    os_init();
	os_task_create_list((task_t*)app_task_table, TASK_EOT_ID);
	os_run();

    while(1)
    {
        // BSP_PRINT("Hello \n");
        // for(volatile int i = 0; i < 100000; i++);
        // GPIOB->OUTDR ^= (1 << 12);
    }
}