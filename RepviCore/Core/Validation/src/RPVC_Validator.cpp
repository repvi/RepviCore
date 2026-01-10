#include "RPVC_Validator.hpp"
#include <cmath>
#include <cstring>

bool repvicore::RPVC_Validator::isTrue(bool value)
{
    return value;
}

bool repvicore::RPVC_Validator::isFalse(bool value)
{
    return !value;
}

bool repvicore::RPVC_Validator::isEqual(bool a, bool b)
{
    return (a == b);
}

bool repvicore::RPVC_Validator::isNotEqual(bool a, bool b)
{
    return (a != b);
}

bool repvicore::RPVC_Validator::isInRange(int value, int min, int max)
{
    return (min <= value && value <= max);
}

bool repvicore::RPVC_Validator::isOutOfRange(int value, int min, int max)
{
    return (value < min || value > max);
}

bool repvicore::RPVC_Validator::isGreater(int value, int threshold)
{
    return (value > threshold);
}

bool repvicore::RPVC_Validator::isGreaterOrEqual(int value, int threshold)
{
    return (value >= threshold);
}

bool repvicore::RPVC_Validator::isLess(int value, int threshold)
{
    return (value < threshold);
}

bool repvicore::RPVC_Validator::isLessOrEqual(int value, int threshold)
{
    return (value <= threshold);
}

bool repvicore::RPVC_Validator::isZero(int value)
{
    return (value == 0);
}

bool repvicore::RPVC_Validator::isNonZero(int value)
{
    return (value != 0);
}

bool repvicore::RPVC_Validator::isEven(int value)
{
    return ((value & 1) == 0);
}

bool repvicore::RPVC_Validator::isOdd(int value)
{
    return ((value & 1) != 0);
}

bool repvicore::RPVC_Validator::isPositive(int value)
{
    return (value > 0);
}

bool repvicore::RPVC_Validator::isNegative(int value)
{
    return (value < 0);
}

bool repvicore::RPVC_Validator::isPowerOfTwo(int value)
{
    return (value > 0) && ((value & (value - 1)) == 0);
}

// not implemented yet
bool repvicore::RPVC_Validator::isPrime(int value)
{
    (void)value;
    return false;
}

bool repvicore::RPVC_Validator::isInRange(float value, float min, float max)
{
    return (min <= value && value <= max);
}

bool repvicore::RPVC_Validator::isFinite(float value)
{
    return std::isfinite(value);
}

bool repvicore::RPVC_Validator::isNaN(float value)
{
    return std::isnan(value);
}

bool repvicore::RPVC_Validator::isApproximately(float a, float b, float epsilon)
{
    return (std::fabs(a - b) <= epsilon);
}

bool repvicore::RPVC_Validator::clamp(int value, int min, int max)
{
    if (value < min) return min; 
    if (value > max) return max; 
    return value;
}

bool repvicore::RPVC_Validator::clamp(float value, float min, float max)
{
    if (value < min) return min; 
    if (value > max) return max; 
    return value;
}

bool repvicore::RPVC_Validator::ormalize01(float value, float min, float max)
{
    return (value - min) / (max - min);
}

bool repvicore::RPVC_Validator::arraysEqual(const int *a, const int *b, size_t length)
{
    return (std::memcmp(a, b, length * sizeof(int)) == 0);
}

bool repvicore::RPVC_Validator::arraysNotEqual(const int *a, const int *b, size_t length)
{
    return (std::memcmp(a, b, length * sizeof(int)) != 0);
}

bool repvicore::RPVC_Validator::arrayAllZero(const int *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) 
        if (arr[i] != 0) 
            return false; 
    return true;
}

bool repvicore::RPVC_Validator::arrayAnyZero(const int *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) 
        if (arr[i] == 0) 
            return true; 
    return false;
}

bool repvicore::RPVC_Validator::arrayAllPositive(const int *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) 
        if (arr[i] <= 0) 
            return false;
    return true;
}

bool repvicore::RPVC_Validator::arrayAllNegative(const int *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) 
        if (arr[i] >= 0) 
            return false;
    return true;
}

bool repvicore::RPVC_Validator::arrayIsSortedAscending(const int *arr, size_t length)
{
    int prev = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] < prev) {
            return false;
        }
        prev = arr[i];
    }
    return true;
}

bool repvicore::RPVC_Validator::arrayIsSortedDescending(const int *arr, size_t length)
{
    int prev = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] > prev) {
            return false;
        }
        prev = arr[i];
    }
    return true;
}

bool repvicore::RPVC_Validator::isNull(const void *ptr)
{
    return (ptr == nullptr);
}

bool repvicore::RPVC_Validator::isNotNull(const void *ptr)
{
    return (ptr != nullptr);
}

bool repvicore::RPVC_Validator::stringEqual(const char *a, const char *b, size_t length)
{
    return (std::memcmp(a, b, length) == 0);
}

bool repvicore::RPVC_Validator::stringNotEqual(const char *a, const char *b, size_t length)
{
    return (std::memcmp(a, b, length) != 0);
}

bool repvicore::RPVC_Validator::stringLengthInRange(const char *s, size_t length, size_t min, size_t max)
{
    size_t strLen = strnlen(s, length);
    return (strLen >= min && strLen <= max);
}

bool repvicore::RPVC_Validator::stringIsEmpty(const char *s)
{
    return (s[0] == '\0');
}

bool repvicore::RPVC_Validator::stringNotEmpty(const char *s)
{
    return (s[0] != '\0');
}

bool repvicore::RPVC_Validator::allTrue(const bool *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (!arr[i]) {
            return false;
        }
    }
    return true;
}

bool repvicore::RPVC_Validator::anyTrue(const bool *arr, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (arr[i]) {
            return true;
        }
    }
    return false;
}

bool repvicore::RPVC_Validator::allInRange(const int *arr, size_t length, int min, int max)
{
    for (size_t i = 0; i < length; i++) {
        if (arr[i] < min || arr[i] > max) {
            return false;
        }
    }
    return true;
}

bool repvicore::RPVC_Validator::anyOutOfRange(const int *arr, size_t length, int min, int max)
{
    for (size_t i = 0; i < length; i++) {
        if (arr[i] < min || arr[i] > max) {
            return true;
        }
    }
    return false;
}

bool repvicore::RPVC_Validator::isWithinPercent(int value, int target, float percent)
{
    float lowerBound = target * (1.0f - percent / 100.0f);
    float upperBound = target * (1.0f + percent / 100.0f);
    return (value >= lowerBound && value <= upperBound);
}

bool repvicore::RPVC_Validator::isMonotonicIncreasing(const int *arr, size_t length)
{
    int prev = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] < prev) {
            return false;
        }
        prev = arr[i];
    }
    return true;
}

bool repvicore::RPVC_Validator::isMonotonicDecreasing(const int *arr, size_t length)
{
    int prev = arr[0];
    for (size_t i = 1; i < length; i++) {
        if (arr[i] > prev) {
            return false;
        }
        prev = arr[i];
    }
    return true;
}
