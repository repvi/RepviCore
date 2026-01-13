#include "repvicore.h"
#include "RPVC_TIME.h"
#include "event_api.h"
#include "SoftwareBus.h"

RPVC_Status_t RPVC_Init(void)
{
    RPVC_Status_t status;
    
    RPVC_TimeConfig_t timeConfig = {
        .systemHz = 1000000, // Example system frequency in Hz
        .tickHz = 1000       // Example tick frequency in Hz
    };
    
    status = RPVC_TIME_Init(&timeConfig);
    if (status != RPVC_OK) {
        return status;
    }

    status = RPVC_EVENT_Init();
    if (status != RPVC_OK) {
        return status;
    }

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