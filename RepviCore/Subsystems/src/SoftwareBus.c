#include "SoftwareBus.h"
#include "RPVC_MEMORYPOOL.h"
#include <string.h>

#define MAX_PAYLOAD_SIZE 100
#define MAX_QUEUE_DEPTH 5
#define MAX_PIPES 10
#define MAX_SUBSCRIBERS 10
#define MAX_MESSAGE_ID 20

#define NOT_VALID_SUBSCRIBER_ID (RPVC_SbSubscriberId_t)(-1)

typedef struct RPVC_SbMsg_t {
    uint8_t payload[MAX_PAYLOAD_SIZE];
    RPVC_SbMsgId_t messageId;
    uint16_t len;
    uint8_t refCount;
} RPVC_SbMsg_t;

typedef struct {
    RPVC_SbMsgHandle_t buffer[MAX_QUEUE_DEPTH];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} RPVC_SbQueue_t;

typedef struct {
    RPVC_SbQueue_t queue;
    bool isInitialized;
} RPVC_SbPip_t;

typedef struct {
    RPVC_SbSubscriberId_t subscriberIds[MAX_SUBSCRIBERS]; // store the index of pipes here
    uint8_t count;
}RPVC_SbRouteEntry_t;

typedef struct {
    RPVC_SbPip_t pipes[MAX_PIPES];
    RPVC_SbRouteEntry_t routes[MAX_MESSAGE_ID];
    bool isInitialized;
} RPVC_SbState_t;

static RPVC_SbState_t g_sbState = {};

static void addMessageToPipe(RPVC_SbPip_t *pipe, RPVC_SbMsgHandle_t messageHandle) 
{
    size_t tailIndex = pipe->queue.tail;
    pipe->queue.tail = (tailIndex + 1) % MAX_QUEUE_DEPTH;
    pipe->queue.count++;
    messageHandle->refCount++;
    pipe->queue.buffer[tailIndex] = messageHandle;
}

static void grabAndPopMessageFromPipe(RPVC_SbPip_t *pipe, RPVC_SbMsgHandle_t *outMessageHandle) 
{
    size_t headIndex = pipe->queue.head;
    pipe->queue.head = (headIndex + 1) % MAX_QUEUE_DEPTH;
    pipe->queue.count--;
    (*outMessageHandle)->refCount--;
    *outMessageHandle = pipe->queue.buffer[headIndex];
}

static void initPipes() 
{
    for (size_t i = 0; i < MAX_PIPES; i++) {
        g_sbState.pipes[i].isInitialized = false;
        g_sbState.pipes[i].queue.head = 0;
        g_sbState.pipes[i].queue.tail = 0;
        g_sbState.pipes[i].queue.count = 0;
    }
}

static void initRoutes() 
{
    for (size_t i = 0; i < MAX_MESSAGE_ID; i++) {
        g_sbState.routes[i].count = 0;
        for (size_t j = 0; j < MAX_SUBSCRIBERS; j++) {
            g_sbState.routes[i].subscriberIds[j] = NOT_VALID_SUBSCRIBER_ID;
        }
    }
}

RPVC_Status_t RPVC_SB_Init(const RPVC_SoftwareBusConfig_t* config) 
{
    if (!config) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (!RPVC_MEMORYPOOL_IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }

    if (g_sbState.isInitialized) {
        return RPVC_ERR_STATE;
    }

    memset(&g_sbState, 0, sizeof(g_sbState));

    initRoutes();
    initPipes();
    g_sbState.isInitialized = true;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SB_Deinit(void) 
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    g_sbState.isInitialized = false;
    return RPVC_OK;
}

static bool IsValidSubscriber(RPVC_SbSubscriberId_t subscriberId)
{
    return subscriberId < MAX_PIPES;
}

static bool IsValidMessageId(RPVC_SbMsgId_t messageId)
{
    return messageId < MAX_MESSAGE_ID;
}

RPVC_Status_t RPVC_SB_Subscribe(RPVC_SbSubscriberId_t subscriberId, RPVC_SbMsgId_t messageId)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!IsValidSubscriber(subscriberId) || !IsValidMessageId(messageId)) {
        return RPVC_ERR_INVALID_ARG;
    }

    RPVC_SbRouteEntry_t *entry = &g_sbState.routes[messageId];
    if (entry->count < MAX_SUBSCRIBERS) {
        size_t emptyIndex = (size_t)(-1);
        for (size_t i = 0; i < MAX_SUBSCRIBERS; i++) {
            if (entry->subscriberIds[i] == subscriberId) {
                return RPVC_OK; // Already subscribed
            }

            if (entry->subscriberIds[i] == NOT_VALID_SUBSCRIBER_ID) {
                emptyIndex = i;
            }
        }

        if (emptyIndex == (size_t)(-1)) {
            return RPVC_ERR_OUT_OF_RANGE; // Should not happen due to earlier check
        }

        entry->subscriberIds[emptyIndex] = subscriberId;
        entry->count++;

        g_sbState.pipes[subscriberId].isInitialized = true;
        return RPVC_OK;
    }
    else {
        return RPVC_ERR_OUT_OF_RANGE;
    }
}

