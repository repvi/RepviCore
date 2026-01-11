/* RISC-V system control */
#include "RPVC_System.h"
#include "RPVC_Interrupts.h"
#include <stdint.h>

/* RISC-V CSR access macros */
#define read_csr(reg) ({ unsigned long __tmp; \
    __asm__ volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define write_csr(reg, val) ({ \
    __asm__ volatile ("csrw " #reg ", %0" :: "rK"(val)); })

static uint8_t s_systemInitialized = 0;

RPVC_Status_t RPVC_System_Init(void)
{
    if (s_systemInitialized) {
        return RPVC_OK;
    }
    
    /* RISC-V system initialization */
    s_systemInitialized = 1;
    return RPVC_OK;
}

RPVC_Status_t RPVC_System_Deinit(void)
{
    s_systemInitialized = 0;
    return RPVC_OK;
}

void RPVC_System_Idle(void)
{
    __asm__ volatile ("wfi");
}

void RPVC_System_Reset(void)
{
    /* Platform-specific reset mechanism */
    /* Most RISC-V implementations require writing to a vendor-specific register */
    while(1) {
        __asm__ volatile ("nop");
    }
}

RPVC_Status_t RPVC_System_GetCPUName(const char **cpuName)
{
    if (cpuName == NULL) {
        return RPVC_ERR_INVALID_ARG;
    }
#if __riscv_xlen == 32
    #if defined(__riscv_mul) && defined(__riscv_atomic) && defined(__riscv_flen)
        *cpuName = "RV32IMAF";
    #elif defined(__riscv_mul) && defined(__riscv_atomic)
        *cpuName = "RV32IMA";
    #elif defined(__riscv_mul)
        *cpuName = "RV32IM";
    #else
        *cpuName = "RV32I";
    #endif
#elif __riscv_xlen == 64
    #if defined(__riscv_mul) && defined(__riscv_atomic) && defined(__riscv_flen)
        *cpuName = "RV64IMAF";
    #elif defined(__riscv_mul) && defined(__riscv_atomic)
        *cpuName = "RV64IMA";
    #elif defined(__riscv_mul)
        *cpuName = "RV64IM";
    #else
        *cpuName = "RV64I";
    #endif
#else
    *cpuName = "RISC-V";
#endif
    return RPVC_OK;
}

uint32_t RPVC_System_GetCPUID(void)
{
    /* Read mhartid (hardware thread ID) */
    return (uint32_t)read_csr(mhartid);
}

uint32_t RPVC_System_GetCycleCount(void)
{
#if __riscv_xlen == 32
    /* For RV32, need to read cycleh and cycle carefully for overflow */
    uint32_t high, low;
    do {
        high = (uint32_t)read_csr(mcycleh);
        low = (uint32_t)read_csr(mcycle);
    } while (high != (uint32_t)read_csr(mcycleh));
    return low; /* Return lower 32 bits */
#else
    return (uint32_t)read_csr(mcycle);
#endif
}
