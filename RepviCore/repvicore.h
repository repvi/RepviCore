#ifndef REPVICORE_H
#define REPVICORE_H

#include "core/core_types.h"
#include "core/core_errors.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_Init(void);

RPVC_Status_t RPVC_Shutdown(void);

RPVC_Status_t RPVC_Deinit(void);

RPVC_EXTERN_C_END



#endif // REPVICORE_H