# Minimal example ARM toolchain file for CMake
# Edit the paths below to match your arm-none-eabi installation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Cross compiler executables
set(ARM_PREFIX "C:/msys64/mingw64/bin/arm-none-eabi-")
set(CMAKE_C_COMPILER   "${ARM_PREFIX}gcc.exe")
set(CMAKE_CXX_COMPILER "${ARM_PREFIX}g++.exe")
set(CMAKE_ASM_COMPILER "${ARM_PREFIX}gcc.exe")

# Skip linking test executables during CMake try_compile (baremetal toolchains
# often lack host syscalls / runtime, which causes the link step to fail).
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Adjust as needed
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
