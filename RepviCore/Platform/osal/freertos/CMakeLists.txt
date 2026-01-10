set(PLATFORM_OSAL_SOURCES
    RPVC_OS_freertos.c
)

target_sources(RepviCore PRIVATE ${PLATFORM_OSAL_SOURCES})
