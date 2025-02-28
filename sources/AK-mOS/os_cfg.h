/*
 * os_cfg.h
 *
 *  Created on: Jun 26, 2024
 *      Author: giahu
 */

#ifndef OS_CFG_H
#define OS_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "bsp_cfg.h"
#include <stdint.h>

/* Kernel common config */
#define OS_CFG_HEAP_SIZE                  ((size_t)1024 * 5u)
#define OS_CFG_PRIO_MAX                   (8)
#define OS_CFG_DELAY_MAX                  ((uint32_t)0xffffffffUL)
/* Under developing */
#define OS_CFG_CHECK_FOR_STK_OVERFLOW     (0u)

/* Task config */
#define OS_CFG_TASK_STK_SIZE_MIN          ((size_t)32u) // (Min > 128 byte) In stack, equal to x 4 bytes
#define OS_CFG_TASK_ENABLE_STACK_FILL     (1u)
#define OS_CFG_TASK_STACK_FILL_BYTE       (0x5Au)
#define OS_CFG_TASK_MSG_Q_SIZE_NORMAL     (8u)

/* Messages config */
#define OS_CFG_MSG_POOL_SIZE              (32u)

/* Timers config */
#define OS_CFG_TIMER_POOL_SIZE            (8u)  /* Max num of timer */
#define OS_CFG_TIMER_TASK_PRI             (0u)  /* Recommend as high as possible */

/* Log config */
#define OS_CFG_USE_LOG                    (1u)  

/* Command line interface config */
#define OS_CFG_USE_CLI                    (0u)  
#define OS_CFG_CLI_TASK_PRI               (0u)  

#if (OS_CFG_USE_LOG || OS_CFG_USE_CLI) == 1
/* Your print method here */
#define USER_PRINT(fmt, ...) BSP_PRINT((const char*)fmt, ##__VA_ARGS__)
#else
#define USER_PRINT(fmt, ...) ((void*)0)
#endif

#ifdef __cplusplus
}
#endif
#endif /* OS_CFG_H */