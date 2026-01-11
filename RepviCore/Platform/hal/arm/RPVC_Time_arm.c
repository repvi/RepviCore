/* ARM Cortex-M time/tick management using SysTick */
#include <stddef.h>
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
#include "RPVC_Time_common.h"
#include <stdint.h>

/* SysTick registers */
#define SysTick_BASE        (0xE000E010UL)
#define SysTick_CTRL        (*(volatile uint32_t*)(SysTick_BASE + 0x00))
#define SysTick_LOAD        (*(volatile uint32_t*)(SysTick_BASE + 0x04))
#define SysTick_VAL         (*(volatile uint32_t*)(SysTick_BASE + 0x08))
#define SysTick_CALIB       (*(volatile uint32_t*)(SysTick_BASE + 0x0C))

/* SysTick Control / Status Register Definitions */
#define SysTick_CTRL_ENABLE_Pos      0
#define SysTick_CTRL_TICKINT_Pos     1
#define SysTick_CTRL_CLKSOURCE_Pos   2
#define SysTick_CTRL_COUNTFLAG_Pos   16

#define SysTick_CTRL_ENABLE_Msk      (1UL << SysTick_CTRL_ENABLE_Pos)
#define SysTick_CTRL_TICKINT_Msk     (1UL << SysTick_CTRL_TICKINT_Pos)
#define SysTick_CTRL_CLKSOURCE_Msk   (1UL << SysTick_CTRL_CLKSOURCE_Pos)

static uint32_t g_tickFrequency = 0;

RPVC_Status_t RPVC_Time_Init(const RPVC_TimeConfig_t *config)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (config == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    uint32_t ticks = (config->systemHz / config->tickHz) - 1;
    g_tickFrequency = config->tickHz;
    /* Disable SysTick during configuration */
    SysTick_CTRL = 0;
    
    /* Set reload register */
    SysTick_LOAD = ticks & 0x00FFFFFF;
    
    /* Reset counter */
    SysTick_VAL = 0;
    
    /* Enable SysTick: processor clock, interrupt enabled, counter enabled */
    SysTick_CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
    
    RPVC_Time_SetInitialized(true);
    return RPVC_OK;
}

/* GetTick is implemented in RPVC_Time_common.c */

RPVC_Status_t RPVC_Time_GetMicroseconds(uint64_t *outUs)
{
    if (!RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (outUs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    
    uint32_t reload;
    uint32_t cycles;
    uint64_t ticks;
    
    uint32_t state = RPVC_Interrupts_EnterCritical();
    RPVC_Status_t status = RPVC_Time_GetTick64(&ticks);
    if (status != RPVC_OK) {
        RPVC_Interrupts_ExitCritical(state);
        return status;
    }
    reload = SysTick_LOAD;
    cycles = reload - SysTick_VAL;
    RPVC_Interrupts_ExitCritical(state);
    
    /* Convert to microseconds */
    uint64_t us = ticks * (1000000 / g_tickFrequency);
    us += (uint64_t)cycles * 1000000 / (g_tickFrequency * reload);
    
    *outUs = us;
    return RPVC_OK;
}

RPVC_Status_t RPVC_Time_GetTimeMilliseconds(uint64_t *outMs)
{
    if (!RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (outMs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    uint64_t ticks;
    RPVC_Status_t status = RPVC_Time_GetTick64(&ticks);
    if (status != RPVC_OK) {
        return status;
    }

    *outMs = ticks * (1000 / g_tickFrequency);
    return RPVC_OK;
}

/* TickDiff functions implemented in RPVC_Time_common.c */

RPVC_Status_t RPVC_Time_DelayUs(uint32_t us)
{
    if (!RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    uint64_t start;
    RPVC_Status_t status = RPVC_Time_GetMicroseconds(&start);
    if (status != RPVC_OK) {
        return status;
    }

    while (1) {
        uint64_t current;
        status = RPVC_Time_GetMicroseconds(&current);
        if (status != RPVC_OK) {
            return status;
        }
        if ((current - start) >= us) {
            break;
        }
        __asm__ volatile ("nop");
    }

    return RPVC_OK;
}

RPVC_Status_t RPVC_Time_DelayMs(uint32_t ms)
{
    return RPVC_Time_DelayUs(ms * 1000);
}

RPVC_Status_t RPVC_Time_GetTickFrequency(uint32_t *outFrequency)
{
    if (!RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }
    if (outFrequency == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    *outFrequency = g_tickFrequency;
    return RPVC_OK;
}
