/* RISC-V platform abstraction */
#include "RPVC_Platform.h"
#include <stdint.h>

static bool s_platformInitialized = false;

RPVC_Status_t RPVC_Platform_Init(void)
{
    if (s_platformInitialized) {
        return RPVC_ERR_INIT;
    }
    s_platformInitialized = true;
    return RPVC_OK;
    /* RISC-V platform-specific initialization */
}

RPVC_Status_t RPVC_Platform_GetName(const char **name)
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

RPVC_PlatformCapabilities_t RPVC_Platform_GetCapabilities(void)
{
    RPVC_PlatformCapabilities_t caps = 0;
    
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
