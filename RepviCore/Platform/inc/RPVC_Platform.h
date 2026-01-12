#ifndef RPVC_PLATFORM_H
#define RPVC_PLATFORM_H

#include "core/core_types.h"
#include "compile_time.h"

typedef enum RPVC_PlatformCapabilities_e {
    /* x86/x64 SIMD capabilities */
    RPVC_CAP_SIMD_MMX     = 1 << 0,
    RPVC_CAP_SIMD_SSE     = 1 << 1,
    RPVC_CAP_SIMD_SSE2    = 1 << 2,
    RPVC_CAP_SIMD_SSE3    = 1 << 3,
    RPVC_CAP_SIMD_SSSE3   = 1 << 4,
    RPVC_CAP_SIMD_SSE41   = 1 << 5,
    RPVC_CAP_SIMD_SSE42   = 1 << 6,
    RPVC_CAP_SIMD_AVX     = 1 << 7,
    RPVC_CAP_SIMD_AVX2    = 1 << 8,
    RPVC_CAP_SIMD_AVX512  = 1 << 9,

    /* ARM SIMD capabilities */
    RPVC_CAP_SIMD_NEON    = 1 << 10,
    RPVC_CAP_SIMD_SVE     = 1 << 11,
    RPVC_CAP_ARM_DSP      = 1 << 12,
    RPVC_CAP_ARM_MVE      = 1 << 13,  /* Helium */

    /* RISC-V extensions */
    RPVC_CAP_RISCV_M      = 1 << 14,  /* Multiply/Divide */
    RPVC_CAP_RISCV_A      = 1 << 15,  /* Atomic */
    RPVC_CAP_RISCV_F      = 1 << 16,  /* Single-precision FP */
    RPVC_CAP_RISCV_D      = 1 << 17,  /* Double-precision FP */
    RPVC_CAP_RISCV_C      = 1 << 18,  /* Compressed */
    RPVC_CAP_RISCV_V      = 1 << 19,  /* Vector */

    /* Xtensa capabilities */
    RPVC_CAP_XTENSA_MAC16 = 1 << 20,
    RPVC_CAP_XTENSA_MUL32 = 1 << 21,
    RPVC_CAP_XTENSA_DIV32 = 1 << 22,

    /* Common hardware features */
    RPVC_CAP_HW_FPU       = 1 << 23,
    RPVC_CAP_HW_MMU       = 1 << 24,
    RPVC_CAP_HW_DCACHE    = 1 << 25,
    RPVC_CAP_HW_ICACHE    = 1 << 26,
} RPVC_PlatformCapabilities_t;

RPVC_EXTERN_C_BEGIN

RPVC_Status_t RPVC_PLATFORM_Init(void);

RPVC_Status_t RPVC_PLATFORM_GetName(const char **name);

RPVC_PlatformCapabilities_t RPVC_PLATFORM_GetCapabilities(void);

void RPVC_PLATFORM_Yield(void);

void RPVC_PLATFORM_Idle(void);

RPVC_EXTERN_C_END

#endif // RPVC_PLATFORM_H