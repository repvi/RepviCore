#include "RPVC_Validator_api.h"
#include "RPVC_Validator.hpp"
#include "events/EventManager.hpp"

using namespace repvicore;

static RPVC_Status_t RPVC_Validator_TryRecordEvent(const RPVC_EventPacket_t* eventPacket)
{
    // Defensive programming: ensure pointer is valid
    if (eventPacket == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }

    return EventManager::recordEvent(eventPacket);
}

extern "C" {

RPVC_Status_t RPVC_Validator_CheckTrue(bool value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isTrue(value)) {
        return RPVC_OK;
    } 
    
    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckFalse(bool value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isFalse(value)) {
        return RPVC_OK;
    } 
    
    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckEqual(bool a, bool b, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isEqual(a, b)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckNotEqual(bool a, bool b, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isNotEqual(a, b)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckInRange_int(int value, int min, int max, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isInRange(value, min, max)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckOutOfRange_int(int value, int min, int max, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isOutOfRange(value, min, max)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckGreater(int value, int threshold, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isGreater(value, threshold)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckGreaterOrEqual(int value, int threshold, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isGreaterOrEqual(value, threshold)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckLess(int value, int threshold, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isLess(value, threshold)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckLessOrEqual(int value, int threshold, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isLessOrEqual(value, threshold)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckZero(int value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isZero(value)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckNonZero(int value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isNonZero(value)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckInRange_float(float value, float min, float max, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isInRange(value, min, max)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckFinite(float value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isFinite(value)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckNaN(float value, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isNaN(value)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckApproximately(float a, float b, float epsilon, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isApproximately(a, b, epsilon)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckNull(const void* ptr, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isNull(ptr)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

RPVC_Status_t RPVC_Validator_CheckNotNull(const void* ptr, const RPVC_EventPacket_t *eventPacket) 
{
    RPVC_Validator v;
    if (v.isNotNull(ptr)) {
        return RPVC_OK;
    }

    return RPVC_Validator_TryRecordEvent(eventPacket);
}

} // extern "C"