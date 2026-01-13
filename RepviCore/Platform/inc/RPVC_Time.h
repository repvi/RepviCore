#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core_types.h"
#include "compile_time.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t systemHz; /* System tick frequency in Hz */
    uint32_t tickHz;   /* Tick frequency in Hz */
} RPVC_TimeConfig_t;

RPVC_EXTERN_C_BEGIN

/* Platform-specific initialization */
RPVC_Status_t RPVC_TIME_Init(const RPVC_TimeConfig_t *config);

/* Query initialization state */
bool RPVC_TIME_IsInitialized(void);

/* Tick management - call from platform TIMEr ISR */
void RPVC_TIME_OnTick(void);

/* Optional user hook called on each tick (weak function) */
void RPVC_TIME_UserTickHook(void);

/* Get current tick count */
RPVC_Status_t RPVC_TIME_GetTick(uint32_t *outTick);
RPVC_Status_t RPVC_TIME_GetTick64(uint64_t *outTick);

/* High-precision timing */
RPVC_Status_t RPVC_TIME_GetMicroseconds(uint64_t *outUs);
RPVC_Status_t RPVC_TIME_GetMilliseconds(uint64_t *outMs);

/* Time difference calculations */
uint32_t RPVC_TIME_TickDiff(uint32_t start, uint32_t end);
uint64_t RPVC_TIME_DiffUs(uint64_t start, uint64_t end);
uint64_t RPVC_TIME_DiffMs(uint64_t start, uint64_t end);

/* Delay functions */
RPVC_Status_t RPVC_TIME_DelayUs(uint32_t us);
RPVC_Status_t RPVC_TIME_DelayMs(uint32_t ms);

/* Configuration */
RPVC_Status_t RPVC_TIME_GetTickFrequency(uint32_t *outFrequency);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H