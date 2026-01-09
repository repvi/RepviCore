#include "event_api.h"
#include "EventManager.hpp"

using namespace repvicore;

RPVC_Status_t RPVC_Event_Init(void) 
{
    return EventManager::Init();
}

RPVC_Status_t RPVC_Event_RegisterHandler(RPVC_EventCallback_t callback) 
{

}

RPVC_Status_t RPVC_Event_Record(const RPVC_EventPacket_t *eventPacket) 
{

}

void RPVC_Event_Dispatch(void) 
{

}