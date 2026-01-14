/* ARM baremetal OS abstraction layer */
#include <stddef.h>
#include "RPVC_OS.h"
#include "RPVC_TIME.h"
#include "RPVC_SYSTEM.h"
#include "RPVC_MEMORYPOOL.h"
#include "RPVC_INTERRUPTS.h"
#include <stdint.h>
#include <string.h>

#define RPVC_OS_DEFAULT_DEFINE

/* Baremetal implementation - no RTOS support */
static bool g_RPVC_OS_Initialized = false;

typedef struct RPVC_OS_Task {
    uint32_t id;
} RPVC_OS_Task;

typedef struct RPVC_OS_Mutex {
    uint32_t *owner;
    bool locked;
} RPVC_OS_Mutex;

typedef struct RPVC_OS_Sem {
    uint32_t count;
} RPVC_OS_Sem;

typedef struct RPVC_OS_Queue {
    uint8_t *buffer;
    uint32_t itemSize;
    uint32_t length;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} RPVC_OS_Queue;

#define RPVC_OS_MAX_TASKS 8
//static RPVC_OS_Task tasks[RPVC_OS_MAX_TASKS] = {0};

#define RPVC_OS_MAX_MUXES 8
static RPVC_OS_Mutex mutex_pool[RPVC_OS_MAX_MUXES];
static bool mutex_used[RPVC_OS_MAX_MUXES];

#define RPVC_OS_MAX_SEMS 8
static RPVC_OS_Sem sem_pool[RPVC_OS_MAX_SEMS];
static bool sem_used[RPVC_OS_MAX_SEMS];

#define RPVC_OS_MAX_QUEUES 8
#define RPVC_OS_MAX_QUEUE_BYTES 1028   // max memory per queue

static uint8_t queue_memory_pool[RPVC_OS_MAX_QUEUES][RPVC_OS_MAX_QUEUE_BYTES];
static bool queue_used[RPVC_OS_MAX_QUEUES];

static size_t getFreeElement(bool *usedArray, uint32_t maxElements)
{
    for (uint32_t i = 0; i < maxElements; ++i) {
        if (!usedArray[i]) {
            usedArray[i] = true;
            return i;
        }
    }
    return (size_t)-1;  /* No free element */
}

RPVC_Status_t RPVC_OS_Init(void)
{
    if (g_RPVC_OS_Initialized) {
        return RPVC_ERR_INIT;
    }
    /* Nothing to initialize for baremetal */
    memset(mutex_used, 0, sizeof(mutex_used));
    memset(sem_used, 0, sizeof(sem_used));
    memset(queue_used, 0, sizeof(queue_used));
    memset(queue_memory_pool, 0, sizeof(queue_memory_pool));

    g_RPVC_OS_Initialized = true;
    return RPVC_OK;
}

static inline uint32_t get_psp(void)
{
    uint32_t result;
    __asm__ volatile ("mrs %0, psp" : "=r" (result) );
    return result;
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
    #ifdef RPVC_OS_DEFAULT_DEFINE
    (void)entry;
    (void)arg;
    (void)name;
    (void)stackSize;
    (void)priority;
    if (task == NULL || entry == NULL) {
        return -1;  /* Invalid argument */
    }

    task->id = get_psp();  /* Use PSP as task identifier */
    #else

    //define your own code here if needed

    #endif
    return -1;  /* Not supported */
}

void RPVC_OS_TaskYield(void)
{
    /* No scheduler - do nothing */
    #ifdef RPVC_OS_DEFAULT_DEFINE
    
    #else 
    // define your own code here if needed
    #endif
}

RPVC_Status_t RPVC_OS_TaskSleep(uint32_t ms)
{
    /* Use platform delay instead */
    #ifdef RPVC_OS_DEFAULT_DEFINE

    return RPVC_Time_DelayMs(ms);
    #else
    // define your own code here if needed

    return RPVC_OK;
    #endif
}