RPVC_Status_t RPVC_SB_Unsubscribe(RPVC_SbSubscriberId_t subscriberId, RPVC_SbMsgId_t messageId)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!IsValidSubscriber(subscriberId) || !IsValidMessageId(messageId)) {
        return RPVC_ERR_INVALID_ARG;
    }

    RPVC_SbRouteEntry_t *entry = &g_sbState.routes[messageId];
    if (entry->count > 0) {
        for (size_t i = 0; i < MAX_SUBSCRIBERS; i++) {
            if (entry->subscriberIds[i] == subscriberId) {
                RPVC_SbPip_t *pipe = &g_sbState.pipes[subscriberId];
                entry->subscriberIds[i] = NOT_VALID_SUBSCRIBER_ID;
                entry->count--;
                return RPVC_OK;
            }
        }
    }
    return RPVC_ERR_NOT_FOUND;
}

RPVC_Status_t RPVC_SB_Publish(RPVC_SbMsgHandle_t messageHandle)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    RPVC_SbMsgId_t msgId = messageHandle->messageId;
    if (!messageHandle || !IsValidMessageId(msgId)) {
        return RPVC_ERR_INVALID_ARG;
    }

    bool publishedToAtLeastOne = false;

    RPVC_SbRouteEntry_t *entry = &g_sbState.routes[msgId];
    for (size_t j = 0; j < MAX_SUBSCRIBERS; j++) {
        RPVC_SbSubscriberId_t subscriberId = entry->subscriberIds[j];

        if (subscriberId != NOT_VALID_SUBSCRIBER_ID) {
            RPVC_SbPip_t *pipe = &g_sbState.pipes[subscriberId];

            if (pipe->isInitialized && pipe->queue.count < MAX_QUEUE_DEPTH) {
                addMessageToPipe(pipe, messageHandle);
                publishedToAtLeastOne = true;
            }
        }
    }

    if (publishedToAtLeastOne) {
        return RPVC_OK;
    }
    else {
        return RPVC_ERR_OUT_OF_RANGE;
    }
}

RPVC_Status_t RPVC_SB_Receive(RPVC_SbSubscriberId_t subscriberId, uint8_t *outBuffer, size_t bufferSize)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!IsValidSubscriber(subscriberId) || outBuffer == NULL || bufferSize == 0) {
        return RPVC_ERR_INVALID_ARG;
    }

    RPVC_SbPip_t *pipe = &g_sbState.pipes[subscriberId];
    if (!pipe->isInitialized || pipe->queue.count == 0) {
        return RPVC_ERR_OUT_OF_RANGE;
    }
    RPVC_SbMsgHandle_t messageHandle;
    grabAndPopMessageFromPipe(pipe, &messageHandle);
    size_t copySize = messageHandle->len < bufferSize ? messageHandle->len : bufferSize;
    memcpy(outBuffer, messageHandle->payload, copySize);
    return RPVC_OK;
}

RPVC_Status_t RPVC_SB_Flush(RPVC_SbSubscriberId_t subscriberId)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!IsValidSubscriber(subscriberId)) {
        return RPVC_ERR_INVALID_ARG;
    }

    RPVC_SbPip_t *pipe = &g_sbState.pipes[subscriberId];
    pipe->queue.head = 0;
    pipe->queue.tail = 0;
    pipe->queue.count = 0;
    pipe->isInitialized = false;
    
    return RPVC_OK;
}

RPVC_Status_t RPVC_SB_CreateMessage(RPVC_SbMsgId_t messageId, const uint8_t *messageData, size_t messageSize, RPVC_SbMsgHandle_t *outMessageHandle)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!IsValidMessageId(messageId) || !outMessageHandle || !messageData) {
        return RPVC_ERR_INVALID_ARG;
    }

    RPVC_SbMsgHandle_t newmessage = NULL;
    RPVC_Status_t status = RPVC_MEMORYPOOL_Allocate(sizeof(RPVC_SbMsg_t), (void**)&newmessage);
    if (status != RPVC_OK) {
        return status;
    }

    size_t trueSize = messageSize > MAX_PAYLOAD_SIZE ? MAX_PAYLOAD_SIZE : messageSize;
    newmessage->messageId = messageId;
    memcpy(newmessage->payload, messageData, trueSize);
    newmessage->len = trueSize;
    newmessage->refCount = 0;
    *outMessageHandle = newmessage;
    return RPVC_OK;
}

RPVC_Status_t RPVC_SB_ReleaseMessage(RPVC_SbMsgHandle_t messageHandle)
{
    if (!g_sbState.isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (!messageHandle) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (messageHandle->refCount > 0) { // some pipes are still using it
        return RPVC_ERR_STATE;
    }
    else {
        return RPVC_MEMORYPOOL_Free((void*)messageHandle);
    }
}
