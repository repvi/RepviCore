#ifndef RPVC_VALIDATOR_H
#define RPVC_VALIDATOR_H

#include <stdbool.h>
#include "compile_time.h"
#include <cstddef>

namespace repvicore {
    class RPVC_Validator {
    public:
        bool isTrue(bool value);
        bool isFalse(bool value);
        bool isEqual(bool a, bool b);
        bool isNotEqual(bool a, bool b);
        
        bool isInRange(int value, int min, int max);
        bool isOutOfRange(int value, int min, int max);
        bool isGreater(int value, int threshold);
        bool isGreaterOrEqual(int value, int threshold);
        bool isLess(int value, int threshold);
        bool isLessOrEqual(int value, int threshold);
        bool isZero(int value);
        bool isNonZero(int value);

        bool isEven(int value);
        bool isOdd(int value);
        bool isPositive(int value);
        bool isNegative(int value);
        bool isPowerOfTwo(int value);
        bool isPrime(int value);

        bool isInRange(float value, float min, float max);
        bool isFinite(float value);
        bool isNaN(float value);
        bool isApproximately(float a, float b, float epsilon);

        bool clamp(int value, int min, int max);
        bool clamp(float value, float min, float max);
        bool ormalize01(float value, float min, float max);

        bool arraysEqual(const int* a, const int* b, size_t length);
        bool arraysNotEqual(const int* a, const int* b, size_t length);
        bool arrayAllZero(const int* arr, size_t length);
        bool arrayAnyZero(const int* arr, size_t length);
        bool arrayAllPositive(const int* arr, size_t length);
        bool arrayAllNegative(const int* arr, size_t length);
        bool arrayIsSortedAscending(const int* arr, size_t length);
        bool arrayIsSortedDescending(const int* arr, size_t length);

        bool isNull(const void* ptr);
        bool isNotNull(const void* ptr);

        bool stringEqual(const char* a, const char* b, size_t length);
        bool stringNotEqual(const char* a, const char* b, size_t length);
        bool stringLengthInRange(const char* s, size_t min, size_t max, size_t length);
        bool stringIsEmpty(const char* s);
        bool stringNotEmpty(const char* s);

        bool allTrue(const bool* arr, size_t length);
        bool anyTrue(const bool* arr, size_t length);
        bool allInRange(const int* arr, size_t length, int min, int max);
        bool anyOutOfRange(const int* arr, size_t length, int min, int max);

        bool isWithinPercent(int value, int target, float percent);
        bool isMonotonicIncreasing(const int* arr, size_t length);
        bool isMonotonicDecreasing(const int* arr, size_t length);
    };
};

#endif // RPVC_VALIDATOR_H