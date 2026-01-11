#ifndef REPVICORE_LOGGING_DEF_H
#define REPVICORE_LOGGING_DEF_H

#include "compile_time.h"
#include <stdint.h>

RPVC_EXTERN_C_BEGIN

/**
 * @brief Log level enumeration
 * @note Higher numeric values indicate more verbose logging
 */
typedef enum RPVC_LogLevel_e {
    RPVC_LOG_LEVEL_NONE    = 0,  /**< No logging */
    RPVC_LOG_LEVEL_ERROR   = 1,  /**< Error conditions only */
    RPVC_LOG_LEVEL_WARNING = 2,  /**< Warnings and errors */
    RPVC_LOG_LEVEL_INFO    = 3,  /**< Informational messages */
    RPVC_LOG_LEVEL_DEBUG   = 4,  /**< Debug messages */
    RPVC_LOG_LEVEL_TRACE   = 5   /**< Detailed trace messages */
} RPVC_LogLevel_t;

/**
 * @brief Maximum log message length in bytes
 * @note NASA guideline: Fixed buffer sizes for predictable memory usage
 */
#define RPVC_LOG_MAX_MESSAGE_LENGTH  (1024U)

/**
 * @brief Maximum number of log sinks/handlers
 * @note Allows multiple output destinations (UART, file, etc.)
 */
#define RPVC_LOG_MAX_SINKS           (4U)

/**
 * @brief Log buffer safety margin in bytes
 * @note Reserve space to prevent buffer overrun
 */
#define RPVC_LOG_BUFFER_MARGIN       (64U)

/**
 * @brief Minimum valid log level
 */
#define RPVC_LOG_LEVEL_MIN           RPVC_LOG_LEVEL_NONE

/**
 * @brief Maximum valid log level
 */
#define RPVC_LOG_LEVEL_MAX           RPVC_LOG_LEVEL_TRACE

/**
 * @brief Validate log level is in acceptable range
 * @param level Log level to validate
 * @return true if valid, false otherwise
 */
#define RPVC_LOG_LEVEL_IS_VALID(level) \
    (((level) >= RPVC_LOG_LEVEL_MIN) && ((level) <= RPVC_LOG_LEVEL_MAX))

/**
 * @brief Log sink callback function type
 * @param level Log level of the message
 * @param message Null-terminated log message
 * @param length Length of message in bytes (excluding null terminator)
 * @note Callback must be reentrant and non-blocking
 */
typedef void (*RPVC_LogSink_t)(RPVC_LogLevel_t level, const char *message, size_t length);

RPVC_EXTERN_C_END

#endif // REPVICORE_LOGGING_DEF_H
