#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

uint32_t RPVC_Time_GetTick(void);
uint64_t RPVC_Time_GetMicroseconds(void);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H