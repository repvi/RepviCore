#ifndef RPVC_INTERRUPTS_H
#define RPVC_INTERRUPTS_H

#include "core/core_types.h"
#include "compile_time.h"

RPVC_EXTERN_C_BEGIN

uint32_t RPVC_Interrupts_Enable(void);
uint64_t RPVC_Interrupts_Disable(void); 

uint32_t RPVC_Interrupts_SaveState(void);
void RPVC_Interrupts_RestoreState(uint32_t state);
uint32_t RPVC_Interrupts_AreEnabled(void);

uint32_t RPVC_Interrupts_EnterCritical(void); 
void RPVC_Interrupts_ExitCritical(uint32_t state);

RPVC_EXTERN_C_END

#endif // RPVC_INTERRUPTS_H