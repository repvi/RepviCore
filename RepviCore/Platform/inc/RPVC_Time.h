#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"
#include "compile_time.h"
#include <stdbool.h>
#include <stdint.h>

RPVC_EXTERN_C_BEGIN

/* Platform-specific initialization */
RPVC_Status_t RPVC_Time_Init(void);

/* Query initialization state */
bool RPVC_Time_IsInitialized(void);

/* Tick management - call from platform timer ISR */
void RPVC_Time_OnTick(void);

/* Optional user hook called on each tick (weak function) */
void RPVC_Time_UserTickHook(void);

/* Get current tick count */
uint32_t RPVC_Time_GetTick(void);
uint64_t RPVC_Time_GetTick64(void);

/* High-precision timing */
uint64_t RPVC_Time_GetMicroseconds(void);
uint64_t RPVC_Time_GetTimeMilliseconds(void);

/* Time difference calculations */
uint32_t RPVC_Time_TickDiff(uint32_t start, uint32_t end);
uint64_t RPVC_Time_TimeDiffUs(uint64_t start, uint64_t end);
uint64_t RPVC_Time_TimeDiffMs(uint64_t start, uint64_t end);

/* Delay functions */
void RPVC_Time_DelayUs(uint32_t us);
void RPVC_Time_DelayMs(uint32_t ms);

/* Configuration */
uint32_t RPVC_Time_GetTickFrequency(void);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H