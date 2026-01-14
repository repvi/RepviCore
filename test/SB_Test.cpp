#include "repvicore.h"
#include "RPVC_MEMORYPOOL.h"
#include "SoftwareBus.h"
#include <cassert>
#include <cstring>
#include <vector>

#include <iostream>
using namespace std;

string getErrorName(RPVC_Status_t status) 
{
    switch (status) {
        case RPVC_OK: return "RPVC_OK";
        case RPVC_ERR_INIT: return "RPVC_ERR_INIT";
        case RPVC_ERR_CONFIG: return "RPVC_ERR_CONFIG";
        case RPVC_ERR_NOT_READY: return "RPVC_ERR_NOT_READY";
        case RPVC_ERR_INVALID_ARG: return "RPVC_ERR_INVALID_ARG";
        case RPVC_ERR_OUT_OF_RANGE: return "RPVC_ERR_OUT_OF_RANGE";
        case RPVC_ERR_STATE: return "RPVC_ERR_STATE";
        case RPVC_ERR_TIMEOUT: return "RPVC_ERR_TIMEOUT";
        case RPVC_ERR_NO_MEMORY: return "RPVC_ERR_NO_MEMORY";
        case RPVC_ERR_NO_RESOURCE: return "RPVC_ERR_NO_RESOURCE";
        case RPVC_ERR_INTERNAL: return "RPVC_ERR_INTERNAL";
        case RPVC_ERR_INTEGRITY: return "RPVC_ERR_INTEGRITY";
        case RPVC_ERR_NOT_FOUND: return "RPVC_ERR_NOT_FOUND";
        default: return "UNKNOWN_ERROR";
    }
}

#define failOnError(status) \
    do { \
        if ((status) != RPVC_OK) { \
            cout << "Error: " << getErrorName(status) << " at " << __FILE__ << ":" << __LINE__ << endl; \
            abort(); \
        } \
    } while (0)

int main() 
{
    const RPVC_SoftwareBusConfig_t sbConfig = {};
    failOnError(RPVC_MEMORYPOOL_Init());
    failOnError(RPVC_SB_Init(&sbConfig));

    // Use system limits from header
    const int NUM_SUBSCRIBERS = RPVC_SB_MAX_PIPES;
    const int MAX_QUEUE_DEPTH = RPVC_SB_MAX_QUEUE_DEPTH;
    const int MAX_MSG_IDS = RPVC_SB_MAX_MESSAGE_ID;

    // For each message ID: subscribe all pipes, publish until queues fill,
    // verify overflow, drain, then release all allocated handles.
    for (int msgId = 0; msgId < MAX_MSG_IDS; ++msgId) {
        // subscribe all pipes to this message id
        for (int s = 0; s < NUM_SUBSCRIBERS; ++s) {
            failOnError(RPVC_SB_Subscribe((RPVC_SbSubscriberId_t)s, (RPVC_SbMsgId_t)msgId));
        }

        // store created handles so we can release them after draining
        vector<RPVC_SbMsgHandle_t> handles;
        handles.reserve(NUM_SUBSCRIBERS * MAX_QUEUE_DEPTH);

        // publish MAX_QUEUE_DEPTH messages (each will be copied to every subscriber)
        for (int i = 0; i < MAX_QUEUE_DEPTH; ++i) {
            char payload[RPVC_SB_MAX_PAYLOAD_SIZE];
            int len = snprintf(payload, sizeof(payload), "id%d-msg-%d", msgId, i);
            RPVC_SbMsgHandle_t mh = nullptr;
            failOnError(RPVC_SB_CreateMessage((RPVC_SbMsgId_t)msgId, (const uint8_t*)payload, (size_t)len+1, &mh));
            RPVC_Status_t st = RPVC_SB_Publish(mh);
            if (st != RPVC_OK) {
                cout << "Publish failed for msgId=" << msgId << " i=" << i << " -> " << getErrorName(st) << endl;
                // release handle if publish failed
                failOnError(RPVC_SB_ReleaseMessage(mh));
                break;
            }
            handles.push_back(mh);
        }

        // Attempt one more publish; expect out-of-range (queues full)
        {
            RPVC_SbMsgHandle_t mh = nullptr;
            failOnError(RPVC_SB_CreateMessage((RPVC_SbMsgId_t)msgId, (const uint8_t*)"overflow", 9, &mh));
            RPVC_Status_t st = RPVC_SB_Publish(mh);
            if (st == RPVC_OK) {
                cout << "Unexpected: publish succeeded when queues should be full for msgId=" << msgId << endl;
                handles.push_back(mh);
            } else {
                cout << "Expected publish failure when full for msgId=" << msgId << ": " << getErrorName(st) << endl;
                failOnError(RPVC_SB_ReleaseMessage(mh));
            }
        }

        // drain all queues by receiving from each subscriber until empty
        for (int s = 0; s < NUM_SUBSCRIBERS; ++s) {
            while (true) {
                uint8_t buf[RPVC_SB_MAX_PAYLOAD_SIZE] = {0};
                RPVC_Status_t st = RPVC_SB_Receive((RPVC_SbSubscriberId_t)s, buf, sizeof(buf));
                if (st != RPVC_OK) break;
                // Received payload in buf; nothing to free here — queue pop decremented refs.
            }
            failOnError(RPVC_SB_Flush((RPVC_SbSubscriberId_t)s));
        }

        // Now release all created handles; ReleaseMessage will free when refCount==0.
        for (auto h : handles) {
            RPVC_Status_t st = RPVC_SB_ReleaseMessage(h);
            if (st != RPVC_OK) {
                cout << "Release failed for msgId=" << msgId << " -> " << getErrorName(st) << endl;
            }
        }

        // Unsubscribe all pipes from this message id
        for (int s = 0; s < NUM_SUBSCRIBERS; ++s) {
            RPVC_Status_t st = RPVC_SB_Unsubscribe((RPVC_SbSubscriberId_t)s, (RPVC_SbMsgId_t)msgId);
            if (st != RPVC_OK && st != RPVC_ERR_NOT_FOUND) {
                cout << "Unsubscribe failed for msgId=" << msgId << " pipe=" << s << " -> " << getErrorName(st) << endl;
            }
        }
    }
    failOnError(RPVC_SB_Deinit());
    cout << "Stress test completed." << endl;
    return 0;
}