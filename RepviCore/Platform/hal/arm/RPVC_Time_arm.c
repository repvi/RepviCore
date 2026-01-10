/* ARM Cortex-M time/tick management using SysTick */
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

/* Configuration - adjust based on your system clock */
#ifndef RPVC_SYSTICK_FREQ_HZ
#define RPVC_SYSTICK_FREQ_HZ    1000  /* 1ms tick rate */
#endif

#ifndef RPVC_CPU_FREQ_HZ
#define RPVC_CPU_FREQ_HZ        48000000  /* Default 48MHz - override for your MCU */
#endif

static uint32_t s_tickFrequency = RPVC_SYSTICK_FREQ_HZ;

RPVC_Status_t RPVC_Time_Init(void)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    uint32_t ticks = (RPVC_CPU_FREQ_HZ / RPVC_SYSTICK_FREQ_HZ) - 1;
    
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

uint64_t RPVC_Time_GetMicroseconds(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    
    uint32_t ticks;
    uint32_t reload;
    uint32_t cycles;
    
    uint32_t state = RPVC_Interrupts_EnterCritical();
    ticks = (uint32_t)RPVC_Time_GetTick64();
    reload = SysTick_LOAD;
    cycles = reload - SysTick_VAL;
    RPVC_Interrupts_ExitCritical(state);
    
    /* Convert to microseconds */
    uint64_t us = (uint64_t)ticks * (1000000 / RPVC_SYSTICK_FREQ_HZ);
    us += (uint64_t)cycles * 1000000 / RPVC_CPU_FREQ_HZ;
    
    return us;
}

uint64_t RPVC_Time_GetTimeMilliseconds(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    return RPVC_Time_GetTick64() * (1000 / RPVC_SYSTICK_FREQ_HZ);
}

/* TickDiff functions implemented in RPVC_Time_common.c */

void RPVC_Time_DelayUs(uint32_t us)
{
    uint64_t start = RPVC_Time_GetMicroseconds();
    while((RPVC_Time_GetMicroseconds() - start) < us) {
        __asm volatile ("nop");
    }
}

void RPVC_Time_DelayMs(uint32_t ms)
{
    if (!RPVC_Time_IsInitialized()) {
        return;
    }
    uint32_t start = RPVC_Time_GetTick();
    uint32_t target = ms * RPVC_SYSTICK_FREQ_HZ / 1000;
    while((RPVC_Time_GetTick() - start) < target) {
        __asm volatile ("wfi");
    }
}

uint32_t RPVC_Time_GetTickFrequency(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    return s_tickFrequency;
}
