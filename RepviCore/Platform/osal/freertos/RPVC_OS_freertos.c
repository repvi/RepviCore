#include "RPVC_OS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* Concrete internal types backed by FreeRTOS handles */
struct RPVC_OS_Task { TaskHandle_t handle; };
struct RPVC_OS_Mutex { SemaphoreHandle_t handle; };
struct RPVC_OS_Sem { SemaphoreHandle_t handle; };
struct RPVC_OS_Queue { QueueHandle_t handle; };

int RPVC_OS_TaskCreate(
    RPVC_OS_Task* task,
    void (*entry)(void*),
    void* arg,
    const char* name,
    uint32_t stackSize,
    uint32_t priority
)
{
    if (!task || !entry) return -1;
    BaseType_t r = xTaskCreate((TaskFunction_t)entry, name ? name : "rpvc_task",
                               (configSTACK_DEPTH_TYPE)stackSize, arg,
                               (UBaseType_t)priority, &task->handle);
    return (r == pdPASS) ? 0 : -1;
}

void RPVC_OS_TaskYield(void)
{
    taskYIELD();
}

RPVC_Status_t RPVC_OS_TaskSleep(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
    return RPVC_OK;
}

// -----------------------------------------------------------------------------
// Mutex API
// -----------------------------------------------------------------------------

int RPVC_OS_MutexCreate(RPVC_OS_Mutex* m)
{
    if (!m) return -1;
    m->handle = xSemaphoreCreateMutex();
    return m->handle ? 0 : -1;
}
int RPVC_OS_MutexLock(RPVC_OS_Mutex* m)
{
    if (!m || !m->handle) return -1;
    return (xSemaphoreTake(m->handle, portMAX_DELAY) == pdTRUE) ? 0 : -1;
}
int RPVC_OS_MutexUnlock(RPVC_OS_Mutex* m)
{
    if (!m || !m->handle) return -1;
    return (xSemaphoreGive(m->handle) == pdTRUE) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// Semaphore API
// -----------------------------------------------------------------------------

int RPVC_OS_SemCreate(RPVC_OS_Sem* s, uint32_t initialCount)
{
    if (!s) return -1;
    if (initialCount > 1) {
        s->handle = xSemaphoreCreateCounting((UBaseType_t)initialCount, (UBaseType_t)initialCount);
    } else {
        s->handle = xSemaphoreCreateBinary();
        if (s->handle && initialCount == 1) {
            xSemaphoreGive(s->handle);
        }
    }
    return s->handle ? 0 : -1;
}
int RPVC_OS_SemWait(RPVC_OS_Sem* s, uint32_t timeoutMs)
{
    if (!s || !s->handle) return -1;
    TickType_t ticks = (timeoutMs == UINT32_MAX) ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    return (xSemaphoreTake(s->handle, ticks) == pdTRUE) ? 0 : -1;
}
int RPVC_OS_SemSignal(RPVC_OS_Sem* s)
{
    if (!s || !s->handle) return -1;
    return (xSemaphoreGive(s->handle) == pdTRUE) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// Queue API
// -----------------------------------------------------------------------------

int RPVC_OS_QueueCreate(
    RPVC_OS_Queue* q,
    uint32_t itemSize,
    uint32_t length
)
{
    if (!q || itemSize == 0 || length == 0) return -1;
    q->handle = xQueueCreate((UBaseType_t)length, (UBaseType_t)itemSize);
    return q->handle ? 0 : -1;
}

int RPVC_OS_QueueSend(
    RPVC_OS_Queue* q,
    const void* item,
    uint32_t timeoutMs
)
{
    if (!q || !q->handle || !item) return -1;
    TickType_t ticks = (timeoutMs == UINT32_MAX) ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    return (xQueueSend(q->handle, item, ticks) == pdTRUE) ? 0 : -1;
}

int RPVC_OS_QueueReceive(
    RPVC_OS_Queue* q,
    void* item,
    uint32_t timeoutMs
)
{
    if (!q || !q->handle || !item) return -1;
    TickType_t ticks = (timeoutMs == UINT32_MAX) ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    return (xQueueReceive(q->handle, item, ticks) == pdTRUE) ? 0 : -1;
}

// -----------------------------------------------------------------------------
// Time API
// -----------------------------------------------------------------------------
RPVC_Status_t RPVC_OS_GetTick(uint32_t *outTick)
{
    if (!outTick) return RPVC_ERR_INVALID_ARG;
    *outTick = (uint32_t)xTaskGetTickCount();
    return RPVC_OK;
}
RPVC_Status_t RPVC_OS_GetTimeMicroseconds(uint64_t *outUs)
{
    if (!outUs) return RPVC_ERR_INVALID_ARG;
    /* Best-effort: convert ticks to microseconds */
    TickType_t ticks = xTaskGetTickCount();
    uint64_t ms = (uint64_t)ticks * (uint64_t)portTICK_PERIOD_MS;
    *outUs = ms * 1000ULL;
    return RPVC_OK;
}

// -----------------------------------------------------------------------------
// Optional OS Hooks
// -----------------------------------------------------------------------------
void RPVC_OS_OnIdle(void)
{
    /* Optional hook: called when idle. */
}
void RPVC_OS_OnTick(void)
{
    /* Optional hook: called on tick. */
}