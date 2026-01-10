/* ARM Cortex-M platform abstraction */
#include "RPVC_Platform.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>

void RPVC_Platform_Init(void)
{
    /* Platform-specific initialization */
    /* Configure system clocks, peripherals, etc. */
}

const char* RPVC_Platform_GetName(void)
{
#if defined(__ARM_ARCH_6M__)
    return "ARM Cortex-M0/M0+";
#elif defined(__ARM_ARCH_7M__)
    return "ARM Cortex-M3";
#elif defined(__ARM_ARCH_7EM__)
    #if defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
        return "ARM Cortex-M4F/M7";
    #else
        return "ARM Cortex-M4";
    #endif
#elif defined(__ARM_ARCH_8M_BASE__)
    return "ARM Cortex-M23";
#elif defined(__ARM_ARCH_8M_MAIN__)
    return "ARM Cortex-M33/M35P";
#else
    return "ARM Cortex-M";
#endif
}

uint32_t RPVC_Platform_GetCapabilities(void)
{
    uint32_t caps = 0;
    
#if defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
    caps |= (1 << 0); /* DSP extensions */
#endif

#if defined(__ARM_FP)
    caps |= (1 << 1); /* FPU present */
#endif

#if defined(__ARM_FEATURE_MVE)
    caps |= (1 << 2); /* M-Profile Vector Extension (Helium) */
#endif

    return caps;
}

void RPVC_Platform_Yield(void)
{
    /* Hint to the processor that we're in a spin-wait loop */
    __asm volatile ("yield");
}

void RPVC_Platform_Idle(void)
{
    /* Wait for interrupt - low power mode */
    __asm volatile ("wfi");
}
