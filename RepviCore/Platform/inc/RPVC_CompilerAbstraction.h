#ifndef RPVC_COMPILER_ABSTRACTION_H
#define RPVC_COMPILER_ABSTRACTION_H

/*
 * RepviCore - Compiler Abstraction Layer
 *
 * This header centralizes all compiler-specific attributes and keywords.
 * Core code MUST use only the RPVC_* macros defined here, never raw
 * compiler attributes. This ensures portability across:
 *
 *   - GCC / Clang
 *   - MSVC
 *   - IAR / Keil / Green Hills / TI / others (with graceful fallbacks)
 */

/* --------------------------------------------------------------------------
 *  Compiler detection
 * -------------------------------------------------------------------------- */

#if defined(__clang__)
    #define RPVC_COMPILER_CLANG 1
#elif defined(__GNUC__) && !defined(__CC_ARM)
    #define RPVC_COMPILER_GCC   1
#elif defined(_MSC_VER)
    #define RPVC_COMPILER_MSVC  1
#elif defined(__ICCARM__) || defined(__IAR_SYSTEMS_ICC__)
    #define RPVC_COMPILER_IAR   1
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #define RPVC_COMPILER_KEIL  1
#elif defined(__ghs__)
    #define RPVC_COMPILER_GHS   1
#elif defined(__TI_COMPILER_VERSION__)
    #define RPVC_COMPILER_TI    1
#else
    #define RPVC_COMPILER_UNKNOWN 1
#endif

/* --------------------------------------------------------------------------
 *  Inline control
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #define RPVC_INLINE         inline
    #define RPVC_ALWAYS_INLINE  inline __attribute__((always_inline))
    #define RPVC_NOINLINE       __attribute__((noinline))
#elif defined(RPVC_COMPILER_MSVC)
    #define RPVC_INLINE         __inline
    #define RPVC_ALWAYS_INLINE  __forceinline
    #define RPVC_NOINLINE       __declspec(noinline)
#else
    /* Safe fallbacks */
    #define RPVC_INLINE         inline
    #define RPVC_ALWAYS_INLINE  inline
    #define RPVC_NOINLINE
#endif

/* --------------------------------------------------------------------------
 *  Function attributes
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #define RPVC_NORETURN       __attribute__((noreturn))
    #define RPVC_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(RPVC_COMPILER_MSVC)
    #define RPVC_NORETURN       __declspec(noreturn)
    #define RPVC_DEPRECATED(msg) __declspec(deprecated(msg))
#else
    #define RPVC_NORETURN
    #define RPVC_DEPRECATED(msg)
#endif

/* --------------------------------------------------------------------------
 *  Variable / type attributes
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #define RPVC_PACKED         __attribute__((packed))
    #define RPVC_ALIGN(x)       __attribute__((aligned(x)))
    #define RPVC_UNUSED         __attribute__((unused))
#elif defined(RPVC_COMPILER_MSVC)
    #define RPVC_PACKED
    #define RPVC_ALIGN(x)       __declspec(align(x))
    #define RPVC_UNUSED
#else
    #define RPVC_PACKED
    #define RPVC_ALIGN(x)
    #define RPVC_UNUSED
#endif

/* --------------------------------------------------------------------------
 *  Branch prediction hints (optional, no-op on many compilers)
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #define RPVC_LIKELY(x)      __builtin_expect(!!(x), 1)
    #define RPVC_UNLIKELY(x)    __builtin_expect(!!(x), 0)
#else
    #define RPVC_LIKELY(x)      (x)
    #define RPVC_UNLIKELY(x)    (x)
#endif

/* --------------------------------------------------------------------------
 *  Fallthrough annotation (for switch statements)
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #if __has_attribute(fallthrough)
        #define RPVC_FALLTHROUGH __attribute__((fallthrough))
    #else
        #define RPVC_FALLTHROUGH /* fallthrough */
    #endif
#else
    #define RPVC_FALLTHROUGH    /* fallthrough */
#endif

/* --------------------------------------------------------------------------
 *  Restrict keyword
 * -------------------------------------------------------------------------- */

#if defined(RPVC_COMPILER_GCC) || defined(RPVC_COMPILER_CLANG)
    #define RPVC_RESTRICT       __restrict__
#elif defined(RPVC_COMPILER_MSVC)
    #define RPVC_RESTRICT       __restrict
#else
    #define RPVC_RESTRICT
#endif

#endif /* RPVC_COMPILER_ABSTRACTION_H */
