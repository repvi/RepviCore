#include "repvicore.h"
#include "RPVC_TIME.h"
#include "event_api.h"
#include "event_type.h"
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

static int g_handler1_count = 0;
static int g_handler2_count = 0;
static bool g_payload_ok = true;
static std::vector<uint32_t> g_received_ids;

extern "C" void testHandler1(const RPVC_EventPacket_t *p)
{
    ++g_handler1_count;
    if (p) {
        g_received_ids.push_back((uint32_t)p->eventId);
        if (p->payload[0] != (uint8_t)(p->eventId & 0xFF)) {
            g_payload_ok = false;
        }
    }
}

extern "C" void testHandler2(const RPVC_EventPacket_t *p)
{
    ++g_handler2_count;
}

int main()
{
    // Initialize time (required by event API)
    RPVC_TimeConfig_t timeConfig = { .systemHz = 1000000, .tickHz = 1000 };
    RPVC_Status_t status = RPVC_TIME_Init(&timeConfig);
    failOnError(status);

    // Ensure event subsystem initializes
    status = RPVC_EVENT_Init();
    failOnError(status);

    // Null handler registration should be rejected
    status = RPVC_EVENT_RegisterHandler(nullptr);
    assert(status == RPVC_ERR_INVALID_ARG);

    // Register two handlers
    status = RPVC_EVENT_RegisterHandler(&testHandler1);
    failOnError(status);
    status = RPVC_EVENT_RegisterHandler(&testHandler2);
    failOnError(status);

    // Stress enqueue/dispatch cycles to test queue wrap-around and limits
    size_t total_enqueued = 0;
    size_t total_processed = 0;

    for (int cycle = 0; cycle < 10; ++cycle) {
        // Attempt to enqueue as many as possible until queue reports full
        int enqueued_this_cycle = 0;
        for (int i = 0; i < 100; ++i) {
            RPVC_EventPacket_t pkt;
            std::memset(&pkt, 0, sizeof(pkt));
            pkt.timestamp = 0;
            pkt.severity = RPVC_SEVERITY_INFO;
            pkt.eventId = RPVC_EVENT_USER_BASE + (uint32_t)((cycle * 100) + i);
            pkt.payload[0] = (uint8_t)(pkt.eventId & 0xFF);

            status = RPVC_EVENT_Record(&pkt);
            if (status == RPVC_OK) {
                ++enqueued_this_cycle;
            }
            else if (status == RPVC_ERR_OUT_OF_RANGE) {
                // queue full - stop this batch
                break;
            }
            else {
                cout << "Unexpected error during record: " << getErrorName(status) << endl;
                abort();
            }
        }

        total_enqueued += enqueued_this_cycle;

        // Dispatch queued events
        RPVC_EVENT_Dispatch();

        // After dispatch, handler counts should have increased by enqueued_this_cycle
        total_processed += enqueued_this_cycle;
        assert((size_t)g_handler1_count == total_processed);
        assert((size_t)g_handler2_count == total_processed);
    }

    // Sanity checks
    assert(g_payload_ok);
    assert(!g_received_ids.empty());

    cout << "Stress test summary:\n";
    cout << "  Total enqueued: " << total_enqueued << "\n";
    cout << "  Handler1 called: " << g_handler1_count << "\n";
    cout << "  Handler2 called: " << g_handler2_count << "\n";
    cout << "  Payloads OK: " << (g_payload_ok ? "yes" : "NO") << "\n";

    cout << "Stress test completed." << endl;
    return 0;
}