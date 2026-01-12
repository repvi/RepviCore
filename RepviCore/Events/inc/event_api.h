#ifndef REPVICORE_EVENT_API_H
#define REPVICORE_EVENT_API_H

#include "core_types.h"
#include "core_errors.h"
#include "event_type.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_EVENT_Init(void);

RPVC_Status_t RPVC_EVENT_RegisterHandler(RPVC_EventCallback_t callback);

RPVC_Status_t RPVC_EVENT_Record(const RPVC_EventPacket_t *eventPacket);

void RPVC_EVENT_Dispatch(void);

RPVC_EXTERN_C_END

#endif // REPVICORE_EVENT_API_H