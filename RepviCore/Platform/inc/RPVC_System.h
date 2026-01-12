#ifndef RPVC_SYSTEM_H
#define RPVC_SYSTEM_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_SYSTEM_Init(void);

RPVC_Status_t RPVC_SYSTEM_Deinit(void);

void RPVC_SYSTEM_Idle(void);

void RPVC_SYSTEM_Reset(void);

RPVC_Status_t RPVC_SYSTEM_GetCPUName(const char **cpuName);

uint32_t RPVC_SYSTEM_GetCPUID(void);

RPVC_Status_t RPVC_SYSTEM_GetCycleCount(uint32_t *cycleCount);

RPVC_EXTERN_C_END

#endif // RPVC_SYSTEM_H