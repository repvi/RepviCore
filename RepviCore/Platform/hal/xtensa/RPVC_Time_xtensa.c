/* Xtensa time/tick management */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
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

static volatile uint32_t s_systemTicks = 0;
static uint32_t s_tickFrequency = RPVC_XTENSA_TICK_HZ;
static uint32_t s_cyclesPerTick = 0;

/* Timer interrupt handler - must be registered with interrupt dispatcher */
void xtensa_timer_handler(void)
{
    uint32_t ccompare;
    
    s_systemTicks++;
    
    /* Update compare register for next interrupt */
    RSR(CCOMPARE0, ccompare);
    ccompare += s_cyclesPerTick;
    WSR(CCOMPARE0, ccompare);
    
    /* Clear interrupt */
    uint32_t intclear = (1 << XCHAL_TIMER0_INTERRUPT);
    WSR(INTCLEAR, intclear);
}

RPVC_Status_t RPVC_Time_Init(void)
{
    s_systemTicks = 0;
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
    
    return RPVC_OK;
}

uint32_t RPVC_Time_GetTick(void)
{
    return s_systemTicks;
}

uint64_t RPVC_Time_GetMicroseconds(void)
{
    uint32_t ccount;
    uint32_t ticks;
    
    uint32_t state = RPVC_Interrupts_EnterCritical();
    RSR(CCOUNT, ccount);
    ticks = s_systemTicks;
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

uint32_t RPVC_Time_TickDiff(uint32_t start, uint32_t end)
{
    return end - start;
}

uint64_t RPVC_Time_TimeDiffUs(uint64_t start, uint64_t end)
{
    return end - start;
}

uint64_t RPVC_Time_TimeDiffMs(uint64_t start, uint64_t end)
{
    return end - start;
}

void RPVC_Time_DelayUs(uint32_t us)
{
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
    return s_tickFrequency;
}
