#ifndef REPVICORE_EVENT_TYPE_H
#define REPVICORE_EVENT_TYPE_H

#include <stdint.h>

typedef enum RPVC_EventBaseId_e {
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
} RPVC_EventBaseId_t;

#define RPVC_EVENT_TYPE_ID uint32_t

typedef RPVC_EVENT_TYPE_ID RPVC_EventId_t;

typedef enum RPVC_EventSeverity_e {
    RPVC_SEVERITY_INFO = 0,
    RPVC_SEVERITY_WARNING = 1,
    RPVC_SEVERITY_ERROR = 2,
    RPVC_SEVERITY_CRITICAL = 3
} RPVC_EventSeverity_t;

typedef struct RPVC_EventPacket_t {
    uint32_t timestamp; // System tick or RTC time
    RPVC_EventSeverity_t severity;
    RPVC_EventId_t eventId;
    uint8_t payload[32];
} RPVC_EventPacket_t;

typedef void (*RPVC_EventCallback_t)(const RPVC_EventPacket_t *eventPacket);

#endif // REPVICORE_EVENT_TYPE_H