#include "SoftwareBus.h"
#include "core/RPVC_MEMORYPOOL.h"

static bool g_isInitialized = false;

RPVC_Status_t RPVC_SOFTWAREBUS_INIT(const RPVC_SoftwareBusConfig_t* config) 
{
    if (!config) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (!RPVC_MEMORYPOOL_IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }

    if (g_isInitialized) {
        return RPVC_ERR_STATE;
    }

    g_isInitialized = true;

    return RPVC_OK;
}

RPVC_Status_t RPVC_SOFTWAREBUS_DEINIT(void) 
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    g_isInitialized = false;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SOFTWAREBUS_Subscribe(uint32_t messageId, uint32_t subsriberId) 
{

}

RPVC_Status_t RPVC_SOFTWAREBUS_Unsubscribe(uint32_t messageId, uint32_t subsriberId) 
{

}

RPVC_Status_t RPVC_SOFTWAREBUS_Publish(uint32_t messageId, const void* messageData, uint32_t messageSize) 
{

}

RPVC_Status_t RPVC_SOFTWAREBUS_Flush(uint32_t subscriberId)
{
    
}
