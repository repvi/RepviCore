/* Xtensa platform abstraction */
#include "RPVC_Platform.h"
#include <stdint.h>
#include <xtensa/config/core.h>

void RPVC_Platform_Init(void)
{
    /* Xtensa platform-specific initialization */
}

const char* RPVC_Platform_GetName(void)
{
#if XCHAL_HAVE_FP
    return "Xtensa LX (FPU)";
#else
    return "Xtensa LX";
#endif
}

uint32_t RPVC_Platform_GetCapabilities(void)
{
    uint32_t caps = 0;
    
#if XCHAL_HAVE_FP
    caps |= (1 << 0); /* Floating-point unit */
#endif

#if XCHAL_HAVE_MAC16
    caps |= (1 << 1); /* 16-bit MAC */
#endif

#if XCHAL_HAVE_MUL32
    caps |= (1 << 2); /* 32-bit multiply */
#endif

#if XCHAL_HAVE_DIV32
    caps |= (1 << 3); /* 32-bit divide */
#endif

#if XCHAL_DCACHE_SIZE > 0
    caps |= (1 << 4); /* Data cache */
#endif

#if XCHAL_ICACHE_SIZE > 0
    caps |= (1 << 5); /* Instruction cache */
#endif

    return caps;
}

void RPVC_Platform_Yield(void)
{
    /* No specific yield instruction on Xtensa */
    __asm__ volatile ("nop");
}

void RPVC_Platform_Idle(void)
{
    /* Wait for interrupt - low power mode */
    __asm__ volatile ("waiti 0");
}
