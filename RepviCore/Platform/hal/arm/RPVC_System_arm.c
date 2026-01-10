/* ARM Cortex-M system control */
#include "RPVC_System.h"
#include "RPVC_Interrupts.h"
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

static uint8_t s_systemInitialized = 0;

RPVC_Status_t RPVC_System_Init(void)
{
    if (s_systemInitialized) {
        return RPVC_OK;
    }

    /* Enable DWT cycle counter for performance monitoring */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__)
    CoreDebug_DEMCR |= CoreDebug_TRCENA;
    DWT_CYCCNT = 0;
    DWT_CTRL |= DWT_CYCCNTENA;
#endif

    s_systemInitialized = 1;
    return RPVC_OK;
}

RPVC_Status_t RPVC_System_Deinit(void)
{
    s_systemInitialized = 0;
    return RPVC_OK;
}

void RPVC_System_Idle(void)
{
    __asm volatile ("wfi");
}

void RPVC_System_Reset(void)
{
    /* AIRCR register: write key and set SYSRESETREQ */
    uint32_t aircr = SCB_AIRCR & 0x0000FFFF;
    SCB_AIRCR = 0x05FA0000 | aircr | (1UL << 2);
    
    /* Wait for reset */
    while(1) {
        __asm volatile ("nop");
    }
}

const char* RPVC_System_GetCPUName(void)
{
    uint32_t cpuid = SCB_CPUID;
    uint32_t partno = (cpuid >> 4) & 0xFFF;
    
    switch(partno) {
        case 0xC20: return "Cortex-M0";
        case 0xC60: return "Cortex-M0+";
        case 0xC21: return "Cortex-M1";
        case 0xC23: return "Cortex-M3";
        case 0xC24: return "Cortex-M4";
        case 0xC27: return "Cortex-M7";
        case 0xD20: return "Cortex-M23";
        case 0xD21: return "Cortex-M33";
        case 0xD22: return "Cortex-M55";
        case 0xD23: return "Cortex-M85";
        default:    return "ARM Cortex-M";
    }
}

uint32_t RPVC_System_GetCPUID(void)
{
    return SCB_CPUID;
}

uint32_t RPVC_System_GetCycleCount(void)
{
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__)
    return DWT_CYCCNT;
#else
    /* Cycle counter not available on Cortex-M0/M0+ */
    return 0;
#endif
}
