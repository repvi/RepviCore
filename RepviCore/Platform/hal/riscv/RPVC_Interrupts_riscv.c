/* RISC-V interrupt control using CSR */
#include "RPVC_Interrupts.h"
#include <stdint.h>

/* RISC-V CSR (Control and Status Register) macros */
#define read_csr(reg) ({ unsigned long __tmp; \
    __asm__ volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define write_csr(reg, val) ({ \
    __asm__ volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define set_csr(reg, bit) ({ unsigned long __tmp; \
    __asm__ volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
    __tmp; })

#define clear_csr(reg, bit) ({ unsigned long __tmp; \
    __asm__ volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "rK"(bit)); \
    __tmp; })

/* Machine Status Register (mstatus) bits */
#define MSTATUS_MIE     0x00000008  /* Machine Interrupt Enable */

uint32_t RPVC_Interrupts_Enable(void)
{
    set_csr(mstatus, MSTATUS_MIE);
    return 0;
}

uint64_t RPVC_Interrupts_Disable(void)
{
    clear_csr(mstatus, MSTATUS_MIE);
    return 0;
}

uint32_t RPVC_Interrupts_SaveState(void)
{
    return (uint32_t)(read_csr(mstatus) & MSTATUS_MIE);
}

void RPVC_Interrupts_RestoreState(uint32_t state)
{
    if (state & MSTATUS_MIE) {
        set_csr(mstatus, MSTATUS_MIE);
    } else {
        clear_csr(mstatus, MSTATUS_MIE);
    }
}

uint32_t RPVC_Interrupts_AreEnabled(void)
{
    return (read_csr(mstatus) & MSTATUS_MIE) ? 1 : 0;
}

uint32_t RPVC_Interrupts_EnterCritical(void)
{
    uint32_t state = (uint32_t)(read_csr(mstatus) & MSTATUS_MIE);
    clear_csr(mstatus, MSTATUS_MIE);
    return state;
}

void RPVC_Interrupts_ExitCritical(uint32_t state)
{
    if (state & MSTATUS_MIE) {
        set_csr(mstatus, MSTATUS_MIE);
    }
}
