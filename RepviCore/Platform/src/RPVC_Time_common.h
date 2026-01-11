/* Internal common time functions - do not include directly in user code */
#ifndef RPVC_TIME_COMMON_H
#define RPVC_TIME_COMMON_H

#include "core/core_types.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Internal API - called from platform HAL implementations */
void RPVC_Time_SetInitialized(bool initialized);
bool RPVC_Time_IsInitialized(void);

/* Tick counter access (returns status, uses output parameter) */
RPVC_Status_t RPVC_Time_GetTick64(uint64_t *outTick);
RPVC_Status_t RPVC_Time_GetTick(uint32_t *outTick);

#ifdef __cplusplus
}
#endif

#endif /* RPVC_TIME_COMMON_H */
