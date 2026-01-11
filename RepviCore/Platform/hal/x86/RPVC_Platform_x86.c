/* x86/x64 platform abstraction */
#include "RPVC_Platform.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

static bool s_platformInitialized = false;

RPVC_Status_t RPVC_Platform_Init(void)
{
    if (s_platformInitialized) {
        return RPVC_ERR_INIT;
    }

    s_platformInitialized = true;
    
    /* x86 platform-specific initialization */
}

RPVC_Status_t RPVC_Platform_GetName(const char **name)
{
    if (name == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if defined(__x86_64__) || defined(_M_X64)
    *name = "x86-64";
#else
    *name = "x86";
#endif
    return RPVC_OK;
}

RPVC_PlatformCapabilities_t RPVC_Platform_GetCapabilities(void)
{
    RPVC_PlatformCapabilities_t caps = 0;
    uint32_t eax, ebx, ecx, edx;
    
    /* Check CPUID support */
#if defined(_MSC_VER)
    int cpuInfo[4];
    __cpuid(cpuInfo, 1);
    ecx = cpuInfo[2];
    edx = cpuInfo[3];
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile(
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(1)
    );
#endif
    
    /* CPUID leaf 1: Basic feature flags */
    if (edx & (1 << 23)) caps |= RPVC_CAP_SIMD_MMX;
    if (edx & (1 << 25)) caps |= RPVC_CAP_SIMD_SSE;
    if (edx & (1 << 26)) caps |= RPVC_CAP_SIMD_SSE2;
    if (ecx & (1 << 0))  caps |= RPVC_CAP_SIMD_SSE3;
    if (ecx & (1 << 9))  caps |= RPVC_CAP_SIMD_SSSE3;
    if (ecx & (1 << 19)) caps |= RPVC_CAP_SIMD_SSE41;
    if (ecx & (1 << 20)) caps |= RPVC_CAP_SIMD_SSE42;
    if (ecx & (1 << 28)) caps |= RPVC_CAP_SIMD_AVX;
    
    /* Check extended features (CPUID leaf 7) */
#if defined(_MSC_VER)
    __cpuidex(cpuInfo, 7, 0);
    ebx = cpuInfo[1];
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile(
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(7), "c"(0)
    );
#endif
    
    if (ebx & (1 << 5))  caps |= RPVC_CAP_SIMD_AVX2;
    if (ebx & (1 << 16)) caps |= RPVC_CAP_SIMD_AVX512;
    
    /* x86 always has FPU in modern processors */
    caps |= RPVC_CAP_HW_FPU;
    
    return caps;
}

void RPVC_Platform_Yield(void)
{
    /* x86 PAUSE instruction - hint for spin-wait loops */
#if defined(_MSC_VER)
    _mm_pause();
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile("pause" ::: "memory");
#endif
}

void RPVC_Platform_Idle(void)
{
    /* HLT instruction - halt until interrupt */
#if defined(_MSC_VER)
    __halt();
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile("hlt");
#endif
}
