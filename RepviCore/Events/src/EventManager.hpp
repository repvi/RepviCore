#ifndef REPVICORE_EVENT_MANAGER_HPP
#define REPVICORE_EVENT_MANAGER_HPP

#include "core/core_types.h"
#include "core/core_errors.h"
#include "compile_time.h"
#include "event_type.h"

namespace repvicore {
    class EventManager {
        public:
        static RPVC_Status_t Init(void);
        static RPVC_Status_t registerHandler(RPVC_EventCallback_t callback);
        static RPVC_Status_t recordEvent(const RPVC_EventPacket_t *eventPacket);
        static void dispatch(void);

        private:
        static constexpr int MAX_EVENT_CALLBACKS = 10;
        static constexpr int MAX_EVENT_QUEUE_SIZE = 20;
        static RPVC_EventCallback_t eventCallback[MAX_EVENT_CALLBACKS];
        static uint8_t eventCallbackCount;

        static RPVC_EventPacket_t eventQueue[MAX_EVENT_QUEUE_SIZE];
        static uint8_t head, tail;
    };
};

#endif // REPVICORE_EVENT_MANAGER_HPP
