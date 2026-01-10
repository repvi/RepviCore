#include "repvicore.h"
#include "RPVC_Time.h"
#include "event_api.h"

RPVC_Status_t RPVC_Init(void)
{
    RPVC_Status_t status;
    
    status = RPVC_Time_Init();
    if (status != RPVC_OK) {
        return status;
    }

    status = RPVC_Event_Init();
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