/* x86/x64 interrupt control */
#include "RPVC_Interrupts.h"
#include <stdint.h>

#if defined(_MSC_VER)
    #include <intrin.h>
    #define cli() __asm__ cli
    #define sti() __asm__ sti
    #define pushfd_popfd(val) __asm__ { pushfd; pop val }
    #define push_popfd(val) __asm__ { push val; popfd }
#elif defined(__GNUC__) || defined(__clang__)
    static inline void cli(void) 
    {
        __asm__ volatile("cli" ::: "memory");
    }
    
    static inline void sti(void) 
    {
        __asm__ volatile("sti" ::: "memory");
    }
    
    static inline uint32_t pushfd_popfd(void) 
    {
        uint32_t flags;
#if defined(__x86_64__)
        uint64_t flags64;
        __asm__ volatile (
            "pushfq\n\t"
            "popq %0"
            : "=r"(flags64)
            :
            : "memory"
        );

        flags = (uint32_t)flags64;
#else
        __asm__ volatile (
            "pushfl\n\t"
            "popl %0"
            : "=r"(flags)
            :
            : "memory"
        );
#endif
        return flags;
    }
    
    static inline void push_popfd(uint32_t flags) 
    {
#if defined(__x86_64__)
        uint64_t flags64 = (uint64_t)flags;
        __asm__ volatile(
            "pushq %0\n\t"
            "popfq"
            :: "r"(flags64)
            : "memory", "cc"
        );
#else
        __asm__ volatile (
            "pushl %0\n\t"
            "popfl"
            :
            : "r"(flags)
            : "memory", "cc"
        );
#endif
    }
#endif

/* x86 EFLAGS register IF (Interrupt Enable Flag) bit */
#define X86_EFLAGS_IF   0x00000200

uint32_t RPVC_Interrupts_Enable(void)
{
    sti();
    return 0;
}

uint64_t RPVC_Interrupts_Disable(void)
{
    cli();
    return 0;
}

uint32_t RPVC_Interrupts_SaveState(void)
{
#if defined(_MSC_VER)
    uint32_t flags;
    pushfd_popfd(flags);
    return flags;
#else
    return pushfd_popfd();
#endif
}

void RPVC_Interrupts_RestoreState(uint32_t state)
{
    push_popfd(state);
}

uint32_t RPVC_Interrupts_AreEnabled(void)
{
    uint32_t flags = RPVC_Interrupts_SaveState();
    return (flags & X86_EFLAGS_IF) ? 1 : 0;
}

uint32_t RPVC_Interrupts_EnterCritical(void)
{
    uint32_t state = RPVC_Interrupts_SaveState();
    cli();
    return state;
}

void RPVC_Interrupts_ExitCritical(uint32_t state)
{
    push_popfd(state);
}
