#ifndef RPVC_MEMORYPOOL_H
#define RPVC_MEMORYPOOL_H

#include "compile_time.h"
#include "core_types.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_MEMORYPOOL_Init(void);

RPVC_Status_t RPVC_MEMORYPOOL_Deinit(void);

bool RPVC_MEMORYPOOL_IsInitialized(void);

RPVC_Status_t RPVC_MEMORYPOOL_Allocate(size_t size, void** outPtr);

RPVC_Status_t RPVC_MEMORYPOOL_Free(void* ptr);

RPVC_Status_t RPVC_MEMORYPOOL_GetStats(size_t* totalAllocated, size_t* totalFree);

RPVC_EXTERN_C_END

#endif // RPVC_MEMORYPOOL_H