#ifndef REPVICORE_LOGGING_H
#define REPVICORE_LOGGING_H

#include "core_types.h"
#include "core_errors.h"
#include "compile_time.h"
#include "logging_def.h"
#include <stddef.h>

RPVC_EXTERN_C_BEGIN

/**
 * @brief Initialize the logging subsystem
 * @return RPVC_OK on success, error code otherwise
 * @pre None
 * @post Logging system is initialized and ready
 */
RPVC_Status_t RPVC_LOG_Init(void);

/**
 * @brief Deinitialize the logging subsystem
 * @return RPVC_OK on success, error code otherwise
 * @pre Logging system must be initialized
 * @post Logging system is shut down
 */
RPVC_Status_t RPVC_LOG_Deinit(void);

/**
 * @brief Check if logging system is initialized
 * @return true if initialized, false otherwise
 */
bool RPVC_LOG_IsInitialized(void);

/**
 * @brief Log a message with default log level
 * @param message Null-terminated message string (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 * @pre message != NULL, logging system initialized
 */
RPVC_Status_t RPVC_LOG_Message(const char *message);

/**
 * @brief Log a message with specified log level
 * @param level Log level (must be valid RPVC_LogLevel_t)
 * @param message Null-terminated message string (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 * @pre message != NULL, logging system initialized, level is valid
 */
RPVC_Status_t RPVC_LOG_MessageLevel(RPVC_LogLevel_t level, const char *message);

/**
 * @brief Log binary data
 * @param data Pointer to binary data (must not be NULL)
 * @param len Length of data in bytes (must be > 0)
 * @return RPVC_OK on success, error code otherwise
 * @pre data != NULL, len > 0, logging system initialized
 */
RPVC_Status_t RPVC_LOG_Binary(const void *data, size_t len);

/**
 * @brief Set the current log level filter
 * @param level New log level
 * @return RPVC_OK on success, error code otherwise
 * @pre Logging system initialized, level is valid
 */
RPVC_Status_t RPVC_LOG_SetLevel(RPVC_LogLevel_t level);

/**
 * @brief Get the current log level filter
 * @param outLevel Pointer to receive log level (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 * @pre outLevel != NULL, logging system initialized
 */
RPVC_Status_t RPVC_LOG_GetLevel(RPVC_LogLevel_t *outLevel);

/**
 * @brief Get pointer to internal log buffer
 * @param out Pointer to receive buffer address (must not be NULL)
 * @param len Pointer to receive buffer length (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 * @pre out != NULL, len != NULL, logging system initialized
 */
RPVC_Status_t RPVC_LOG_GetBuffer(const uint8_t **out, size_t *len);

/**
 * @brief Flush the log buffer
 * @return RPVC_OK on success, error code otherwise
 * @pre Logging system initialized
 */
RPVC_Status_t RPVC_LOG_Flush(void);

/**
 * @brief Clear the log buffer
 * @return RPVC_OK on success, error code otherwise
 * @pre Logging system initialized
 */
RPVC_Status_t RPVC_LOG_Clear(void);

/**
 * @brief Log error message (convenience function)
 * @param message Error message (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 */
RPVC_Status_t RPVC_LOG_Error(const char *message);

/**
 * @brief Log warning message (convenience function)
 * @param message Warning message (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 */
RPVC_Status_t RPVC_LOG_Warning(const char *message);

/**
 * @brief Log info message (convenience function)
 * @param message Info message (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 */
RPVC_Status_t RPVC_LOG_Info(const char *message);

/**
 * @brief Log debug message (convenience function)
 * @param message Debug message (must not be NULL)
 * @return RPVC_OK on success, error code otherwise
 */
RPVC_Status_t RPVC_LOG_Debug(const char *message);

RPVC_EXTERN_C_END

#endif // REPVICORE_LOGGING_H