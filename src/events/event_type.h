#ifndef REPVICORE_EVENT_TYPE_H
#define REPVICORE_EVENT_TYPE_H

#include <stdint.h>

#define RPVC_EVENT_TYPE_ID uint16_t

enum : RPVC_EVENT_TYPE_ID {
    RPVC_EVENT_NONE = 0,

    // Core system events
    RPVC_EVENT_OK,
    RPVC_EVENT_ERROR,
    RPVC_EVENT_WARNING,
    RPVC_EVENT_TIMEOUT,
    RPVC_EVENT_DATA_READY,
    RPVC_EVENT_DATA_INVALID,
    RPVC_EVENT_STATE_CHANGED,
    RPVC_EVENT_LIMIT_EXCEEDED,
    RPVC_EVENT_BOOT,
    RPVC_EVENT_SHUTDOWN,

    // Reserve a block for future RepviCore expansion
    RPVC_EVENT_RESERVED_START = 100,
    RPVC_EVENT_RESERVED_END   = 199,

    // User-defined events start here
    RPVC_EVENT_USER_BASE = 200
};

typedef RPVC_EVENT_TYPE_ID RPVC_EventId_t;

typedef void (*RPVC_EventCallback_t)(const RPVC_EventPacket_t *eventPacket);

typedef enum RPVC_EventSeverity_e : uint8_t {
    RPVC_EVENT_INFO = 0,
    RPVC_EVENT_WARNING = 1,
    RPVC_EVENT_ERROR = 2,
    RPVC_EVENT_CRITICAL = 3
} RPVC_EventSeverity_t;

typedef struct RPVC_EventPacket_t {
    uint32_t timestamp; // System tick or RTC time
    RPVC_EventSeverity_t severity;
    RPVC_EventId_t eventId;
    uint8_t payload[32];
} RPVC_EventPacket_t;

#endif // REPVICORE_EVENT_TYPE_H