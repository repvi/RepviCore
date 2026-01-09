#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"

RPVC_EXTERN_C_BEGIN

void RPVC_TIME_Init(void);

void RPVC_TIME_Deinit(void);

void RPVC_TIME_Configure(volatile uint32_t* systemTickCounter);

uint32_t RPVC_TIME_GetSystemTick(void);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H