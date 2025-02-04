#include "ch32x035_gpio.h"

int main()
{     
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    while(1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        for(volatile int i = 0; i < 1000000; i++);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        for(volatile int i = 0; i < 1000000; i++);
    }
}