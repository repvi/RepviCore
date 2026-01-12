/* RISC-V PLATFORM abstraction */
#include "RPVC_PLATFORM.h"
#include <stdint.h>

static bool s_PLATFORMInitialized = false;

RPVC_Status_t RPVC_PLATFORM_Init(void)
{
    if (s_PLATFORMInitialized) {
        return RPVC_ERR_INIT;
    }
    s_PLATFORMInitialized = true;
    return RPVC_OK;
    /* RISC-V PLATFORM-specific initialization */
}

RPVC_Status_t RPVC_PLATFORM_GetName(const char **name)
{
    if (name == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if __riscv_xlen == 32
    *name = "RISC-V RV32";
#elif __riscv_xlen == 64
    *name = "RISC-V RV64";
#else
    *name = "RISC-V";
#endif
    return RPVC_OK;
}

RPVC_PLATFORMCapabilities_t RPVC_PLATFORM_GetCapabilities(void)
{
    RPVC_PLATFORMCapabilities_t caps = 0;
    
#if defined(__riscv_mul)
    caps |= RPVC_CAP_RISCV_M;  /* M extension (integer multiply/divide) */
#endif

#if defined(__riscv_atomic)
    caps |= RPVC_CAP_RISCV_A;  /* A extension (atomics) */
#endif

#if defined(__riscv_flen)
    #if __riscv_flen >= 64
        caps |= RPVC_CAP_RISCV_D;  /* D extension (double FP) */
    #endif
    #if __riscv_flen >= 32
        caps |= RPVC_CAP_RISCV_F;  /* F extension (single FP) */
    #endif
    caps |= RPVC_CAP_HW_FPU;
#endif

#if defined(__riscv_compressed)
    caps |= RPVC_CAP_RISCV_C;  /* C extension (compressed instructions) */
#endif

#if defined(__riscv_vector)
    caps |= RPVC_CAP_RISCV_V;  /* V extension (vector) */
#endif

    return caps;
}

void RPVC_PLATFORM_Yield(void)
{
    /* Hint that we're waiting - NOP on basic RISC-V */
    __asm__ volatile ("nop");
}

void RPVC_PLATFORM_Idle(void)
{
    /* Wait for interrupt */
    __asm__ volatile ("wfi");
}
