#include "event_api.h"
#include "core/TimeSystem/RPVC_TIME.h"
#include "EventManager.hpp"

using namespace repvicore;

static bool g_isInitialized = false;

RPVC_Status_t RPVC_Event_Init(void) 
{
    if (!RPVC_TIME_IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }

    if (g_isInitialized) {
        return RPVC_ERR_STATE;
    }

    RPVC_Status_t status =  EventManager::Init();
    if (status == RPVC_OK) {
        g_isInitialized = true;
    }
    return status;
}

RPVC_Status_t RPVC_Event_RegisterHandler(RPVC_EventCallback_t callback) 
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (callback == nullptr) {
        return RPVC_ERR_INVALID_ARG;
    }

    return EventManager::registerHandler(callback);
}

RPVC_Status_t RPVC_Event_Record(const RPVC_EventPacket_t *eventPacket) 
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (eventPacket == nullptr) {
        return RPVC_ERR_INVALID_ARG;
    }

    return EventManager::recordEvent(eventPacket);
}

void RPVC_Event_Dispatch(void) 
{
    if (!g_isInitialized) {
        return;
    }
    EventManager::dispatch();
}