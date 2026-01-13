#ifndef REPVICORE_INTERNAL_LOGMANAGER_HPP
#define REPVICORE_INTERNAL_LOGMANAGER_HPP

#include <cstddef>
#include <cstdint>
#include "core_types.h"
#include "core_errors.h"
#include "logging.h"

namespace repvicore_internal {

    class LogManager {
    public:
        static RPVC_Status_t Init();
        static RPVC_Status_t Deinit();
        static bool isInitialized();
        
        static RPVC_Status_t Log(const char *message);
        static RPVC_Status_t LogLevel(RPVC_LogLevel_t level, const char *message);
        static RPVC_Status_t LogBinary(const void *data, size_t len);
        
        static void Flush();
        static void Clear();
        
        static RPVC_Status_t GetBuffer(const uint8_t **out, size_t *len);
        static RPVC_Status_t GetLastError(char *buffer, size_t bufferLen);
        
        static RPVC_Status_t SetLevel(RPVC_LogLevel_t level);
        static RPVC_Status_t GetLevel(RPVC_LogLevel_t *level);

    private:
        static constexpr int MAX_LOG_MESSAGE_LENGTH = 1024;
        
        static bool isInit;
        static uint8_t logBuffer[MAX_LOG_MESSAGE_LENGTH];
        static size_t bufferPosition;
        static RPVC_LogLevel_t currentLogLevel;
        
        static RPVC_Status_t WriteToBuffer(const char *label, const char *message);
    };

}

#endif // REPVICORE_INTERNAL_LOGMANAGER_HPP
