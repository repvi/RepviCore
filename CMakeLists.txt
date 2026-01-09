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
    
    # Time System
    RepviCore/Core/TimeSystem/RPVC_TIME.c
    
    # Validation
    RepviCore/Core/Validation/src/RPVC_Validator_api.cpp
    RepviCore/Core/Validation/src/RPVC_Validator.cpp
    
    # Events
    RepviCore/Events/src/event_api.cpp
    RepviCore/Events/src/EventManager.cpp
    
    # Subsystems
    RepviCore/Subsystems/src/logging_impl.cpp
    RepviCore/Subsystems/src/messaging_impl.cpp
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
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Core/Validation/src
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Events/src
        ${CMAKE_CURRENT_SOURCE_DIR}/RepviCore/Subsystems/src
)

# Compiler warnings
if(MSVC)
    target_compile_options(repvicore PRIVATE /W4)
else()
    target_compile_options(repvicore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Test executable (optional)
option(BUILD_TESTS "Build test executable" ON)
if(BUILD_TESTS)
    add_executable(repvicore_test
        Test/main.cpp
    )
    
    target_link_libraries(repvicore_test PRIVATE repvicore)
endif()
