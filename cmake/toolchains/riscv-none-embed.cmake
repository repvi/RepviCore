# Minimal example RISC-V toolchain file for CMake
# Edit the paths below to match your riscv-none-embed installation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

set(RISCV_PREFIX "C:/msys64/mingw64/bin/riscv64-unknown-elf-")
set(CMAKE_C_COMPILER   ${RISCV_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${RISCV_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${RISCV_PREFIX}gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
