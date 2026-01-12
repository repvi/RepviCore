/* ARM Cortex-M system control */
#include <stddef.h>
#include "RPVC_SYSTEM.h"
#include "RPVC_INTERRUPTS.h"
#include <stdint.h>

/* ARM Cortex-M System Control Block (SCB) registers */
#define SCB_BASE            (0xE000ED00UL)
#define SCB_CPUID           (*(volatile uint32_t*)(SCB_BASE + 0x00))
#define SCB_AIRCR           (*(volatile uint32_t*)(SCB_BASE + 0x0C))

/* Data Watchpoint and Trace (DWT) for cycle counter */
#define DWT_BASE            (0xE0001000UL)
#define DWT_CTRL            (*(volatile uint32_t*)(DWT_BASE + 0x00))
#define DWT_CYCCNT          (*(volatile uint32_t*)(DWT_BASE + 0x04))
#define DWT_CYCCNTENA       (1UL << 0)

/* Core Debug registers */
#define CoreDebug_BASE      (0xE000EDF0UL)
#define CoreDebug_DEMCR     (*(volatile uint32_t*)(CoreDebug_BASE + 0x0C))
#define CoreDebug_TRCENA    (1UL << 24)

static bool s_systemInitialized = false;

RPVC_Status_t RPVC_SYSTEM_Init(void)
{
    if (s_systemInitialized) {
        return RPVC_ERR_INIT;
    }

    /* Enable DWT cycle counter for performance monitoring */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__)
    CoreDebug_DEMCR |= CoreDebug_TRCENA;
    DWT_CYCCNT = 0;
    DWT_CTRL |= DWT_CYCCNTENA;
#endif

    s_systemInitialized = true;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SYSTEM_Deinit(void)
{
    if (!s_systemInitialized) {
        return RPVC_ERR_INIT;
    }
    s_systemInitialized = false;
    return RPVC_OK;
}

void RPVC_SYSTEM_Idle(void)
{
    __asm__ volatile ("wfi");
}

void RPVC_SYSTEM_Reset(void)
{
    /* AIRCR register: write key and set SYSRESETREQ */
    uint32_t aircr = SCB_AIRCR & 0x0000FFFF;
    SCB_AIRCR = 0x05FA0000 | aircr | (1UL << 2);
    
    /* Wait for reset */
    while(1) {
        __asm__ volatile ("nop");
    }
}

RPVC_Status_t RPVC_SYSTEM_GetCPUName(const char **cpuName)
{
    if (cpuName == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    
    uint32_t cpuid = SCB_CPUID;
    uint32_t partno = (cpuid >> 4) & 0xFFF;
    
    switch(partno) {
        case 0xC20: *cpuName = "Cortex-M0"; return RPVC_OK;
        case 0xC60: *cpuName = "Cortex-M0+"; return RPVC_OK;
        case 0xC21: *cpuName = "Cortex-M1"; return RPVC_OK;
        case 0xC23: *cpuName = "Cortex-M3"; return RPVC_OK;
        case 0xC24: *cpuName = "Cortex-M4"; return RPVC_OK;
        case 0xC27: *cpuName = "Cortex-M7"; return RPVC_OK;
        case 0xD20: *cpuName = "Cortex-M23"; return RPVC_OK;
        case 0xD21: *cpuName = "Cortex-M33"; return RPVC_OK;
        case 0xD22: *cpuName = "Cortex-M55"; return RPVC_OK;
        case 0xD23: *cpuName = "Cortex-M85"; return RPVC_OK;
        default:    *cpuName = "ARM Cortex-M"; return RPVC_OK;
    }
}

uint32_t RPVC_SYSTEM_GetCPUID(void)
{
    return SCB_CPUID;
}

RPVC_Status_t RPVC_SYSTEM_GetCycleCount(uint32_t *cycleCount)
{
    #if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__)
    if (!s_systemInitialized) {
        return RPVC_ERR_INIT;
    }
    if (cycleCount == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    *cycleCount = DWT_CYCCNT;
    return RPVC_OK;
#else
    /* Cycle counter not available on Cortex-M0/M0+ */
    *cycleCount = 0;
    return RPVC_ERR_NOT_READY;
#endif
}
