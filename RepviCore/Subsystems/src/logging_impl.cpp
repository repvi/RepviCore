/**
 * @file logging_impl.cpp
 * @brief C API implementation for logging subsystem
 * @note NASA-grade defensive programming: All pointers validated, all states checked
 */

#include "logging.h"
#include "LogManager.hpp"

using namespace repvicore_internal;

/* NASA guideline: Explicit initialization state tracking */
static bool g_loggingApiInitialized = false;

RPVC_Status_t RPVC_LOG_Init(void)
{
    /* Prevent double initialization */
    if (g_loggingApiInitialized) {
        return RPVC_ERR_INIT;
    }

    RPVC_Status_t status = LogManager::Init();
    if (status == RPVC_OK) {
        g_loggingApiInitialized = true;
    }
    
    return status;
}

RPVC_Status_t RPVC_LOG_Deinit(void)
{
    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    RPVC_Status_t status = LogManager::Deinit();
    if (status == RPVC_OK) {
        g_loggingApiInitialized = false;
    }
    
    return status;
}

bool RPVC_LOG_IsInitialized(void)
{
    return g_loggingApiInitialized && LogManager::isInitialized();
}

RPVC_Status_t RPVC_LOG_Message(const char *message)
{
    /* NASA guideline: Defensive null pointer check */
    if (message == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::Log(message);
}

RPVC_Status_t RPVC_LOG_MessageLevel(RPVC_LogLevel_t level, const char *message)
{
    /* NASA guideline: Validate all inputs */
    if (message == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (!RPVC_LOG_LEVEL_IS_VALID(level)) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::LogLevel(level, message);
}

RPVC_Status_t RPVC_LOG_Binary(const void *data, size_t len)
{
    /* NASA guideline: Validate all inputs */
    if (data == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    if (len == 0) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::LogBinary(data, len);
}

RPVC_Status_t RPVC_LOG_SetLevel(RPVC_LogLevel_t level)
{
    /* Validate log level */
    if (!RPVC_LOG_LEVEL_IS_VALID(level)) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::SetLevel(level);
}

RPVC_Status_t RPVC_LOG_GetLevel(RPVC_LogLevel_t *outLevel)
{
    /* Validate output pointer */
    if (outLevel == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::GetLevel(outLevel);
}

RPVC_Status_t RPVC_LOG_GetBuffer(const uint8_t **out, size_t *len)
{
    /* Validate output pointers */
    if (out == NULL || len == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    return LogManager::GetBuffer(out, len);
}

RPVC_Status_t RPVC_LOG_Flush(void)
{
    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    LogManager::Flush();
    return RPVC_OK;
}

RPVC_Status_t RPVC_LOG_Clear(void)
{
    /* Check initialization state */
    if (!g_loggingApiInitialized) {
        return RPVC_ERR_NOT_READY;
    }

    LogManager::Clear();
    return RPVC_OK;
}

/* Convenience functions - wrappers around RPVC_LOG_MessageLevel */

RPVC_Status_t RPVC_LOG_Error(const char *message)
{
    return RPVC_LOG_MessageLevel(RPVC_LOG_LEVEL_ERROR, message);
}

RPVC_Status_t RPVC_LOG_Warning(const char *message)
{
    return RPVC_LOG_MessageLevel(RPVC_LOG_LEVEL_WARNING, message);
}

RPVC_Status_t RPVC_LOG_Info(const char *message)
{
    return RPVC_LOG_MessageLevel(RPVC_LOG_LEVEL_INFO, message);
}

RPVC_Status_t RPVC_LOG_Debug(const char *message)
{
    return RPVC_LOG_MessageLevel(RPVC_LOG_LEVEL_DEBUG, message);
}
