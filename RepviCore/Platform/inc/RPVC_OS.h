#ifndef RPVC_OS_H
#define RPVC_OS_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

void RPVC_OS_Init(void);
void RPVC_OS_Delay(uint32_t ms);
 
RPVC_EXTERN_C_END

#endif // RPVC_OS_H