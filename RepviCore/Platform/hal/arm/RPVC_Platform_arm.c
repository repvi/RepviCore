/* ARM Cortex-M PLATFORM abstraction */
#include <stddef.h>
#include "RPVC_PLATFORM.h"
#include "RPVC_INTERRUPTS.h"
#include <stdint.h>

static bool g_PLATFORMInitialized = false;

RPVC_Status_t RPVC_PLATFORM_Init(void)
{
    if (g_PLATFORMInitialized) {
        return RPVC_ERR_INIT;
    }
    /* PLATFORM-specific initialization */
    /* Configure system clocks, peripherals, etc. */
    g_PLATFORMInitialized = true;
    return RPVC_OK;
}

RPVC_Status_t RPVC_PLATFORM_GetName(const char **name)
{
    if (name == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if defined(__ARM_ARCH_6M__)
    *name = "ARM Cortex-M0/M0+";
#elif defined(__ARM_ARCH_7M__)
    *name = "ARM Cortex-M3";
#elif defined(__ARM_ARCH_7EM__)
    #if defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
        *name = "ARM Cortex-M4F/M7";
    #else
        *name = "ARM Cortex-M4";
    #endif
#elif defined(__ARM_ARCH_8M_BASE__)
    *name = "ARM Cortex-M23";
#elif defined(__ARM_ARCH_8M_MAIN__)
    *name = "ARM Cortex-M33/M35P";
#else
    *name = "ARM Cortex-M";
#endif
    return RPVC_OK;
}

RPVC_PlatformCapabilities_t RPVC_PLATFORM_GetCapabilities(void)
{
    RPVC_PlatformCapabilities_t caps = 0;
    
#if defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
    caps |= RPVC_CAP_ARM_DSP;
#endif

#if defined(__ARM_FP)
    caps |= RPVC_CAP_HW_FPU;
#endif

#if defined(__ARM_NEON)
    caps |= RPVC_CAP_SIMD_NEON;
#endif

#if defined(__ARM_FEATURE_MVE)
    caps |= RPVC_CAP_ARM_MVE;  /* Helium */
#endif

#if defined(__ARM_FEATURE_SVE)
    caps |= RPVC_CAP_SIMD_SVE;
#endif

    return caps;
}

void RPVC_PLATFORM_Yield(void)
{
    /* Hint to the processor that we're in a spin-wait loop */
    __asm__ volatile ("yield");
}

void RPVC_PLATFORM_Idle(void)
{
    /* Wait for interrupt - low power mode */
    __asm__ volatile ("wfi");
}
