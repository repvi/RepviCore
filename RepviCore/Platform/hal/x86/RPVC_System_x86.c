/* x86/x64 SYSTEM control */
#include "RPVC_SYSTEM.h"
#include "RPVC_INTERRUPTS.h"
#include <stdint.h>
#include <string.h>

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

static uint8_t s_SYSTEMInitialized = 0;

/* x86 CPUID instruction wrapper */
static void cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
#if defined(_MSC_VER)
    int cpuInfo[4];
    __cpuid(cpuInfo, leaf);
    *eax = cpuInfo[0];
    *ebx = cpuInfo[1];
    *ecx = cpuInfo[2];
    *edx = cpuInfo[3];
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile(
        "cpuid"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(leaf), "c"(0)
    );
#endif
}

RPVC_Status_t RPVC_SYSTEM_Init(void)
{
    if (s_SYSTEMInitialized) {
        return RPVC_OK;
    }
    
    s_SYSTEMInitialized = 1;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SYSTEM_Deinit(void)
{
    if (!s_SYSTEMInitialized) {
        return RPVC_ERR_INIT;
    }
    s_SYSTEMInitialized = 0;
    return RPVC_OK;
}

void RPVC_SYSTEM_Idle(void)
{
#if defined(_MSC_VER)
    __halt();
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile("hlt");
#endif
}

void RPVC_SYSTEM_Reset(void)
{
    /* x86 keyboard controller reset (legacy method) */
    /* Or use triple-fault method - not implemented here for safety */
#if defined(_MSC_VER)
    __outbyte(0x64, 0xFE);
#elif defined(__GNUC__) || defined(__clang__)
    __asm__ volatile("outb %0, $0x64" :: "a"((uint8_t)0xFE));
#endif
    
    while(1) {
        __asm__ volatile("hlt");
    }
}

RPVC_Status_t RPVC_SYSTEM_GetCPUName(const char **cpuName)
{
    if (cpuName == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    static char cpu_brand[49] = {0};
    uint32_t eax, ebx, ecx, edx;
    
    if (cpu_brand[0] == 0) {
        /* Check if extended CPUID is available */
        cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
        if (eax >= 0x80000004) {
            /* Get CPU brand string */
            uint32_t *brand = (uint32_t*)cpu_brand;
            cpuid(0x80000002, &brand[0], &brand[1], &brand[2], &brand[3]);
            cpuid(0x80000003, &brand[4], &brand[5], &brand[6], &brand[7]);
            cpuid(0x80000004, &brand[8], &brand[9], &brand[10], &brand[11]);
        } else {
            /* Fallback to generic name */
#if defined(__x86_64__) || defined(_M_X64)
            strcpy(cpu_brand, "x86-64 CPU");
#else
            strcpy(cpu_brand, "x86 CPU");
#endif
        }
    }
    
    *cpuName = cpu_brand;

    return RPVC_OK;
}

uint32_t RPVC_SYSTEM_GetCPUID(void)
{
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    return eax;
}

RPVC_Status_t RPVC_SYSTEM_GetCycleCount(uint32_t *cycleCount)
{
    if (cycleCount == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if defined(_MSC_VER)
    *cycleCount = (uint32_t)__rdtsc();
    return RPVC_OK;
#elif defined(__GNUC__) || defined(__clang__)
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    *cycleCount = lo;
    return RPVC_OK;
#else
    *cycleCount = 0;
    return RPVC_ERR_NOT_READY;
#endif
}
