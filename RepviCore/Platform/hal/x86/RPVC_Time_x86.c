/* x86/x64 time/tick management */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
#include "RPVC_Time_common.h"
#include <stdint.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

static uint32_t g_tickFrequency = 0;
static uint64_t g_tscFrequency = 0;

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

RPVC_Status_t RPVC_Time_Init(const RPVC_TimeConfig_t *config)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (config == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    
    g_tickFrequency = config->tickHz;
    g_tscFrequency = config->systemHz;
    /* Platform-specific timer setup (PIT, APIC, HPET) would go here */
    /* This is a simplified implementation */
    
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
    uint64_t tsc = rdtsc();
    *outUs = (tsc * 1000000ULL) / g_tscFrequency;
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

    uint64_t us;
    RPVC_Status_t status = RPVC_Time_GetMicroseconds(&us);
    if (status != RPVC_OK) {
        return status;
    }

    *outMs = us / 1000;
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
#if defined(_MSC_VER)
        _mm_pause();
#elif defined(__GNUC__) || defined(__clang__)
        __asm__ volatile("pause" ::: "memory");
#endif
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
