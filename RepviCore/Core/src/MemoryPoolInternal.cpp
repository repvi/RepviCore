#include "MemoryPoolInternal.hpp"

namespace RPVC {
    MemoryPool<MemoryPoolManager::MEMPOOL32_BUFFER_SIZE, MemoryPoolManager::MEMPOOL32_BLOCK_SIZE> MemoryPoolManager::memoryPool32_;
    MemoryPool<MemoryPoolManager::MEMPOOL64_BUFFER_SIZE, MemoryPoolManager::MEMPOOL64_BLOCK_SIZE> MemoryPoolManager::memoryPool64_;
    MemoryPool<MemoryPoolManager::MEMPOOL128_BUFFER_SIZE, MemoryPoolManager::MEMPOOL128_BLOCK_SIZE> MemoryPoolManager::memoryPool128_;

    bool MemoryPoolManager::isInitialized_ = false;
    
    RPVC_Status_t MemoryPoolManager::Init()
    {
        if (memoryPool32_.Init() != RPVC_OK) {
            return RPVC_ERR_INIT;
        }
        if (memoryPool64_.Init() != RPVC_OK) {
            return RPVC_ERR_INIT;
        }
        if (memoryPool128_.Init() != RPVC_OK) {
            return RPVC_ERR_INIT;
        }

        isInitialized_ = true;
        return RPVC_OK;
    }

    RPVC_Status_t MemoryPoolManager::Deinit()
    {
        if (!isInitialized_) {
            return RPVC_ERR_NOT_READY;
        }
        isInitialized_ = false;
        return RPVC_OK;
    }

    bool MemoryPoolManager::IsInitialized()
    {
        return isInitialized_;
    }

    void *MemoryPoolManager::AllocateBlock(uint32_t size)
    {
        if (size <= 32) {
            void* block;
            if (memoryPool32_.AllocateBlock(&block) == RPVC_OK) {
                return block;
            } else {
                return nullptr;
            }
        }
        else if (size <= 64) {
            void* block;
            if (memoryPool64_.AllocateBlock(&block) == RPVC_OK) {
                return block;
            } else {
                return nullptr;
            }
        }
        else if (size <= 128) {
            void* block;
            if (memoryPool128_.AllocateBlock(&block) == RPVC_OK) {
                return block;
            } else {
                return nullptr;
            }
        }
        else {
            return nullptr; // Size too large
        }
    }

    RPVC_Status_t MemoryPoolManager::FreeBlock(void *ptr)
    {
        if (memoryPool32_.FreeBlock(ptr) == RPVC_OK) {
            return RPVC_OK;
        }
        if (memoryPool64_.FreeBlock(ptr) == RPVC_OK) {
            return RPVC_OK;
        }
        if (memoryPool128_.FreeBlock(ptr) == RPVC_OK) {
            return RPVC_OK;
        }
        return RPVC_ERR_INVALID_ARG; // Pointer not found in any pool
    }
};