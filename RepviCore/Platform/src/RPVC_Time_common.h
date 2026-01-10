/* Internal common time functions - do not include directly in user code */
#ifndef RPVC_TIME_COMMON_H
#define RPVC_TIME_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Internal API - called from platform HAL implementations */
void RPVC_Time_SetInitialized(bool initialized);
bool RPVC_Time_IsInitialized(void);
uint64_t RPVC_Time_GetTick64(void);

#ifdef __cplusplus
}
#endif

#endif /* RPVC_TIME_COMMON_H */
