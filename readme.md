Build presets and cross-compile examples

Use CMake presets to configure for different architectures.

Configure & build for host x86 (default preset):
```bash
cmake --preset x86-debug
cmake --build build/x86-debug
```

Configure for ARM using the sample toolchain file (edit paths in cmake/toolchains/arm-none-eabi.cmake):
```bash
cmake --preset arm-debug
cmake --build build/arm-debug
```

Configure for RISC-V using the sample toolchain (edit paths in cmake/toolchains/riscv-none-embed.cmake):
```bash
cmake --preset riscv-debug
cmake --build build/riscv-debug
```

If you prefer manual configure:
```bash
cmake -S . -B build -G Ninja -DRPVC_ARCH=arm -DRPVC_OS=baremetal -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi.cmake
cmake --build build
```

Notes:
- The toolchain files are templates. Adjust compiler prefixes and paths to match your toolchain installation.
- You can add additional presets to `CMakePresets.json` for xtensa, windows mingw, etc.
