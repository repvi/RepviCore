#include "repvicore.h"
#include "core/TimeSystem/RPVC_TIME.h"
#include "event_api.h"

const RPVC_TimeConfig_t timeConfig = {
    .systemTickCounter = NULL // Replace NULL with actual system tick counter pointer
};

RPVC_Status_t RPVC_Init(void)
{
    RPVC_TIME_Init(&timeConfig);
    RPVC_Event_Init();

    return RPVC_OK; // Added return statement
}

RPVC_Status_t RPVC_Shutdown(void) 
{
    return RPVC_ERR_NOT_READY;
}

RPVC_Status_t RPVC_Deinit(void) 
{
    return RPVC_Shutdown();
    // do other things after shutdown if needed
}