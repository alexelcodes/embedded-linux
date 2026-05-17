# Embedded Linux

Embedded Linux projects using Raspberry Pi, C, and cross-compilation.

- Linux application development in C
- Raspberry Pi
- cross-compilation
- hardware interfacing
- Linux debugging tools

The repository also includes setup guides and development notes.

---

## Technologies

- C
- Raspberry Pi OS
- libgpiod
- CMake
- GCC cross toolchains
- GDB / gdbserver
- VS Code

---

## Projects

### 1. [Raspberry Pi GPIO Button & LED Controller](./projects/rpi-gpio-libgpiod-button-led)

GPIO button and LED control project using `libgpiod`.

Features:

- button input handling
- LED pulse generation
- internal GPIO pull-up configuration
- GPIO timing measurements
- oscilloscope measurements
- Raspberry Pi cross-compilation workflow

### 2. [Raspberry Pi Hardware PWM Servo Control](./projects/rpi-hardware-pwm-servo-control)

Hardware PWM servo control project using Raspberry Pi PWM peripherals and POSIX timers.

Features:

- hardware PWM using Linux sysfs
- servo motor control
- S-curve motion profile
- PWM timing measurements
- Raspberry Pi cross-compilation workflow

### 3. [Raspberry Pi GPIO Events and Debounce](./projects/rpi-gpio-events-debounce)

GPIO event handling project using `libgpiod`, POSIX threads, and software debounce on Raspberry Pi.

Features:

- GPIO edge event handling
- software debounce
- POSIX threads
- event-driven input processing
- polling vs interrupt-style GPIO handling

### 4. [Raspberry Pi Multi-purpose Knob Controller](./projects/rpi-multipurpose-knob-controller)

Multi-purpose Raspberry Pi controller using a rotary encoder, WS2812 RGB LED, hardware PWM, and POSIX threads.

Features:

- rotary encoder input handling
- WS2812 RGB LED control using SPI
- servo motor control using hardware PWM
- mode switching
- GPIO control using `libgpiod`
- multithreading
- polling-based GPIO input handling

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
