# Cross-Compilation and Libraries

This document summarizes key concepts related to cross-compilation, shared libraries, and remote debugging in embedded Linux development.

---

# What is Cross-Compilation

Cross-compilation means building software for a different CPU architecture than the system used for development.

Example:

- development host: macOS (Apple Silicon)
- target system: Raspberry Pi ARMHF Linux

In this setup:

- source code is edited on the host machine
- binaries are compiled using a cross-toolchain
- executables run on the target device

This approach is commonly used in embedded Linux development because target devices usually have limited CPU power, memory, and storage.

---

# Native vs Cross Compilation

## Native compilation

The executable is built for the same architecture as the host system.

Example:

    gcc main.c -o app

Host:

- x86 Linux

Output:

- x86 executable

The executable can run directly on the same machine.

---

## Cross-compilation

The executable is built for another architecture.

Example:

    armv7-unknown-linux-gnueabihf-gcc main.c -o app

Host:

- macOS

Output:

- ARMHF Linux executable

The executable cannot run directly on the host machine.

---

# Why ARM Binaries Cannot Run on the Host

An ARM executable contains ARM machine instructions.

A macOS host cannot execute ARMHF Linux binaries because:

- the executable targets Linux
- the executable uses ARMHF ABI
- the required Linux runtime loader does not exist on macOS

Typical error:

    zsh: exec format error

or:

    Could not open '/lib/ld-linux-armhf.so.3'

This behavior confirms that the binary was correctly cross-compiled for the target platform.

---

# Shared Libraries

Linux applications commonly use shared libraries (`.so` files).

Examples:

    libc.so.6
    libm.so
    libpthread.so

Shared libraries are loaded dynamically at runtime by the Linux loader.

Advantages:

- smaller executables
- lower memory usage
- shared system libraries
- easier updates

---

# Static vs Dynamic Linking

## Static linking

Library code is copied directly into the executable.

Advantages:

- fewer runtime dependencies
- self-contained executable

Disadvantages:

- larger binaries
- duplicated library code in memory

---

## Dynamic linking

Libraries are loaded at runtime.

Advantages:

- smaller binaries
- shared memory usage
- easier library updates

Disadvantages:

- runtime dependency management
- possible version mismatches

---

# Build-Time, Run-Time, and Debug-Time Libraries

Cross-development environments typically involve multiple copies of system libraries.

---

## Build-Time Libraries

Used by the cross-compiler during linking.

Location example:

    ~/rpi/sysroot/lib/arm-linux-gnueabihf/

These libraries belong to the target sysroot.

---

## Run-Time Libraries

Used when the executable runs on the Raspberry Pi.

Location example:

    /lib/arm-linux-gnueabihf/libc.so.6

These libraries are part of the Raspberry Pi operating system.

---

## Debug-Time Libraries

Used by GDB during remote debugging.

When debugging with `gdbserver`, GDB loads symbol information from target libraries to correctly resolve functions and variables.

---

# glibc Compatibility

Linux systems commonly use GNU libc (glibc).

Example:

    libc.so.6

glibc is generally backwards compatible:

- binaries built with older glibc usually run on newer systems
- binaries built with newer glibc may fail on older systems

Example:

- binary built with glibc 2.31
- runs on glibc 2.36
- may fail on glibc 2.28

---

# Useful Inspection Tools

## file

Shows file type and architecture.

    file app

Example output:

    ELF 32-bit LSB executable, ARM

---

## ldd

Lists runtime shared library dependencies.

    ldd app

---

## readelf

Displays ELF information.

    readelf --headers app
    readelf --symbols app

---

## objdump

Displays binary information and symbols.

    objdump -T app

---

## strings

Extracts printable strings from binaries.

    strings libc.so.6 | grep GNU

Useful for checking glibc versions.

---

# Remote Debugging with gdbserver

In embedded Linux systems, debugging is commonly split into two parts:

- `gdb` running on the host machine
- `gdbserver` running on the target device

Workflow:

1. build executable on host
2. copy executable to Raspberry Pi
3. start `gdbserver` on Raspberry Pi
4. connect using cross-GDB from host

This allows:

- breakpoints
- stepping
- variable inspection
- remote execution control

---

# Example glibc Version Check Program

    #include <stdio.h>
    #include <gnu/libc-version.h>

    int main()
    {
        printf("Compile-time glibc version: %d.%d\n",
               __GLIBC__,
               __GLIBC_MINOR__);

        printf("Runtime glibc version: %s\n",
               gnu_get_libc_version());

        return 0;
    }

This program prints both:

- compile-time glibc version
- runtime glibc version

---

# Key Takeaways

- Cross-compilation allows building software for embedded targets on a more powerful host system.
- Embedded Linux applications rely heavily on shared libraries.
- Multiple library environments may exist in cross-development workflows.
- glibc compatibility is important when distributing binaries.
- Remote debugging with `gdbserver` is standard practice in embedded Linux development.
