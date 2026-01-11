/* ARM baremetal OS abstraction layer */
#include <stddef.h>
#include "RPVC_OS.h"
#include "RPVC_Time.h"
#include "RPVC_System.h"
#include <stdint.h>

/* Baremetal implementation - no RTOS support */
static bool g_RPVC_OS_Initialized = false;

RPVC_Status_t RPVC_OS_Init(void)
{
    if (g_RPVC_OS_Initialized) {
        return RPVC_ERR_INIT;
    }
    /* Nothing to initialize for baremetal */
    g_RPVC_OS_Initialized = true;
    return RPVC_OK;
}

/* Task API - Not supported in baremetal */
int RPVC_OS_TaskCreate(
    RPVC_OS_Task* task,
    void (*entry)(void*),
    void* arg,
    const char* name,
    uint32_t stackSize,
    uint32_t priority
)
{
    (void)task;
    (void)entry;
    (void)arg;
    (void)name;
    (void)stackSize;
    (void)priority;
    return -1;  /* Not supported */
}

void RPVC_OS_TaskYield(void)
{
    /* No scheduler - do nothing */
}

RPVC_Status_t RPVC_OS_TaskSleep(uint32_t ms)
{
    /* Use platform delay instead */
    return RPVC_Time_DelayMs(ms);
}

/* Mutex API - Not supported in baremetal */
int RPVC_OS_MutexCreate(RPVC_OS_Mutex* m)
{
    (void)m;
    return -1;  /* Not supported */
}

int RPVC_OS_MutexLock(RPVC_OS_Mutex* m)
{
    (void)m;
    return -1;  /* Not supported */
}

int RPVC_OS_MutexUnlock(RPVC_OS_Mutex* m)
{
    (void)m;
    return -1;  /* Not supported */
}

/* Semaphore API - Not supported in baremetal */
int RPVC_OS_SemCreate(RPVC_OS_Sem* s, uint32_t initialCount)
{
    (void)s;
    (void)initialCount;
    return -1;  /* Not supported */
}

int RPVC_OS_SemWait(RPVC_OS_Sem* s, uint32_t timeoutMs)
{
    (void)s;
    (void)timeoutMs;
    return -1;  /* Not supported */
}

int RPVC_OS_SemSignal(RPVC_OS_Sem* s)
{
    (void)s;
    return -1;  /* Not supported */
}

/* Queue API - Not supported in baremetal */
int RPVC_OS_QueueCreate(
    RPVC_OS_Queue* q,
    uint32_t itemSize,
    uint32_t length
)
{
    (void)q;
    (void)itemSize;
    (void)length;
    return -1;  /* Not supported */
}

int RPVC_OS_QueueSend(
    RPVC_OS_Queue* q,
    const void* item,
    uint32_t timeoutMs
)
{
    (void)q;
    (void)item;
    (void)timeoutMs;
    return -1;  /* Not supported */
}

int RPVC_OS_QueueReceive(
    RPVC_OS_Queue* q,
    void* item,
    uint32_t timeoutMs
)
{
    (void)q;
    (void)item;
    (void)timeoutMs;
    return -1;  /* Not supported */
}

/* Time API - Wrapper around platform time functions */
RPVC_Status_t RPVC_OS_GetTick(uint32_t *outTick)
{
    if (outTick == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    RPVC_Status_t status = RPVC_Time_GetTick(outTick);
    return status;
}

RPVC_Status_t RPVC_OS_GetTimeMicroseconds(uint64_t *outUs)
{
    if (outUs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    RPVC_Status_t status = RPVC_Time_GetMicroseconds(outUs);
    return status;
}

/* Optional OS Hooks */
void RPVC_OS_OnIdle(void)
{
    RPVC_System_Idle();
}

void RPVC_OS_OnTick(void)
{
    /* Called on timer tick - can be used for custom scheduling */
}
