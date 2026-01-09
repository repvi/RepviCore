#ifndef RPVC_VALIDATOR_API_H
#define RPVC_VALIDATOR_API_H

#include "compile_time.h"
#include "core_types.h"
#include "event_type.h"

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_Validator_CheckTrue(bool value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckFalse(bool value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckEqual(bool a, bool b, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckNotEqual(bool a, bool b, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckInRange_int(int value, int min, int max, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckOutOfRange_int(int value, int min, int max, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckGreater(int value, int threshold, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckGreaterOrEqual(int value, int threshold, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckLess(int value, int threshold, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckLessOrEqual(int value, int threshold, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckZero(int value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckNonZero(int value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckInRange_float(float value, float min, float max, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckFinite(float value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckNaN(float value, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckApproximately(float a, float b, float epsilon, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckNull(const void* ptr, const RPVC_EventPacket_t *eventPacket);
RPVC_Status_t RPVC_Validator_CheckNotNull(const void* ptr, const RPVC_EventPacket_t *eventPacket);


RPVC_EXTERN_C_END

#endif // RPVC_VALIDATOR_API_H