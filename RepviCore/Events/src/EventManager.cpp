#include "EventManager.hpp"
#include <cstring>

using namespace repvicore;

RPVC_EventCallback_t EventManager::eventCallbacks[MAX_EVENT_CALLBACKS];
uint8_t EventManager::eventCallbackCount;

RPVC_EventPacket_t EventManager::eventQueue[MAX_EVENT_QUEUE_SIZE];
uint8_t EventManager::head = 0;
uint8_t EventManager::tail = 0;

RPVC_Status_t EventManager::Init(void) 
{
    std::memset(&eventCallbacks, 0, sizeof(eventCallbacks));
    std::memset(&eventQueue, 0, sizeof(eventQueue));
    eventCallbackCount = 0;
    head = 0;
    tail = 0;
    
    return RPVC_OK;
}

RPVC_Status_t EventManager::registerHandler(RPVC_EventCallback_t callback)
{
    if (eventCallbackCount < MAX_EVENT_CALLBACKS) {
        eventCallbacks[eventCallbackCount++] = callback;
        return RPVC_OK;
    }
    else {
        return RPVC_ERR_OUT_OF_RANGE;
    }
}

RPVC_Status_t repvicore::EventManager::unregisterHandler(RPVC_EventCallback_t callback)
{
    if (eventCallbackCount > 0) {
        for (size_t i = 0; i < eventCallbackCount; i++) {
            if (eventCallbacks[i] == callback) {
                // Shift remaining callbacks down
                size_t newCount = eventCallbackCount - 1;
                for (size_t j = i; j < newCount; j++) {
                    eventCallbacks[j] = eventCallbacks[j + 1];
                }
                eventCallbacks[newCount] = nullptr;
                eventCallbackCount = newCount;
                return RPVC_OK;
            }
        }
    }
    else {
        return RPVC_ERR_NOT_FOUND;
    }
}

RPVC_Status_t EventManager::recordEvent(const RPVC_EventPacket_t *eventPacket)
{
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
            if (eventCallbacks[i]) {
                eventCallbacks[i](&eventPacket);
            }
        }
        head = (head + 1) % MAX_EVENT_QUEUE_SIZE;
    }
}