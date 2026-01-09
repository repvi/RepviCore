#include "EventManager.hpp"
#include <cstring>

using namespace repvicore;

RPVC_Status_t EventManager::Init(void) 
{
    std::memset(&eventCallback, 0, sizeof(eventCallback));
    std::memset(&eventQueue, 0, sizeof(eventQueue));
    eventCallbackCount = 0;
    head = 0;
    tail = 0;
    return RPVC_OK;
}

RPVC_Status_t EventManager::registerHandler(RPVC_EventCallback_t callback)
{
    if (callback == nullptr) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (eventCallbackCount < MAX_EVENT_CALLBACKS) {
        eventCallback[eventCallbackCount++] = callback;
        return RPVC_OK;
    }
    else {
        return RPVC_ERR_OUT_OF_RANGE;
    }
}

RPVC_Status_t EventManager::recordEvent(const RPVC_EventPacket_t *eventPacket)
{
    if (eventPacket == nullptr) {
        return RPVC_ERR_INVALID_ARG;
    }
    
    uint8_t next = (tail + 1) % MAX_EVENT_QUEUE_SIZE;
    if (next != head) { // Check for queue full
        eventQueue[tail] = *eventPacket;
        tail = next;
        return RPVC_OK;
    }
    else {
        return RPVC_ERR_OUT_OF_RANGE; // Queue full
    }
}

void EventManager::dispatch(void)
{
    while (head != tail) { // While queue not empty
        const RPVC_EventPacket_t &eventPacket = eventQueue[head];
        for (uint8_t i = 0; i < eventCallbackCount; ++i) {
            if (eventCallback[i]) {
                eventCallback[i](&eventPacket);
            }
        }
        head = (head + 1) % MAX_EVENT_QUEUE_SIZE;
    }
}