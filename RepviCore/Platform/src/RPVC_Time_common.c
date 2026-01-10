/* Common RPVC Time tick management - architecture independent */
#include "RPVC_Time_common.h"
#include "RPVC_Time.h"

static volatile uint64_t s_rpvcTick = 0;
static bool g_RPVC_Time_Initialized = false;

/* Weak function - user can override for custom tick behavior */
__attribute__((weak))
void RPVC_Time_UserTickHook(void)
{
    /* Default: no user behavior */
}

/* Called from platform timer ISR - increments tick counter */
void RPVC_Time_OnTick(void)
{
    s_rpvcTick++;
    RPVC_Time_UserTickHook();
}

/* Mark system as initialized - called by platform-specific init */
void RPVC_Time_SetInitialized(bool initialized)
{
    g_RPVC_Time_Initialized = initialized;
}

/* Check if time system is initialized */
bool RPVC_Time_IsInitialized(void)
{
    return g_RPVC_Time_Initialized;
}

/* Get current tick count (64-bit) */
uint64_t RPVC_Time_GetTick64(void)
{
    return s_rpvcTick;
}

/* Get current tick count (32-bit) */
uint32_t RPVC_Time_GetTick(void)
{
    if (!g_RPVC_Time_Initialized) {
        return 0;
    }
    return (uint32_t)s_rpvcTick;
}

/* Tick difference calculation */
uint32_t RPVC_Time_TickDiff(uint32_t start, uint32_t end)
{
    return end - start;
}

uint64_t RPVC_Time_TimeDiffUs(uint64_t start, uint64_t end)
{
    return end - start;
}

uint64_t RPVC_Time_TimeDiffMs(uint64_t start, uint64_t end)
{
    return end - start;
}
