# Embedded Linux

Embedded Linux and cross-compilation projects using Raspberry Pi and C.

- Linux application development in C
- Raspberry Pi
- cross-compilation
- hardware interfacing
- debugging and low-level Linux workflows

The repository includes setup guides and notes for embedded Linux development.

---

## Technologies

- C
- Raspberry Pi OS
- libgpiod
- CMake
- GCC cross toolchains
- GDB / gdbserver
- VS Code
- Linux userspace GPIO

---

## Projects

### 1. [Raspberry Pi GPIO Button & LED Controller](./projects/rpi-gpio-libgpiod-button-led)

GPIO button and LED control project using `libgpiod`.

Features:

- button input handling
- LED pulse generation
- internal GPIO pull-up configuration
- timing experiments with Linux userspace GPIO
- oscilloscope measurements
- Raspberry Pi cross-compilation workflow

### 2. [Raspberry Pi Hardware PWM Servo Control](./projects/rpi-hardware-pwm-servo-control)

Hardware PWM servo control project using Raspberry Pi PWM peripherals and POSIX timers.

Features:

- hardware PWM using Linux sysfs
- servo motor control
- S-curve motion profile
- PWM timing experiments
- Linux timing and jitter analysis
- Raspberry Pi cross-compilation workflow

### 3. [Raspberry Pi GPIO Events and Debounce](./projects/rpi-gpio-events-debounce)

GPIO event handling project using `libgpiod`, POSIX threads, and software debounce on Raspberry Pi.

Features:

- GPIO edge event handling
- software debounce
- POSIX threads
- event-driven input processing
- polling vs interrupt-style GPIO handling
- Linux userspace GPIO experiments

---

## Documentation

### Cross-compilation setup

- [macOS Raspberry Pi Cross-Compile Setup](./docs/macos-rpi-cross-toolchain-setup.md)

### Linux and build system notes

- [C Compilation Process](./docs/c-compilation-process.md)
- [Cross-Compilation and Libraries](./docs/cross-compilation-libraries.md)
- [Platform Selection](./docs/platform-selection.md)
- [VS Code C/C++ Workflow](./docs/vscode-cpp-workflow.md)

---

## Repository Structure

```text
.
├── .vscode/
├── cmake/
├── docs/
└── projects/
```
