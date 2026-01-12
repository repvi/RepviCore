#ifndef RPVC_SOFTWAREBUS_H
#define RPVC_SOFTWAREBUS_H

#include "compile_time.h"
#include "core/core_types.h"

typedef struct RPVC_SoftwareBusConfig_s {
    uint32_t maxMessageQueue;
    uint32_t maxSubscribers;
     void (*ErrorCallback)(int32_t errorCode);
} RPVC_SoftwareBusConfig_t;

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_SOFTWAREBUS_INIT(const RPVC_SoftwareBusConfig_t* config);

RPVC_Status_t RPVC_SOFTWAREBUS_DEINIT(void);

RPVC_Status_t RPVC_SOFTWAREBUS_Subscribe(uint32_t messageId, uint32_t subsriberId);

RPVC_Status_t RPVC_SOFTWAREBUS_Unsubscribe(uint32_t messageId, uint32_t subsriberId);

RPVC_Status_t RPVC_SOFTWAREBUS_Publish(uint32_t messageId, const void* messageData, uint32_t messageSize);

RPVC_Status_t RPVC_SOFTWAREBUS_Flush(uint32_t subscriberId);


RPVC_EXTERN_C_END

#endif // RPVC_SOFTWAREBUS_H