#ifndef RPVC_OS_H
#define RPVC_OS_H

#include <stdint.h>
#include "core/core_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------
void RPVC_OS_Init(void);

// -----------------------------------------------------------------------------
// Task API
// -----------------------------------------------------------------------------
typedef struct RPVC_OS_Task RPVC_OS_Task;

int  RPVC_OS_TaskCreate(
    RPVC_OS_Task* task,
    void (*entry)(void*),
    void* arg,
    const char* name,
    uint32_t stackSize,
    uint32_t priority
);

void RPVC_OS_TaskYield(void);
RPVC_Status_t RPVC_OS_TaskSleep(uint32_t ms);

// -----------------------------------------------------------------------------
// Mutex API
// -----------------------------------------------------------------------------
typedef struct RPVC_OS_Mutex RPVC_OS_Mutex;

int RPVC_OS_MutexCreate(RPVC_OS_Mutex* m);
int RPVC_OS_MutexLock(RPVC_OS_Mutex* m);
int RPVC_OS_MutexUnlock(RPVC_OS_Mutex* m);

// -----------------------------------------------------------------------------
// Semaphore API
// -----------------------------------------------------------------------------
typedef struct RPVC_OS_Sem RPVC_OS_Sem;

int RPVC_OS_SemCreate(RPVC_OS_Sem* s, uint32_t initialCount);
int RPVC_OS_SemWait(RPVC_OS_Sem* s, uint32_t timeoutMs);
int RPVC_OS_SemSignal(RPVC_OS_Sem* s);

// -----------------------------------------------------------------------------
// Queue API
// -----------------------------------------------------------------------------
typedef struct RPVC_OS_Queue RPVC_OS_Queue;

int RPVC_OS_QueueCreate(
    RPVC_OS_Queue* q,
    uint32_t itemSize,
    uint32_t length
);

int RPVC_OS_QueueSend(
    RPVC_OS_Queue* q,
    const void* item,
    uint32_t timeoutMs
);

int RPVC_OS_QueueReceive(
    RPVC_OS_Queue* q,
    void* item,
    uint32_t timeoutMs
);

// -----------------------------------------------------------------------------
// Time API
// -----------------------------------------------------------------------------
RPVC_Status_t RPVC_OS_GetTick(uint32_t *outTick);
RPVC_Status_t RPVC_OS_GetTimeMicroseconds(uint64_t *outUs);

// -----------------------------------------------------------------------------
// Optional OS Hooks
// -----------------------------------------------------------------------------
void RPVC_OS_OnIdle(void);
void RPVC_OS_OnTick(void);

#ifdef __cplusplus
}
#endif

#endif // RPVC_OS_H
