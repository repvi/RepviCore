#ifndef RPVC_PLATFORM_H
#define RPVC_PLATFORM_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

void RPVC_Platform_Init(void);

const char* RPVC_Platform_GetName(void);

uint32_t RPVC_Platform_GetCapabilities(void);

void RPVC_Platform_Yield(void);

void RPVC_Platform_Idle(void);

RPVC_EXTERN_C_END

#endif // RPVC_PLATFORM_H