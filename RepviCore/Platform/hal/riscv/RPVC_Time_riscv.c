/* RISC-V time/tick management using machine timer */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
#include "RPVC_Time_common.h"
#include <stdint.h>

/* RISC-V CSR access */
#define read_csr(reg) ({ unsigned long __tmp; \
    __asm__ volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define write_csr(reg, val) ({ \
    __asm__ volatile ("csrw " #reg ", %0" :: "rK"(val)); })

/* Configuration - adjust based on your system */
#ifndef RPVC_RISCV_TIMEBASE_HZ
#define RPVC_RISCV_TIMEBASE_HZ  1000000  /* 1MHz default */
#endif

#ifndef RPVC_RISCV_TICK_HZ
#define RPVC_RISCV_TICK_HZ      1000     /* 1ms tick */
#endif

static uint32_t s_tickFrequency = RPVC_RISCV_TICK_HZ;

RPVC_Status_t RPVC_Time_Init(void)
{
    if (RPVC_Time_IsInitialized()) {
        return RPVC_ERR_INIT;
    }
    
    /* Set initial mtimecmp */
    uint64_t interval = RPVC_RISCV_TIMEBASE_HZ / RPVC_RISCV_TICK_HZ;
    
#if __riscv_xlen == 32
    uint64_t now = ((uint64_t)read_csr(mtimeh) << 32) | read_csr(mtime);
    uint64_t next = now + interval;
    write_csr(mtimecmph, (uint32_t)(next >> 32));
    write_csr(mtimecmp, (uint32_t)next);
#else
    uint64_t next = read_csr(mtime) + interval;
    write_csr(mtimecmp, next);
#endif
    
    /* Enable machine timer interrupt in mie */
    write_csr(mie, read_csr(mie) | (1 << 7));  /* MTIE bit */
    
    RPVC_Time_SetInitialized(true);
    return RPVC_OK;
}

/* GetTick is implemented in RPVC_Time_common.c */

uint64_t RPVC_Time_GetMicroseconds(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
#if __riscv_xlen == 32
    uint32_t high, low;
    do {
        high = (uint32_t)read_csr(mtimeh);
        low = (uint32_t)read_csr(mtime);
    } while (high != (uint32_t)read_csr(mtimeh));
    uint64_t cycles = ((uint64_t)high << 32) | low;
#else
    uint64_t cycles = read_csr(mtime);
#endif
    
    return cycles * 1000000ULL / RPVC_RISCV_TIMEBASE_HZ;
}

uint64_t RPVC_Time_GetTimeMilliseconds(void)
{
    return RPVC_Time_GetMicroseconds() / 1000;
}

/* TickDiff functions implemented in RPVC_Time_common.c */

void RPVC_Time_DelayUs(uint32_t us)
{
    uint64_t start = RPVC_Time_GetMicroseconds();
    while ((RPVC_Time_GetMicroseconds() - start) < us) {
        __asm__ volatile ("nop");
    }
}

void RPVC_Time_DelayMs(uint32_t ms)
{
    if (g_RPVC_Time_Initialized) {
        RPVC_Time_DelayUs(ms * 1000);
    }
}

uint32_t RPVC_Time_GetTickFrequency(void)
{
    if (!RPVC_Time_IsInitialized()) {
        return 0;
    }
    return s_tickFrequency;
}
