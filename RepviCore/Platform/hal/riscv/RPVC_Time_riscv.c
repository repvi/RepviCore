/* RISC-V TIME/tick management using machine TIMEr */
#include "RPVC_TIME.h"
#include "RPVC_INTERRUPTS.h"
#include "RPVC_TIME_common.h"
#include <stdint.h>

/* RISC-V CSR access */
#define read_csr(reg) ({ unsigned long __tmp; \
    __asm__ volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define write_csr(reg, val) ({ \
    __asm__ volatile ("csrw " #reg ", %0" :: "rK"(val)); })

static uint32_t g_tickFrequency = 0;

RPVC_Status_t RPVC_TIME_Init(const RPVC_TimeConfig_t *config)
{
    if (RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_INIT;
    }

    if (config == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }


    g_tickFrequency = config->tickHz;
    /* Set initial mTIMEcmp */
    uint64_t interval = config->systemHz / g_tickFrequency;
    
#if __riscv_xlen == 32
    uint64_t now = ((uint64_t)read_csr(mTIMEh) << 32) | read_csr(mTIME);
    uint64_t next = now + interval;
    write_csr(mTIMEcmph, (uint32_t)(next >> 32));
    write_csr(mTIMEcmp, (uint32_t)next);
#else
    uint64_t next = read_csr(mTIME) + interval;
    write_csr(mTIMEcmp, next);
#endif
    
    /* Enable machine TIMEr interrupt in mie */
    write_csr(mie, read_csr(mie) | (1 << 7));  /* MTIE bit */
    
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
#if __riscv_xlen == 32
    uint32_t high, low;
    do {
        high = (uint32_t)read_csr(mTIMEh);
        low = (uint32_t)read_csr(mTIME);
    } while (high != (uint32_t)read_csr(mTIMEh));
    uint64_t cycles = ((uint64_t)high << 32) | low;
#else
    uint64_t cycles = read_csr(mTIME);
#endif
    
    *outUs = cycles * 1000000ULL / g_tickFrequency;
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

    uint64_t start;
    RPVC_Status_t status = RPVC_TIME_GetMicroseconds(&start);
    if (status != RPVC_OK) {
        return status;
    }
    
    while (1) {
        uint64_t current;
        status = RPVC_TIME_GetMicroseconds(&current);
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

RPVC_Status_t RPVC_TIME_DelayMs(uint32_t ms)
{
    return RPVC_TIME_DelayUs(ms * 1000);
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
