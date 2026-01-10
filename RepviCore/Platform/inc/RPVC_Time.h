#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_Time_Init(void);

uint32_t RPVC_Time_GetTick(void);
uint64_t RPVC_Time_GetMicroseconds(void);
uint64_t RPVC_Time_GetTimeMilliseconds(void);

uint32_t RPVC_Time_TickDiff(uint32_t start, uint32_t end);
uint64_t RPVC_Time_TimeDiffUs(uint64_t start, uint64_t end);

uint64_t RPVC_Time_TimeDiffMs(uint64_t start, uint64_t end);

void RPVC_Time_DelayUs(uint32_t us);
void RPVC_Time_DelayMs(uint32_t ms);

uint32_t RPVC_Time_GetTickFrequency(void);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H