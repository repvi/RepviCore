/* x86/x64 platform abstraction */
#include "RPVC_Platform.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

void RPVC_Platform_Init(void)
{
    /* x86 platform-specific initialization */
}

const char* RPVC_Platform_GetName(void)
{
#if defined(__x86_64__) || defined(_M_X64)
    return "x86-64";
#else
    return "x86";
#endif
}

uint32_t RPVC_Platform_GetCapabilities(void)
{
    uint32_t caps = 0;
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
    
    /* Check for various extensions */
    if (edx & (1 << 23)) caps |= (1 << 0); /* MMX */
    if (edx & (1 << 25)) caps |= (1 << 1); /* SSE */
    if (edx & (1 << 26)) caps |= (1 << 2); /* SSE2 */
    if (ecx & (1 << 0))  caps |= (1 << 3); /* SSE3 */
    if (ecx & (1 << 9))  caps |= (1 << 4); /* SSSE3 */
    if (ecx & (1 << 19)) caps |= (1 << 5); /* SSE4.1 */
    if (ecx & (1 << 20)) caps |= (1 << 6); /* SSE4.2 */
    if (ecx & (1 << 28)) caps |= (1 << 7); /* AVX */
    
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
