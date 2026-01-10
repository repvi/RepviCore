/* Xtensa time/tick management */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
#include "RPVC_Time_common.h"
#include <stdint.h>
#include <xtensa/config/core.h>

/* Xtensa special register access */
#define RSR(reg, val) __asm__ volatile ("rsr %0, " #reg : "=a"(val))
#define WSR(reg, val) __asm__ volatile ("wsr %0, " #reg :: "a"(val))

/* Configuration - adjust based on your system */
#ifndef RPVC_XTENSA_CPU_FREQ_HZ
#define RPVC_XTENSA_CPU_FREQ_HZ  160000000  /* Default 160MHz (ESP32) */
#endif

#ifndef RPVC_XTENSA_TICK_HZ
#define RPVC_XTENSA_TICK_HZ      1000       /* 1ms tick */
#endif

static uint32_t s_tickFrequency = RPVC_XTENSA_TICK_HZ;
static uint32_t s_cyclesPerTick = 0;

RPVC_Status_t RPVC_Time_Init(void)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    s_cyclesPerTick = RPVC_XTENSA_CPU_FREQ_HZ / RPVC_XTENSA_TICK_HZ;
    
    /* Set up CCOMPARE0 for periodic interrupts */
    uint32_t ccount;
    RSR(CCOUNT, ccount);
    WSR(CCOMPARE0, ccount + s_cyclesPerTick);
    
    /* Enable timer interrupt at level 1 */
    uint32_t intenable;
    RSR(INTENABLE, intenable);
    intenable |= (1 << XCHAL_TIMER0_INTERRUPT);
    WSR(INTENABLE, intenable);
    
    RPVC_Time_SetInitialized(true);
    return RPVC_OK;
}

/* GetTick is implemented in RPVC_Time_common.c */

uint64_t RPVC_Time_GetMicroseconds(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    
    uint32_t ccount;
    uint32_t ticks;
    
    uint32_t state = RPVC_Interrupts_EnterCritical();
    RSR(CCOUNT, ccount);
    ticks = (uint32_t)RPVC_Time_GetTick64();
    RPVC_Interrupts_ExitCritical(state);
    
    /* Convert ticks to microseconds */
    uint64_t us = (uint64_t)ticks * (1000000 / RPVC_XTENSA_TICK_HZ);
    
    /* Add fractional part from cycle counter */
    us += (uint64_t)(ccount % s_cyclesPerTick) * 1000000 / RPVC_XTENSA_CPU_FREQ_HZ;
    
    return us;
}

uint64_t RPVC_Time_GetTimeMilliseconds(void)
{
    return (uint64_t)s_systemTicks * (1000 / RPVC_XTENSA_TICK_HZ);
}

/* TickDiff functions implemented in RPVC_Time_common.c */

void RPVC_Time_DelayUs(uint32_t us)
{
    if (!RPVC_Time_IsInitialized()) {
        return;
    }
    uint32_t cycles = (uint32_t)((uint64_t)us * RPVC_XTENSA_CPU_FREQ_HZ / 1000000);
    uint32_t start, current;
    
    RSR(CCOUNT, start);
    do {
        RSR(CCOUNT, current);
    } while ((current - start) < cycles);
}

void RPVC_Time_DelayMs(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        RPVC_Time_DelayUs(1000);
    }
}

uint32_t RPVC_Time_GetTickFrequency(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    return s_tickFrequency;
}
