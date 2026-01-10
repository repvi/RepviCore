/* RISC-V time/tick management using machine timer */
#include "RPVC_Time.h"
#include "RPVC_Interrupts.h"
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

static volatile uint32_t s_systemTicks = 0;
static uint32_t s_tickFrequency = RPVC_RISCV_TICK_HZ;

/* Machine timer interrupt handler - must be called from trap handler */
void mtimer_handler(void)
{
    s_systemTicks++;
    
    /* Schedule next interrupt */
    uint64_t next = read_csr(mtime) + (RPVC_RISCV_TIMEBASE_HZ / RPVC_RISCV_TICK_HZ);
    
#if __riscv_xlen == 32
    write_csr(mtimecmph, (uint32_t)(next >> 32));
    write_csr(mtimecmp, (uint32_t)next);
#else
    write_csr(mtimecmp, next);
#endif
}

RPVC_Status_t RPVC_Time_Init(void)
{
    s_systemTicks = 0;
    
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
    
    return RPVC_OK;
}

uint32_t RPVC_Time_GetTick(void)
{
    return s_systemTicks;
}

uint64_t RPVC_Time_GetMicroseconds(void)
{
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
    uint64_t start = RPVC_Time_GetMicroseconds();
    while ((RPVC_Time_GetMicroseconds() - start) < us) {
        __asm__ volatile ("nop");
    }
}

void RPVC_Time_DelayMs(uint32_t ms)
{
    RPVC_Time_DelayUs(ms * 1000);
}

uint32_t RPVC_Time_GetTickFrequency(void)
{
    return s_tickFrequency;
}
