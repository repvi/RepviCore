#include "RPVC_TIME.h"

static volatile uint32_t* g_systemTickCounter = NULL;
static bool g_isInitialized = false;

RPVC_Status_t RPVC_TIME_Init(const RPVC_TimeConfig_t *config)
{
    if (g_isInitialized) {
        return RPVC_ERR_STATE;
    }

    if (config == NULL || config->systemTickCounter == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    g_systemTickCounter = config->systemTickCounter;

    g_isInitialized = true;

    return RPVC_OK;
}

void RPVC_TIME_Deinit(void) 
{
    g_systemTickCounter = NULL;
    g_isInitialized = false;
}

bool RPVC_TIME_IsInitialized(void) 
{
    return g_isInitialized;
}

uint32_t RPVC_TIME_GetSystemTick(void) 
{
    if (g_systemTickCounter) {
        return *g_systemTickCounter;
    }
    else {
        return 0;
    }
}