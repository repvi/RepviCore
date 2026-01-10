/* RISC-V platform abstraction */
#include "RPVC_Platform.h"
#include <stdint.h>

void RPVC_Platform_Init(void)
{
    /* RISC-V platform-specific initialization */
}

const char* RPVC_Platform_GetName(void)
{
#if __riscv_xlen == 32
    return "RISC-V RV32";
#elif __riscv_xlen == 64
    return "RISC-V RV64";
#else
    return "RISC-V";
#endif
}

uint32_t RPVC_Platform_GetCapabilities(void)
{
    uint32_t caps = 0;
    
#if defined(__riscv_mul)
    caps |= (1 << 0); /* M extension (integer multiply/divide) */
#endif

#if defined(__riscv_atomic)
    caps |= (1 << 1); /* A extension (atomics) */
#endif

#if defined(__riscv_flen)
    caps |= (1 << 2); /* F/D extension (floating-point) */
#endif

#if defined(__riscv_compressed)
    caps |= (1 << 3); /* C extension (compressed instructions) */
#endif

    return caps;
}

void RPVC_Platform_Yield(void)
{
    /* Hint that we're waiting - NOP on basic RISC-V */
    __asm__ volatile ("nop");
}

void RPVC_Platform_Idle(void)
{
    /* Wait for interrupt */
    __asm__ volatile ("wfi");
}
