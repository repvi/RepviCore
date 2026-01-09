#include "RPVC_TIME.h"

static volatile uint32_t* g_systemTickCounter = NULL;

void RPVC_TIME_Init(void) 
{
    g_systemTickCounter = NULL;
}

void RPVC_TIME_Deinit(void) 
{
    g_systemTickCounter = NULL;
}

void RPVC_TIME_Configure(volatile uint32_t* systemTickCounter) 
{
    g_systemTickCounter = systemTickCounter;
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