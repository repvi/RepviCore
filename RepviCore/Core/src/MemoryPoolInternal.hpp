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
            return RPVC_OK;
        }

        RPVC_Status_t Deinit() 
        {
            return RPVC_OK;
        }

        void* AllocateBlock() 
        {
            for (size_t i = 0; i < NumBlocks; ++i) {
                if (!allocatedFlags_[i]) {
                    allocatedFlags_[i] = true;
                    return pool_ + (i * BlockSize);
                }
            }
            return nullptr; // No free blocks
        }

        RPVC_Status_t FreeBlock(void* block) 
        {
            if (block == nullptr) {
                return RPVC_ERR_INVALID_ARG;
            }
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