/* Mutex API - Not supported in baremetal */
int RPVC_OS_MutexCreate(RPVC_OS_Mutex* m)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    m->locked = false;
    m->owner = NULL;
    return -1;  /* Not supported */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_MutexLock(RPVC_OS_Mutex* m)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    if (m && m->locked) {
        return 0;  /* Already locked by this task */
    }

    uint32_t prevIntrState = RPVC_INTERRUPTS_SaveState();
    RPVC_INTERRUPTS_Disable();

    m->locked = true;
    m->owner = (uint32_t *)get_psp();

    RPVC_INTERRUPTS_RestoreState(prevIntrState);

    return -1;  /* Not supported */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_MutexUnlock(RPVC_OS_Mutex* m)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    // Only the owner can unlock
    if (m && m->locked && m->owner == (uint32_t *)get_psp()) {
        uint32_t prevIntrState = RPVC_INTERRUPTS_SaveState();
        RPVC_INTERRUPTS_Disable();
        
        m->locked = false;
        m->owner = NULL;
        
        RPVC_INTERRUPTS_RestoreState(prevIntrState);
        return 0;  /* Successfully unlocked */
    }
    return -1;  /* Not supported */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

/* Semaphore API - Not supported in baremetal */
int RPVC_OS_SemCreate(RPVC_OS_Sem* s, uint32_t initialCount)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    if (s == NULL) {
        return -1;  /* Invalid argument */
    }

    s->count = initialCount;
    return -1;  /* Not supported */
    #else

    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_SemWait(RPVC_OS_Sem* s, uint32_t timeoutMs)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    (void)timeoutMs; // Not supported

    if (s == NULL) {
        return -1;  /* Invalid argument */
    }

    uint32_t prevIntrState = RPVC_INTERRUPTS_SaveState();
    RPVC_INTERRUPTS_Disable();
    if (s->count > 0) {
        s->count--;
        RPVC_INTERRUPTS_RestoreState(prevIntrState);

        return 0;  /* Successfully acquired */
    }
    else {
        
        RPVC_INTERRUPTS_RestoreState(prevIntrState);
        return -1;  /* Not supported */
    }
    #else


    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_SemSignal(RPVC_OS_Sem* s)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    if (s == NULL) {
        return -1;  /* Invalid argument */
    }
    uint32_t prevIntrState = RPVC_INTERRUPTS_SaveState();
    RPVC_INTERRUPTS_Disable();

    s->count++;
    
    RPVC_INTERRUPTS_RestoreState(prevIntrState);
    return 0;  /* Successfully signaled */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

/* Queue API - Not supported in baremetal */
int RPVC_OS_QueueCreate(
    RPVC_OS_Queue* q,
    uint32_t itemSize,
    uint32_t length
)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    (void)q;
    (void)itemSize;
    (void)length;
    return -1;  /* Not supported */
    #else


    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_QueueSend(
    RPVC_OS_Queue* q,
    const void* item,
    uint32_t timeoutMs
)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    (void)q;
    (void)item;
    (void)timeoutMs;
    return -1;  /* Not supported */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

int RPVC_OS_QueueReceive(
    RPVC_OS_Queue* q,
    void* item,
    uint32_t timeoutMs
)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    (void)q;
    (void)item;
    (void)timeoutMs;
    return -1;  /* Not supported */
    #else
    // define your own code here if needed
    return -1;
    #endif
}

/* Time API - Wrapper around platform time functions */
RPVC_Status_t RPVC_OS_GetTick(uint32_t *outTick)
{
    if (outTick == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    #ifdef RPVC_OS_DEFAULT_DEFINE
    RPVC_Status_t status = RPVC_Time_GetTick(outTick);
    return status;
    #else
    // define your own code here if needed
    return RPVC_OK;
    #endif
}

RPVC_Status_t RPVC_OS_GetTimeMicroseconds(uint64_t *outUs)
{
    if (outUs == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
    #ifdef RPVC_OS_DEFAULT_DEFINE
    RPVC_Status_t status = RPVC_Time_GetMicroseconds(outUs);
    return status;
    #else

    // define your own code here if needed
    return RPVC_OK;
    #endif
}

/* Optional OS Hooks */
void RPVC_OS_OnIdle(void)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    RPVC_System_Idle();
    #else
    // define your own code here if needed
    #endif
}

void RPVC_OS_OnTick(void)
{
    #ifdef RPVC_OS_DEFAULT_DEFINE
    /* Called on timer tick - can be used for custom scheduling */
    //RPVC_System_TimerTick();
    #else
    // define your own code here if needed
    #endif
}
