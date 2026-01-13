#ifndef RPVC_SOFTWAREBUS_H
#define RPVC_SOFTWAREBUS_H

#include "compile_time.h"
#include "core_types.h"

typedef uint16_t RPVC_SbSubscriberId_t; // pipe index
typedef uint16_t RPVC_SbMsgId_t; // routing key

typedef struct RPVC_SoftwareBusConfig_s {
    void (*ErrorCallback)(int32_t errorCode); // no implementation yet
} RPVC_SoftwareBusConfig_t;

typedef struct RPVC_SbMsg_t *RPVC_SbMsgHandle_t;

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_SB_Init(const RPVC_SoftwareBusConfig_t* config);

RPVC_Status_t RPVC_SB_Deinit(void);

RPVC_Status_t RPVC_SB_Subscribe(RPVC_SbSubscriberId_t subscriberId, RPVC_SbMsgId_t messageId);
RPVC_Status_t RPVC_SB_Unsubscribe(RPVC_SbSubscriberId_t subscriberId, RPVC_SbMsgId_t messageId);

RPVC_Status_t RPVC_SB_Publish(RPVC_SbMsgHandle_t messageHandle);
RPVC_Status_t RPVC_SB_Receive(RPVC_SbSubscriberId_t subscriberId, uint8_t *outBuffer, size_t bufferSize);
RPVC_Status_t RPVC_SB_Flush(RPVC_SbSubscriberId_t subscriberId);

RPVC_Status_t RPVC_SB_CreateMessage(RPVC_SbMsgId_t messageId, const uint8_t *messageData, size_t messageSize, RPVC_SbMsgHandle_t *outMessageHandle);
RPVC_Status_t RPVC_SB_ReleaseMessage(RPVC_SbMsgHandle_t messageHandle);

RPVC_EXTERN_C_END

#endif // RPVC_SOFTWAREBUS_H