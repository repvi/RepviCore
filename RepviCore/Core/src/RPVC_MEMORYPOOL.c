#include "RPVC_MEMORYPOOL.h"
#include <string.h>

#define MEMORY_POOL_SIZE (1024 * 20)  // 20 KB
#define BLOCK_SIZE 32               // 32 bytes per block   

static uint8_t g_memoryPool[MEMORY_POOL_SIZE];
static bool g_memoryPoolBBlocks[MEMORY_POOL_SIZE / BLOCK_SIZE]; // Bitmap for 32-byte blocks
static size_t g_remainingMemory = MEMORY_POOL_SIZE;
static bool g_isInitialized = false;

RPVC_Status_t RPVC_MEMORYPOOL_Init(void)
{
    if (g_isInitialized) {
        return RPVC_ERR_STATE;
    }
    
    memset(g_memoryPool, 0, sizeof(g_memoryPool));
    memset(g_memoryPoolBBlocks, 0, sizeof(g_memoryPoolBBlocks));
    g_remainingMemory = MEMORY_POOL_SIZE;
    g_isInitialized = true;
    
    return RPVC_OK;
}

RPVC_Status_t RPVC_MEMORYPOOL_Deinit(void)
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }
    g_isInitialized = false;
    return RPVC_OK;
}

bool RPVC_MEMORYPOOL_IsInitialized(void)
{
    return g_isInitialized;
}

RPVC_Status_t RPVC_MEMORYPOOL_Allocate(void** outPtr)
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (outPtr == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    for (size_t i = 0; i < sizeof(g_memoryPoolBBlocks) / sizeof(g_memoryPoolBBlocks[0]); i++) {
        if (g_memoryPoolBBlocks[i] == false) {
            g_memoryPoolBBlocks[i] = true; // Mark block as used
            *outPtr = &g_memoryPool[i * BLOCK_SIZE];
            g_remainingMemory -= BLOCK_SIZE;
            return RPVC_OK;
        }
    }
    return RPVC_ERR_NO_MEMORY;
}

static bool isPointerInPool(void* ptr)
{
    return (ptr >= (void*)g_memoryPool) && (ptr < (void*)(g_memoryPool + MEMORY_POOL_SIZE));
}

RPVC_Status_t RPVC_MEMORYPOOL_Free(void* ptr)
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    if (ptr == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (!isPointerInPool(ptr)) {
        return RPVC_ERR_INVALID_ARG;
    }

    size_t offset = (uint8_t*)ptr - g_memoryPool;
    size_t blockIndex = offset / BLOCK_SIZE;
    g_memoryPoolBBlocks[blockIndex] = false; // Mark block as free
    g_remainingMemory += BLOCK_SIZE;
    return RPVC_OK;
}

RPVC_Status_t RPVC_MEMORYPOOL_GetStats(size_t* totalAllocated, size_t* totalFree)
{
    if (!g_isInitialized) {
        return RPVC_ERR_NOT_READY;
    }
    if (totalAllocated == NULL || totalFree == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    *totalFree = g_remainingMemory;
    *totalAllocated = MEMORY_POOL_SIZE - g_remainingMemory;
    return RPVC_OK;
}