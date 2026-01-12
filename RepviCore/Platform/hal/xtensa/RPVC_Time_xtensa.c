/* Xtensa TIME/tick management */
#include "RPVC_TIME.h"
#include "RPVC_INTERRUPTS.h"
#include "RPVC_TIME_common.h"
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

static uint32_t g_tickFrequency = 0;
static uint32_t g_cyclesPerTick = 0;

RPVC_Status_t RPVC_TIME_Init(const RPVC_TimeConfig_t *config)
{
    if (RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (config == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    g_tickFrequency = config->tickHz;
    g_cyclesPerTick = config->systemHz / config->tickHz;
    
    /* Set up CCOMPARE0 for periodic interrupts */
    uint32_t ccount;
    RSR(CCOUNT, ccount);
    WSR(CCOMPARE0, ccount + g_cyclesPerTick);
    
    /* Enable TIMEr interrupt at level 1 */
    uint32_t intenable;
    RSR(INTENABLE, intenable);
    intenable |= (1 << XCHAL_TIMER0_INTERRUPT);
    WSR(INTENABLE, intenable);
    
    RPVC_TIME_SetInitialized(true);
    return RPVC_OK;
}

/* GetTick is implemented in RPVC_TIME_common.c */

RPVC_Status_t RPVC_TIME_GetMicroseconds(uint64_t *outUs)
{
    if (!RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (outUs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    
    uint32_t ccount;
    uint64_t ticks;
    
    uint32_t state = RPVC_INTERRUPTS_EnterCritical();
    RSR(CCOUNT, ccount);
    RPVC_Status_t status = RPVC_TIME_GetTick64(&ticks);
    if (status != RPVC_OK) {
        RPVC_INTERRUPTS_ExitCritical(state);
        return status;
    }
    RPVC_INTERRUPTS_ExitCritical(state);
    
    /* Convert ticks to microseconds */
    uint64_t us = ticks * (1000000 / g_tickFrequency);
    
    /* Add fractional part from cycle counter */
    if (g_cyclesPerTick > 0) {
        us += (uint64_t)(ccount % g_cyclesPerTick) * 1000000 / g_cyclesPerTick;
    }
    
    *outUs = us;
    return RPVC_OK;
}

RPVC_Status_t RPVC_TIME_GetMilliseconds(uint64_t *outMs)
{
    if (!RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (outMs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    uint64_t us;
    RPVC_Status_t status = RPVC_TIME_GetMicroseconds(&us);
    if (status != RPVC_OK) {
        return status;
    }

    *outMs = us / 1000;
    return RPVC_OK;
}

/* TickDiff functions implemented in RPVC_TIME_common.c */

RPVC_Status_t RPVC_TIME_DelayUs(uint32_t us)
{
    if (!RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }
    uint32_t cycles = (uint32_t)((uint64_t)us * RPVC_XTENSA_CPU_FREQ_HZ / 1000000);
    uint32_t start, current;
    
    RSR(CCOUNT, start);
    do {
        RSR(CCOUNT, current);
    } while ((current - start) < cycles);
    return RPVC_OK;
}

RPVC_Status_t RPVC_TIME_DelayMs(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        RPVC_TIME_DelayUs(1000);
    }
    return RPVC_OK;
}

RPVC_Status_t RPVC_TIME_GetTickFrequency(uint32_t *outFrequency)
{
    if (!RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }
    if (outFrequency == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    *outFrequency = g_tickFrequency;
    return RPVC_OK;
}
