/* x86/x64 time/tick management */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
#include "RPVC_Time_common.h"
#include <stdint.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

/* Configuration - adjust based on your system */
#ifndef RPVC_X86_TSC_FREQ_HZ
#define RPVC_X86_TSC_FREQ_HZ    2400000000ULL  /* Default 2.4GHz - should be calibrated */
#endif

#ifndef RPVC_X86_TICK_HZ
#define RPVC_X86_TICK_HZ        1000           /* 1ms tick */
#endif

static uint32_t s_tickFrequency = RPVC_X86_TICK_HZ;
static uint64_t s_tscFrequency = RPVC_X86_TSC_FREQ_HZ;

/* Read TSC (Time Stamp Counter) */
static inline uint64_t rdtsc(void)
{
#if defined(_MSC_VER)
    return __rdtsc();
#elif defined(__GNUC__) || defined(__clang__)
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#else
    return 0;
#endif
}

RPVC_Status_t RPVC_Time_Init(void)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }
    
    /* Platform-specific timer setup (PIT, APIC, HPET) would go here */
    /* This is a simplified implementation */
    
    RPVC_Time_SetInitialized(true);
    return RPVC_OK;
}

/* GetTick is implemented in RPVC_Time_common.c */

uint64_t RPVC_Time_GetMicroseconds(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    uint64_t tsc = rdtsc();
    return (tsc * 1000000ULL) / s_tscFrequency;
}

uint64_t RPVC_Time_GetTimeMilliseconds(void)
{
    return RPVC_Time_GetMicroseconds() / 1000;
}

/* TickDiff functions implemented in RPVC_Time_common.c */

void RPVC_Time_DelayUs(uint32_t us)
{
    if (!RPVC_Time_IsInitialized()) {
        return;
    }
    uint64_t start = RPVC_Time_GetMicroseconds();
    while ((RPVC_Time_GetMicroseconds() - start) < us) {
#if defined(_MSC_VER)
        _mm_pause();
#elif defined(__GNUC__) || defined(__clang__)
        __asm__ volatile("pause" ::: "memory");
#endif
    }
}

void RPVC_Time_DelayMs(uint32_t ms)
{
    RPVC_Time_DelayUs(ms * 1000);
}

uint32_t RPVC_Time_GetTickFrequency(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    return s_tickFrequency;
}
