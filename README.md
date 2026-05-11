# Embedded Linux Projects

A collection of practical embedded Linux projects focused on Linux application development, cross-compilation, hardware interfacing, and low-level system behavior using C and Raspberry Pi.

This repository combines:

- standalone embedded Linux projects
- reusable cross-compilation infrastructure
- development environment setup guides
- low-level Linux and toolchain documentation
- practical hardware interaction experiments

The projects are based on hands-on development and reorganized into standalone portfolio-style implementations instead of raw coursework labs.

---

## Main Focus Areas

- Embedded Linux application development
- Cross-compilation workflows
- Raspberry Pi development
- Linux GPIO programming
- Userspace hardware control
- Remote debugging with GDB/gdbserver
- Toolchains and sysroots
- Timing and scheduling behavior under Linux
- CMake-based embedded workflows

---

## Development Environment

Current workflow:

- macOS (Apple Silicon)
- Raspberry Pi OS Bookworm 32-bit
- ARMHF cross-compilation
- Homebrew cross toolchains
- CMake + Ninja
- VS Code
- GDB + gdbserver

The repository includes reusable setup documentation and toolchain configuration for Raspberry Pi cross-development.

---

## Repository Structure

```text
.
├── cmake/
│   └── shared toolchain files
│
├── docs/
│   └── development environment and Linux notes
│
└── projects/
    └── standalone embedded Linux projects
```

---

## Projects

### 1. [Raspberry Pi GPIO Button & LED Controller](./projects/rpi-gpio-libgpiod-button-led)

A Raspberry Pi GPIO project using `libgpiod` and C.

Features:

- GPIO button input
- LED output control
- internal pull-up configuration
- button press duration measurement
- pulse train generation
- Linux userspace timing experiments
- oscilloscope measurements
- cross-compilation support

Main topics explored:

- Linux GPIO character device interface
- `libgpiod`
- timing jitter
- scheduler effects
- buffering and `printf()` timing impact
- embedded Linux debugging workflow

---

## Documentation

### Cross-compilation and toolchains

- `docs/macos-rpi-cross-toolchain-setup.md`
- `cmake/toolchain-armhf-macos.cmake`

### Linux build/runtime concepts

- `docs/c-compilation-process.md`
- `docs/cross-compilation-libraries.md`

### VS Code workflow

- `docs/vscode-cpp-workflow.md`

### Platform notes

- `docs/platform-selection.md`

---

## Goals of This Repository

This repository is intended to grow into a collection of practical embedded Linux projects covering topics such as:

- GPIO
- PWM
- timers
- servos
- MQTT
- networking
- UART / SPI / I2C
- multithreading
- event-driven programming
- Linux services/daemons
- IPC mechanisms
- real-time behavior analysis

---

## Notes

Most projects are designed to remain:

- standalone
- reproducible
- hardware-focused
- cross-platform development friendly
- easy to build and debug

The repository intentionally focuses on understanding the full embedded Linux workflow instead of only producing final applications.
