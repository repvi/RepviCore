#ifndef RPVC_MEMORYPOOLINTERNAL_HPP
#define RPVC_MEMORYPOOLINTERNAL_HPP

#include "compile_time.h"
#include "core_types.h"
#include <cstring>

namespace RPVC {
    template<size_t PoolSize, size_t BlockSize>
    class MemoryPool {
        constexpr static size_t NumBlocks = PoolSize / BlockSize;
        public:

        RPVC_Status_t Init() 
        {
            std::memset(pool_, 0, PoolSize);
            std::memset(allocatedFlags_, 0, sizeof(allocatedFlags_));
            return RPVC_OK;
        }

        RPVC_Status_t Deinit() 
        {
            return RPVC_OK;
        }

        RPVC_Status_t AllocateBlock(void **outBlock) 
        {
            if (outBlock == nullptr) {
                return RPVC_ERR_INVALID_ARG;
            }
            
            for (size_t i = 0; i < NumBlocks; ++i) {
                if (!allocatedFlags_[i]) {
                    allocatedFlags_[i] = true;
                    *outBlock = pool_ + (i * BlockSize);
                    return RPVC_OK;
                }
            }
            return RPVC_ERR_NO_MEMORY; // No free blocks
        }

        RPVC_Status_t FreeBlock(void* block) 
        {
            if (block == nullptr) {
                return RPVC_ERR_INVALID_ARG;
            }

            if (!validMemoryBlock(block)) {
                return RPVC_ERR_INVALID_ARG;
            }
            size_t offset = static_cast<uint8_t*>(block) - pool_;

            size_t index = offset / BlockSize;
            if (!allocatedFlags_[index]) {
                return RPVC_ERR_INVALID_ARG; // Block already free
            }
            allocatedFlags_[index] = false;
            return RPVC_OK;
        }

        size_t GetBlockSize() const 
        {
            return BlockSize;
        }

        size_t GetPoolSize() const 
        {
            return PoolSize;
        }

        size_t GetFreeBlockCount() const 
        {
            size_t count = 0;
            for (size_t i = 0; i < NumBlocks; ++i) {
                if (!allocatedFlags_[i]) {
                    ++count;
                }
            }
            return count;
        }

        size_t GetUsedBlockCount() const 
        {
            return NumBlocks - GetFreeBlockCount();
        }

        size_t GetTotalBlockCount() const 
        {
            return NumBlocks;
        }

        private:

        bool validMemoryBlock(void* block)
        {
            return (pool_ <= block) && (static_cast<uint8_t*>(block) < pool_ + PoolSize) &&
                   (((static_cast<uint8_t*>(block) - pool_) % BlockSize) == 0);
        }

        uint8_t pool_[PoolSize];
        bool allocatedFlags_[NumBlocks];
    };

    class MemoryPoolManager {
        public:
        static RPVC_Status_t Init();
        static RPVC_Status_t Deinit();
        static bool IsInitialized();
        static void *AllocateBlock(uint32_t size);

        private:


        static MemoryPool<1024 * 5, 32> memoryPool32_;
        static MemoryPool<1024 * 10, 64> memoryPool64_;
        static MemoryPool<1024 * 20, 128> memoryPool128_;

        static bool isInitialized_;
    };
};

#endif // RPVC_MEMORYPOOLINTERNAL_HPP