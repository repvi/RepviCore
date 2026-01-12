/* Xtensa interrupt control */
#include "RPVC_INTERRUPTS.h"
#include <stdint.h>
#include <xtensa/hal.h>
#include <xtensa/xtruntime.h>

/* Xtensa special registers */
#define RSR(reg, val) __asm__ volatile ("rsr %0, " #reg : "=a"(val))
#define WSR(reg, val) __asm__ volatile ("wsr %0, " #reg :: "a"(val))
#define XSR(reg, val) __asm__ volatile ("xsr %0, " #reg : "+a"(val))

uint32_t RPVC_INTERRUPTS_Enable(void)
{
    uint32_t state;
    RSR(PS, state);
    __asm__ volatile ("rsil %0, 0" : "=a"(state));
    return state;
}

uint64_t RPVC_INTERRUPTS_Disable(void)
{
    uint32_t state;
    __asm__ volatile ("rsil %0, 15" : "=a"(state));  /* Disable all levels */
    return (uint64_t)state;
}

uint32_t RPVC_INTERRUPTS_SaveState(void)
{
    uint32_t state;
    RSR(PS, state);
    return state;
}

void RPVC_INTERRUPTS_RestoreState(uint32_t state)
{
    WSR(PS, state);
    __asm__ volatile ("rsync");
}

uint32_t RPVC_INTERRUPTS_AreEnabled(void)
{
    uint32_t ps;
    RSR(PS, ps);
    /* Check INTLEVEL bits (0-3) - if 0, INTERRUPTS are enabled */
    return ((ps & 0x0F) == 0) ? 1 : 0;
}

uint32_t RPVC_INTERRUPTS_EnterCritical(void)
{
    uint32_t state;
    __asm__ volatile ("rsil %0, 15" : "=a"(state));
    return state;
}

void RPVC_INTERRUPTS_ExitCritical(uint32_t state)
{
    WSR(PS, state);
    __asm__ volatile ("rsync");
}
