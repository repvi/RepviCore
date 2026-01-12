/* Common RPVC Time tick management - architecture independent */
#include <stddef.h>
#include "RPVC_Time_common.h"
#include "RPVC_TIME.h"

static volatile uint64_t s_rpvcTick = 0;
static bool g_RPVC_TIME_Initialized = false;

/* Weak function - user can override for custom tick behavior */
__attribute__((weak))
void RPVC_TIME_UserTickHook(void)
{
    /* Default: no user behavior */
}

/* Called from platform timer ISR - increments tick counter */
void RPVC_TIME_OnTick(void)
{
    s_rpvcTick++;
    RPVC_TIME_UserTickHook();
}

/* Mark system as initialized - called by platform-specific init */
void RPVC_TIME_SetInitialized(bool initialized)
{
    g_RPVC_TIME_Initialized = initialized;
}

/* Check if time system is initialized */
bool RPVC_TIME_IsInitialized(void)
{
    return g_RPVC_TIME_Initialized;
}

/* Get current tick count (64-bit) */
RPVC_Status_t RPVC_TIME_GetTick64(uint64_t *outTick)
{
    if (outTick == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    if (!g_RPVC_TIME_Initialized) {
        return RPVC_ERR_INIT;
    }
    *outTick = s_rpvcTick;
    return RPVC_OK;
}

/* Get current tick count (32-bit) */
RPVC_Status_t RPVC_TIME_GetTick(uint32_t *outTick)
{
    if (outTick == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    if (!g_RPVC_TIME_Initialized) {
        return RPVC_ERR_INIT;
    }
    *outTick = (uint32_t)s_rpvcTick;
    return RPVC_OK;
}

/* Tick difference calculation */
uint32_t RPVC_TIME_TickDiff(uint32_t start, uint32_t end)
{
    return end - start;
}

uint64_t RPVC_TIME_DiffUs(uint64_t start, uint64_t end)
{
    return end - start;
}

uint64_t RPVC_TIME_DiffMs(uint64_t start, uint64_t end)
{
    return end - start;
}
