/* Xtensa system control */
#include "RPVC_System.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>
#include <xtensa/config/core.h>

/* Xtensa special registers access */
#define RSR(reg, val) __asm__ volatile ("rsr %0, " #reg : "=a"(val))
#define WSR(reg, val) __asm__ volatile ("wsr %0, " #reg :: "a"(val))

static uint8_t s_systemInitialized = 0;

RPVC_Status_t RPVC_SYSTEM_Init(void)
{
    if (s_systemInitialized) {
        return RPVC_OK;
    }
    
    /* Xtensa system initialization */
    s_systemInitialized = 1;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SYSTEM_Deinit(void)
{
    if (!s_systemInitialized) {
        return RPVC_ERR_INIT;
    }
    s_systemInitialized = 0;
    return RPVC_OK;
}

void RPVC_SYSTEM_Idle(void)
{
    __asm__ volatile ("waiti 0");
}

void RPVC_SYSTEM_Reset(void)
{
    /* Xtensa doesn't have a standard software reset */
    /* Platform-specific implementation needed (e.g., ESP32 uses RTC_CNTL) */
    while(1) {
        __asm__ volatile ("nop");
    }
}

RPVC_Status_t RPVC_SYSTEM_GetCPUName(const char **cpuName)
{
    if (cpuName == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if defined(CONFIG_IDF_TARGET_ESP32)
    *cpuName = "ESP32 (Xtensa LX6)";
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
    *cpuName = "ESP32-S2 (Xtensa LX7)";
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    *cpuName = "ESP32-S3 (Xtensa LX7)";
#elif XCHAL_HAVE_FP
    *cpuName = "Xtensa LX7";
#else
    *cpuName = "Xtensa LX6";
#endif
    return RPVC_OK;
}

uint32_t RPVC_SYSTEM_GetCPUID(void)
{
    uint32_t prid;
    RSR(PRID, prid);
    return prid;
}

RPVC_Status_t RPVC_SYSTEM_GetCycleCount(uint32_t *cycleCount)
{
    if (cycleCount == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    uint32_t ccount;
    RSR(CCOUNT, ccount);
    *cycleCount = ccount;
    return RPVC_OK;
}
