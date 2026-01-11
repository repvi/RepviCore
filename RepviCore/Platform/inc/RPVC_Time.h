#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"
#include "compile_time.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t systemHz; /* System tick frequency in Hz */
    uint32_t tickHz;   /* Tick frequency in Hz */
} RPVC_TimeConfig_t;

RPVC_EXTERN_C_BEGIN

/* Platform-specific initialization */
RPVC_Status_t RPVC_Time_Init(const RPVC_TimeConfig_t *config);

/* Query initialization state */
bool RPVC_Time_IsInitialized(void);

/* Tick management - call from platform timer ISR */
void RPVC_Time_OnTick(void);

/* Optional user hook called on each tick (weak function) */
void RPVC_Time_UserTickHook(void);

/* Get current tick count */
RPVC_Status_t RPVC_Time_GetTick(uint32_t *outTick);
RPVC_Status_t RPVC_Time_GetTick64(uint64_t *outTick);

/* High-precision timing */
RPVC_Status_t RPVC_Time_GetMicroseconds(uint64_t *outUs);
RPVC_Status_t RPVC_Time_GetTimeMilliseconds(uint64_t *outMs);

/* Time difference calculations */
uint32_t RPVC_Time_TickDiff(uint32_t start, uint32_t end);
uint64_t RPVC_Time_TimeDiffUs(uint64_t start, uint64_t end);
uint64_t RPVC_Time_TimeDiffMs(uint64_t start, uint64_t end);

/* Delay functions */
RPVC_Status_t RPVC_Time_DelayUs(uint32_t us);
RPVC_Status_t RPVC_Time_DelayMs(uint32_t ms);

/* Configuration */
RPVC_Status_t RPVC_Time_GetTickFrequency(uint32_t *outFrequency);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H