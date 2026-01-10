/* ARM Cortex-M CMSIS-based interrupt control */
#include "RPVC_Interrupts.h"
#include <stdint.h>

/* CMSIS Core includes - adjust path based on your CMSIS location */
#if defined(__ARM_ARCH_6M__)
    /* Cortex-M0/M0+ */
    #define __CM0_CMSIS_VERSION_MAIN  5
    #define __CM0_CMSIS_VERSION_SUB   0
    #define __CORTEX_M                0
#elif defined(__ARM_ARCH_7M__)
    /* Cortex-M3 */
    #define __CM3_CMSIS_VERSION_MAIN  5
    #define __CM3_CMSIS_VERSION_SUB   0
    #define __CORTEX_M                3
#elif defined(__ARM_ARCH_7EM__)
    /* Cortex-M4/M7 */
    #define __CM4_CMSIS_VERSION_MAIN  5
    #define __CM4_CMSIS_VERSION_SUB   0
    #define __CORTEX_M                4
#endif

/* Inline assembly helpers for interrupt control */
static inline uint32_t __get_PRIMASK(void) {
    uint32_t result;
    __asm volatile ("MRS %0, primask" : "=r" (result));
    return result;
}

static inline void __set_PRIMASK(uint32_t priMask) {
    __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

static inline void __enable_irq(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

static inline void __disable_irq(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

uint32_t RPVC_Interrupts_Enable(void)
{
    __enable_irq();
    return 0;
}

uint64_t RPVC_Interrupts_Disable(void)
{
    __disable_irq();
    return 0;
}

uint32_t RPVC_Interrupts_SaveState(void)
{
    return __get_PRIMASK();
}

void RPVC_Interrupts_RestoreState(uint32_t state)
{
    __set_PRIMASK(state);
}

uint32_t RPVC_Interrupts_AreEnabled(void)
{
    return (__get_PRIMASK() == 0) ? 1 : 0;
}

uint32_t RPVC_Interrupts_EnterCritical(void)
{
    uint32_t state = __get_PRIMASK();
    __disable_irq();
    return state;
}

void RPVC_Interrupts_ExitCritical(uint32_t state)
{
    __set_PRIMASK(state);
}
