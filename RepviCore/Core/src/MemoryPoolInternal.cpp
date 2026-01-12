#include "MemoryPoolInternal.hpp"

namespace RPVC {

    RPVC_Status_t MemoryPoolManager::Init()
    {
        if (isInitialized_) {
            return RPVC_ERR_STATE;
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
            return memoryPool32_.AllocateBlock();
        }
        else if (size <= 64) {
            return memoryPool64_.AllocateBlock();
        }
        else if (size <= 128) {
            return memoryPool128_.AllocateBlock();
        }
        else {
            return nullptr; // Size too large
        }
    }
};