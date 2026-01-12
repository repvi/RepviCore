#include "RPVC_MEMORYPOOL.h"
#include "MemoryPoolInternal.hpp"
#include <string.h>

#define MEMORY_POOL_SIZE (1024 * 20)  // 20 KB
#define BLOCK_SIZE 32               // 32 bytes per block   

using namespace RPVC;

RPVC_Status_t RPVC_MEMORYPOOL_Init(void)
{
    if (MemoryPoolManager::IsInitialized()) {
        return RPVC_ERR_STATE;
    }

    MemoryPoolManager::Init();
    
    return RPVC_OK;
}

RPVC_Status_t RPVC_MEMORYPOOL_Deinit(void)
{
    if (!MemoryPoolManager::IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }
    MemoryPoolManager::Deinit();
    return RPVC_OK;
}

bool RPVC_MEMORYPOOL_IsInitialized(void)
{
    return MemoryPoolManager::IsInitialized();
}

RPVC_Status_t RPVC_MEMORYPOOL_Allocate(size_t size, void** outPtr)
{
    if (!MemoryPoolManager::IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }

    if (outPtr == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    *outPtr = MemoryPoolManager::AllocateBlock(size); // Example with fixed size, adjust as needed
    if (*outPtr != NULL) {
        return RPVC_OK;
    }

    return RPVC_ERR_NO_MEMORY;
}

RPVC_Status_t RPVC_MEMORYPOOL_Free(void* ptr)
{
    if (!MemoryPoolManager::IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }

    if (ptr == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    return MemoryPoolManager::FreeBlock(ptr);
}

RPVC_Status_t RPVC_MEMORYPOOL_GetStats(size_t* totalAllocated, size_t* totalFree)
{
    if (!MemoryPoolManager::IsInitialized()) {
        return RPVC_ERR_NOT_READY;
    }
    if (totalAllocated == NULL || totalFree == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }


    return RPVC_ERR_NOT_READY; // Placeholder implementation
}