/* Xtensa system control */
#include "RPVC_System.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>
#include <xtensa/config/core.h>

/* Xtensa special registers access */
#define RSR(reg, val) __asm__ volatile ("rsr %0, " #reg : "=a"(val))
#define WSR(reg, val) __asm__ volatile ("wsr %0, " #reg :: "a"(val))

static uint8_t s_systemInitialized = 0;

RPVC_Status_t RPVC_System_Init(void)
{
    if (s_systemInitialized) {
        return RPVC_OK;
    }
    
    /* Xtensa system initialization */
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
    __asm__ volatile ("waiti 0");
}

void RPVC_System_Reset(void)
{
    /* Xtensa doesn't have a standard software reset */
    /* Platform-specific implementation needed (e.g., ESP32 uses RTC_CNTL) */
    while(1) {
        __asm__ volatile ("nop");
    }
}

const char* RPVC_System_GetCPUName(void)
{
#if defined(CONFIG_IDF_TARGET_ESP32)
    return "ESP32 (Xtensa LX6)";
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return "ESP32-S2 (Xtensa LX7)";
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return "ESP32-S3 (Xtensa LX7)";
#elif XCHAL_HAVE_FP
    return "Xtensa LX7";
#else
    return "Xtensa LX6";
#endif
}

uint32_t RPVC_System_GetCPUID(void)
{
    uint32_t prid;
    RSR(PRID, prid);
    return prid;
}

uint32_t RPVC_System_GetCycleCount(void)
{
    uint32_t ccount;
    RSR(CCOUNT, ccount);
    return ccount;
}
