#ifndef REPVICORE_CORE_ERRORS_H
#define REPVICORE_CORE_ERRORS_H

#include <stdint.h>
#include "ct/ct_macros.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

typedef enum RPVC_Status_e : int32_t {
    RPVC_OK               = 0,
    RPVC_ERR_INIT         = -1,
    RPVC_ERR_CONFIG       = -2,
    RPVC_ERR_NOT_READY    = -3,
    RPVC_ERR_INVALID_ARG  = -4,
    RPVC_ERR_OUT_OF_RANGE = -5,
    RPVC_ERR_STATE        = -6,
    RPVC_ERR_TIMEOUT      = -7,
    RPVC_ERR_NO_MEMORY    = -8,
    RPVC_ERR_NO_RESOURCE  = -9,
    RPVC_ERR_INTERNAL     = -10,
    RPVC_ERR_INTEGRITY    = -11
} RPVC_Status_t;

RPVC_EXTERN_C_END

#endif // REPVICORE_CORE_ERRORS_H