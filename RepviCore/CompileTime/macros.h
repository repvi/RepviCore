#ifndef COMPILE_TIME_MACROS_H
#define COMPILE_TIME_MACROS_H

// C/C++ linkage macros
#ifdef __cplusplus
    #define RPVC_EXTERN_C_BEGIN extern "C" {
    #define RPVC_EXTERN_C_END   }
#else
    #define RPVC_EXTERN_C_BEGIN
    #define RPVC_EXTERN_C_END
#endif

#endif // COMPILE_TIME_MACROS_H