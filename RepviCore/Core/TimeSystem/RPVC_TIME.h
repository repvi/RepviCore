#ifndef RPVC_TIME_H
#define RPVC_TIME_H

#include "core/core_types.h"

typedef struct {
    volatile uint32_t *systemTickCounter;
} RPVC_TimeConfig_t;

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_TIME_Init(const RPVC_TimeConfig_t *config);

void RPVC_TIME_Deinit(void);

bool RPVC_TIME_IsInitialized(void);

uint32_t RPVC_TIME_GetSystemTick(void);

RPVC_EXTERN_C_END

#endif // RPVC_TIME_H