# macOS Raspberry Pi Cross-Compile Setup

This guide shows how to set up a macOS (Apple Silicon) environment for
cross-compiling and remotely debugging Raspberry Pi (ARMHF) Linux applications
using CMake, Ninja, GCC and gdbserver.

---

## Goal

- Build **ARMHF (32‑bit) Linux binaries** on macOS
- Run and **debug on Raspberry Pi via** gdbserver

---

## Install Raspberry Pi OS (Bookworm 32-bit)

### Set:

• hostname: rpi  
• username: pi  
• enable SSH  
• set password

## SSH setup

### Remove old host keys (after OS reinstall):

```bash
ssh-keygen -R rpi.local
ssh-keygen -R rpi
```

### Enable passwordless login:

```bash
ssh-copy-id pi@rpi.local
```

### Add SSH shortcut:

~/.ssh/config

```ssh
Host rpi
    HostName rpi.local
    User pi
```

### Connect:

```bash
ssh rpi
```

---

## Prepare Raspberry Pi

### Update system

```bash
sudo apt update
sudo apt upgrade -y
sudo reboot
```

---

### Install required packages

```bash
sudo apt install -y \
  gdbserver \
  rsync \
  build-essential \
  libc6-dev
```

### Why:

- `gdbserver` → remote debugging on Raspberry Pi (used by VS Code/GDB)
- `rsync` → synchronize Raspberry Pi libraries/headers to macOS sysroot
- `libc6-dev` → C runtime headers + crt objects required for cross-linking
- `build-essential` → basic compiler tools (gcc, make); useful for native tests

### Verify:

```bash
dpkg --print-architecture
getconf LONG_BIT
which gdbserver
ls /lib/ld-linux-armhf.so.3
ls /usr/lib/arm-linux-gnueabihf/crt1.o
ls /usr/include/stdio.h
```

---

## Install tools on macOS (Homebrew)

### Toolchain

```bash
brew tap messense/macos-cross-toolchains
brew install armv7-unknown-linux-gnueabihf
```

### Build tools

```bash
brew install cmake ninja rsync pkg-config
```

### Why:

- `armv7-unknown-linux-gnueabihf` → ARM cross-compiler toolchain (gcc/g++/gdb for Raspberry Pi)
- `cmake` → generate build system files from CMake projects
- `ninja` → fast build executor used to compile targets
- `rsync` → synchronize Raspberry Pi libraries/headers to macOS sysroot
- `pkg-config` → provides correct compiler/linker flags for external libraries.

### Verify:

```bash
armv7-unknown-linux-gnueabihf-gcc --version
armv7-unknown-linux-gnueabihf-gdb --version
```

---

## Create sysroot on macOS

### Create directory

```bash
mkdir -p ~/rpi/sysroot
```

---

### Sync required files from Raspberry Pi

#### C/C++ headers (needed for compilation: #include <...>)

```bash
rsync -a --delete --rsync-path="sudo rsync" \
  rpi:/usr/include/ \
  ~/rpi/sysroot/usr/include/
```

#### User-space libraries + start files (crt\*.o) (needed for linking)

```bash
rsync -a --delete --rsync-path="sudo rsync" \
  rpi:/usr/lib/arm-linux-gnueabihf/ \
  ~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
```

#### Core runtime libraries (libc, libm, etc.) (needed for linking + runtime deps)

```bash
rsync -a --delete --rsync-path="sudo rsync" \
  rpi:/lib/arm-linux-gnueabihf/ \
  ~/rpi/sysroot/lib/arm-linux-gnueabihf/
```

#### Dynamic loader (interpreter) (needed so the linker/debugger can resolve /lib/ld-linux-armhf.so.3)

```bash
rsync -a --delete --rsync-path="sudo rsync" \
  rpi:/lib/ld-linux-armhf.so.3 \
  ~/rpi/sysroot/lib/
```

---

### Validate sysroot

```bash
# headers
ls ~/rpi/sysroot/usr/include/arm-linux-gnueabihf/bits/libc-header-start.h

# crt/link
ls ~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/crt1.o

# runtime libs
ls ~/rpi/sysroot/lib/arm-linux-gnueabihf/libc.so.6

# loader
ls ~/rpi/sysroot/lib/ld-linux-armhf.so.3
```

Expected sysroot size: **\~1.5–2.5 GB**

---

## CMake toolchain file

`cmake/toolchain-armhf-macos.cmake`

