#ifndef RPVC_SYSTEM_H
#define RPVC_SYSTEM_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_System_Init(void);

RPVC_Status_t RPVC_System_Deinit(void);

void RPVC_System_Idle(void);

void RPVC_System_Reset(void);

RPVC_Status_t RPVC_System_GetCPUName(const char **cpuName);

uint32_t RPVC_System_GetCPUID(void);

uint32_t RPVC_System_GetCycleCount(void);

RPVC_EXTERN_C_END

#endif // RPVC_SYSTEM_H