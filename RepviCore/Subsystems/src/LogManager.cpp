#include "LogManager.hpp"
#include <cstring>
#include "event_api.h"
#include "RPVC_TIME.h"

namespace repvicore_internal {
    bool LogManager::isInit = false;
    uint8_t LogManager::logBuffer[MAX_LOG_MESSAGE_LENGTH] = {0};
    size_t LogManager::bufferPosition = 0;
    RPVC_LogLevel_t LogManager::currentLogLevel = RPVC_LOG_LEVEL_INFO;

    RPVC_Status_t LogManager::Init() 
    {
        std::memset(logBuffer, 0, MAX_LOG_MESSAGE_LENGTH);
        bufferPosition = 0;
        isInit = true;
        currentLogLevel = RPVC_LOG_LEVEL_INFO;
        return RPVC_OK;
    }

    RPVC_Status_t LogManager::Deinit()
    {
        isInit = false;
        return RPVC_OK;
    }

    bool LogManager::isInitialized()
    {
        return isInit;
    }

    RPVC_Status_t LogManager::Log(const char *message) 
    {
        size_t messageLength = strnlen(message, MAX_LOG_MESSAGE_LENGTH);
        if (messageLength == 0) {
            return RPVC_ERR_INVALID_ARG; // Empty message
        }

        if (bufferPosition + messageLength >= MAX_LOG_MESSAGE_LENGTH) {
            return RPVC_ERR_OUT_OF_RANGE; // Not enough space in buffer
        }

        std::memcpy(&logBuffer[bufferPosition], message, messageLength);
        bufferPosition += messageLength;
        return RPVC_OK;
    }

    RPVC_Status_t LogManager::LogLevel(RPVC_LogLevel_t level, const char *message)
    {
        switch (level) {
            case RPVC_LOG_LEVEL_ERROR: {
                RPVC_EventPacket_t eventPacket;
                eventPacket.severity = RPVC_SEVERITY_ERROR;
                eventPacket.eventId = RPVC_EVENT_ERROR;
                RPVC_Status_t status = RPVC_Time_GetMicroseconds(&eventPacket.timestamp);
                RPVC_EVENT_Record(&eventPacket); // Placeholder for event recording
            }
                return WriteToBuffer("[ERROR]", message);
            case RPVC_LOG_LEVEL_WARNING:
                return WriteToBuffer("[WARNING]", message);
            case RPVC_LOG_LEVEL_INFO:
                return WriteToBuffer("[INFO]", message);
            case RPVC_LOG_LEVEL_DEBUG:
                return WriteToBuffer("[DEBUG]", message);
            case RPVC_LOG_LEVEL_TRACE:
                return WriteToBuffer("[TRACE]", message);
            case RPVC_LOG_LEVEL_NONE:
            default:
                return RPVC_ERR_INVALID_ARG; // Invalid log level
        }
    }

    RPVC_Status_t LogManager::LogBinary(const void *data, size_t len)
    {
        size_t totalLen = len + 1;
        size_t nextBufferPosition = bufferPosition + totalLen; // +1 for newline
        if (nextBufferPosition >= MAX_LOG_MESSAGE_LENGTH) {
            return RPVC_ERR_OUT_OF_RANGE; // Not enough space in buffer
        }
        std::memcpy(&logBuffer[bufferPosition], data, len);
        logBuffer[bufferPosition + len] = '\n';
        bufferPosition += totalLen;
        return RPVC_OK;
    }

    void LogManager::Flush()
    {

    }

    void LogManager::Clear()
    {
        std::memset(logBuffer, 0, MAX_LOG_MESSAGE_LENGTH);
        bufferPosition = 0;
    }

    RPVC_Status_t LogManager::GetBuffer(const uint8_t **out, size_t *len)
    {
        std::memcpy(out, logBuffer, bufferPosition);
        *len = bufferPosition;
        return RPVC_OK;
    }

    RPVC_Status_t LogManager::GetLastError(char *buffer, size_t bufferLen)
    {
        return RPVC_Status_t();
    }

    RPVC_Status_t LogManager::SetLevel(RPVC_LogLevel_t level)
    {
        currentLogLevel = level;
        return RPVC_OK;
    }

    RPVC_Status_t LogManager::GetLevel(RPVC_LogLevel_t *level)
    {
        *level = currentLogLevel;
        return RPVC_OK;
    }

    RPVC_Status_t LogManager::WriteToBuffer(const char *label, const char *message)
    {
        size_t labelSize = strnlen(label, MAX_LOG_MESSAGE_LENGTH);
        size_t messageSize = strnlen(message, MAX_LOG_MESSAGE_LENGTH);
        size_t totalSize = labelSize + messageSize + 2; // +2 for ": " or "\n"
        if (bufferPosition + totalSize >= MAX_LOG_MESSAGE_LENGTH) {
            return RPVC_ERR_OUT_OF_RANGE; // Not enough space in buffer
        }

        std::memcpy(&logBuffer[bufferPosition], label, labelSize);
        bufferPosition += labelSize;
        logBuffer[bufferPosition++] = ':';
        logBuffer[bufferPosition++] = ' ';
        std::memcpy(&logBuffer[bufferPosition], message, messageSize);
        bufferPosition += messageSize;
        logBuffer[bufferPosition++] = '\n';
        return RPVC_OK;
    }
};