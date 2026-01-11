cmake_minimum_required(VERSION 3.15)

project(RepviCore 
    VERSION 1.0.0
    LANGUAGES C CXX
)

# C/C++ Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(repvicore STATIC
    # Main
    RepviCore/repvicore.c
    
    # Core
    RepviCore/Core/core_api.cpp
        
    # Validation
    RepviCore/Core/Validation/src/RPVC_Validator_api.cpp
    RepviCore/Core/Validation/src/RPVC_Validator.cpp
    
    # Events
    RepviCore/Events/src/event_api.cpp
    RepviCore/Events/src/EventManager.cpp
    
    # Subsystems
    RepviCore/Subsystems/src/logging_impl.cpp
    RepviCore/Subsystems/src/messaging_impl.cpp
    
    # Platform Common (internal)
    RepviCore/Platform/src/RPVC_Time_common.c
)

# Include directories
target_include_directories(repvicore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/CompileTime
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core/TimeSystem
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core/Validation/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Events/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Subsystems/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Platform/inc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core/Validation/src
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Events/src
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Subsystems/src
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Platform/src
)

set(RPVC_TARGET repvicore CACHE INTERNAL "RepviCore target")

#add_library(${RPVC_TARGET} STATIC) 

include(cmake/RPVCConfig.cmake)

if(NOT RPVC_CONFIG_LOADED)
    message(FATAL_ERROR "RPVCConfig.cmake not loaded!")
endif()

# Select HAL implementation
if(RPVC_ARCH STREQUAL "arm")
    add_subdirectory(RepviCore/Platform/hal/arm)
elseif(RPVC_ARCH STREQUAL "riscv")
    add_subdirectory(RepviCore/Platform/hal/riscv)
elseif(RPVC_ARCH STREQUAL "xtensa")
    add_subdirectory(RepviCore/Platform/hal/xtensa)
elseif(RPVC_ARCH STREQUAL "x86")
    add_subdirectory(RepviCore/Platform/hal/x86)
endif()

# Select OSAL implementation
if(RPVC_OS STREQUAL "baremetal")
    if(RPVC_ARCH STREQUAL "arm")
        add_subdirectory(RepviCore/Platform/osal/baremetal/arm)
    elseif(RPVC_ARCH STREQUAL "riscv")
        add_subdirectory(RepviCore/Platform/osal/baremetal/riscv)
    elseif(RPVC_ARCH STREQUAL "xtensa")
        add_subdirectory(RepviCore/Platform/osal/baremetal/xtensa)
    elseif(RPVC_ARCH STREQUAL "x86")
        add_subdirectory(RepviCore/Platform/osal/baremetal/x86)
    else()
        message(FATAL_ERROR "Unsupported RPVC_ARCH for baremetal OSAL: ${RPVC_ARCH}")
    endif()
elseif(RPVC_OS STREQUAL "freertos")
    add_subdirectory(RepviCore/Platform/osal/freertos)
else()
    message(FATAL_ERROR "Unsupported RPVC_OS: ${RPVC_OS}")
endif()

message(STATUS "Building RepviCore for architecture: ${RPVC_ARCH}")
message(STATUS "Building RepviCore for OS: ${RPVC_OS}")

# Architecture-specific compiler options
if(RPVC_ARCH STREQUAL "arm")
    if(NOT MSVC)
        target_compile_options(repvicore PRIVATE -mthumb -mcpu=cortex-m3)
    endif()
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(repvicore PRIVATE /W4)
else()
    target_compile_options(repvicore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Test executable (optional)
option(BUILD_TESTS "Build test executable" ON)
if(RPVC_ARCH STREQUAL "arm" OR RPVC_ARCH STREQUAL "riscv" OR RPVC_ARCH STREQUAL "xtensa")
    set(BUILD_TESTS OFF CACHE BOOL "Disable host tests for cross-compilation" FORCE)
    message(STATUS "Cross-target detected; disabling host tests")
endif()

if(BUILD_TESTS)
    add_executable(repvicore_test
        Test/main.cpp
    )
    
    target_link_libraries(repvicore_test PRIVATE repvicore)
endif()
