# RPVCConfig.cmake
# Master configuration hub for RepviCore

# ---------------------------------------------------------
# Architecture selection
# ---------------------------------------------------------
set(RPVC_ARCH "x86" CACHE STRING "Target architecture")
# Options: arm, riscv, x86, xtensa

# ---------------------------------------------------------
# OS selection
# ---------------------------------------------------------
set(RPVC_OS "baremetal" CACHE STRING "Target OS")
# Options: freertos, baremetal

# ---------------------------------------------------------
# Feature flags
# ---------------------------------------------------------
option(RPVC_ENABLE_TIME "Enable RPVC_Time subsystem" ON)
option(RPVC_ENABLE_INTERRUPTS "Enable RPVC_Interrupts subsystem" ON)
option(RPVC_ENABLE_ATOMICS "Enable atomic operations" ON)

# ---------------------------------------------------------
# Platform-specific flags
# ---------------------------------------------------------
if(RPVC_ARCH STREQUAL "arm")
    add_definitions(-DRPVC_ARCH_ARM)
elseif(RPVC_ARCH STREQUAL "riscv")
    add_definitions(-DRPVC_ARCH_RISCV)
elseif(RPVC_ARCH STREQUAL "xtensa")
    add_definitions(-DRPVC_ARCH_XTENSA)
elseif(RPVC_ARCH STREQUAL "x86")
    add_definitions(-DRPVC_ARCH_X86)
endif()

# ---------------------------------------------------------
# OS-specific flags
# ---------------------------------------------------------
if(RPVC_OS STREQUAL "freertos")
    add_definitions(-DRPVC_OS_FREERTOS)
elseif(RPVC_OS STREQUAL "baremetal")
    add_definitions(-DRPVC_OS_BAREMETAL)
endif()

# ---------------------------------------------------------
# Export variables to parent CMakeLists
# ---------------------------------------------------------
set(RPVC_CONFIG_LOADED TRUE)