```cmake
# Target platform: Linux ARM
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Root filesystem with target headers and libraries
set(CMAKE_SYSROOT "$ENV{HOME}/rpi/sysroot")

# ARM cross compiler binaries
set(TOOLCHAIN armv7-unknown-linux-gnueabihf)
set(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)

# Linker search paths for crt*.o and glibc start files
set(_LIB1 "${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf")
set(_LIB2 "${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf")
set(_RPATH_LINK "-Wl,-rpath-link,${_LIB1} -Wl,-rpath-link,${_LIB2}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-B${_LIB1} -B${_LIB2} ${_RPATH_LINK}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-B${_LIB1} -B${_LIB2} ${_RPATH_LINK}")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-B${_LIB1} -B${_LIB2} ${_RPATH_LINK}")

# Multiarch glibc headers location
set(_INC "${CMAKE_SYSROOT}/usr/include/arm-linux-gnueabihf")

# Use sysroot and correct include paths during compilation
set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT} -isystem ${_INC}")
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT} -isystem ${_INC}")

# Restrict library/header/package lookup to sysroot
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

---

## VS Code CMake Kit

`.vscode/cmake-kits.json`

```json
[
  {
    "name": "RPi ARMHF (macOS)",
    "toolchainFile": "${workspaceFolder}/cmake/toolchain-armhf-macos.cmake"
  }
]
```

---

## VS Code task configuration

`.vscode/tasks.json`

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Copy built to RPi (scp)",
      "type": "shell",
      "command": "scp",
      "args": ["${command:cmake.launchTargetPath}", "rpi:~"],
      "problemMatcher": []
    },
    {
      "label": "Run on RPi (ssh)",
      "type": "shell",
      "command": "ssh",
      "args": ["rpi", "\"~/${command:cmake.launchTargetFilename}\""],
      "problemMatcher": [],
      "dependsOn": "Copy built to RPi (scp)"
    }
  ]
}
```

---

## VS Code launch configurations

`.vscode/launch.json`

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug on RPi (gdbserver)",
      "type": "cppdbg",
      "request": "launch",
      "program": "${command:cmake.launchTargetPath}",
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb",
      "miDebuggerPath": "/opt/homebrew/bin/armv7-unknown-linux-gnueabihf-gdb",
      "preLaunchTask": "Copy built to RPi (scp)",
      "miDebuggerServerAddress": "| ssh -T rpi gdbserver - '~/${command:cmake.launchTargetFilename}'",
      "stopAtEntry": false
    },

    {
      "name": "Run (native)",
      "type": "cppdbg",
      "request": "launch",
      "program": "${command:cmake.launchTargetPath}",
      "cwd": "${workspaceFolder}",
      "MIMode": "lldb",
      "stopAtEntry": false
    }
  ]
}
```

---

## Validation

### On macOS (verify binary architecture)

```bash
file build/<target_name>
```

Expected:

```
ELF 32-bit LSB executable, ARM, EABI5, dynamically linked
```

Confirms that the binary was cross-compiled for ARM Linux (Raspberry Pi).

---

### Run on Raspberry Pi (without debugger)

#### Run directly using VS Code task (`.vscode/tasks.json`):

Shift + Cmd + P → Tasks: Run Task → Run on RPi (ssh)

#### Steps:

- copy binary to Raspberry Pi via scp
- execute binary via ssh

#### Manual equivalent:

```bash
scp build/<target_name> rpi:~
ssh rpi ./<target_name>
```

---

### Debug on Raspberry Pi (gdbserver)

#### Start remote debugging from VS Code:

- Open Run and Debug
- Select Debug on RPi (gdbserver) (from `.vscode/launch.json`)

#### Steps:

- build project (CMake)
- copy binary to Raspberry Pi
- start gdbserver on device
- attach cross-GDB from macOS

Allows breakpoints, stepping, and variable inspection directly from VS Code.

### Native execution and debugging on macOS

The same VS Code configuration can also be used for native execution and
debugging directly on macOS using LLDB.

Select:

Run and Debug → Run (native)

This is useful for testing host-side utilities before deploying to Raspberry Pi.

## Final architecture

```
macOS
 ├─ Homebrew ARM cross toolchain (gcc/g++/ld + gdb)
 │    - gcc/g++ compile ARMHF binaries
 │    - ld links against the sysroot
 │    - gdb connects to gdbserver for debugging
 │
 ├─ Sysroot (rsync from RPi)
 │    - exact headers + libs from the target OS
 │    - ensures “build matches target” (glibc, loader, libgpiod, etc.)
 │
 ├─ CMake + Ninja
 │    - CMake generates build files using the toolchain file
 │    - Ninja executes the build
 │
 └── SSH → gdbserver → Raspberry Pi
      - SSH runs/copies binaries and starts gdbserver remotely
      - gdbserver runs the program on the Pi and waits for the debugger
      - ARM cross-GDB on macOS connects to gdbserver and controls execution
```

## References

Toolchains: [messense/homebrew-macos-cross-toolchains](https://github.com/messense/homebrew-macos-cross-toolchains)
