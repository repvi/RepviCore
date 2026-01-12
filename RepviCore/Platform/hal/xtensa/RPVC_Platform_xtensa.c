/* Xtensa PLATFORM abstraction */
#include "RPVC_PLATFORM.h"
#include <stdint.h>
#include <xtensa/config/core.h>

static bool s_PLATFORMInitialized = false;

RPVC_Status_t RPVC_PLATFORM_Init(void)
{
    if (s_PLATFORMInitialized) {
        return RPVC_ERR_INIT;
    }
    s_PLATFORMInitialized = true;
    return RPVC_OK;
    /* Xtensa PLATFORM-specific initialization */
}

RPVC_Status_t RPVC_PLATFORM_GetName(const char **name)
{
    if (name == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if XCHAL_HAVE_FP
    *name = "Xtensa LX (FPU)";
#else
    *name = "Xtensa LX";
#endif
    return RPVC_OK;
}

RPVC_PLATFORMCapabilities_t RPVC_PLATFORM_GetCapabilities(void)
{
    RPVC_PLATFORMCapabilities_t caps = 0;
    
#if XCHAL_HAVE_FP
    caps |= RPVC_CAP_HW_FPU;
#endif

#if XCHAL_HAVE_MAC16
    caps |= RPVC_CAP_XTENSA_MAC16;
#endif

#if XCHAL_HAVE_MUL32
    caps |= RPVC_CAP_XTENSA_MUL32;
#endif

#if XCHAL_HAVE_DIV32
    caps |= RPVC_CAP_XTENSA_DIV32;
#endif

#if XCHAL_DCACHE_SIZE > 0
    caps |= RPVC_CAP_HW_DCACHE;
#endif

#if XCHAL_ICACHE_SIZE > 0
    caps |= RPVC_CAP_HW_ICACHE;
#endif

    return caps;
}

void RPVC_PLATFORM_Yield(void)
{
    /* No specific yield instruction on Xtensa */
    __asm__ volatile ("nop");
}

void RPVC_PLATFORM_Idle(void)
{
    /* Wait for interrupt - low power mode */
    __asm__ volatile ("waiti 0");
}
