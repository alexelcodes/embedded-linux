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

---

## Documentation

### Cross-compilation setup

- `docs/macos-rpi-cross-toolchain-setup.md`

### Linux and build system notes

- `docs/c-compilation-process.md`
- `docs/cross-compilation-libraries.md`
- `docs/platform-selection.md`
- `docs/vscode-cpp-workflow.md`

---

## Repository Structure

```text
.
├── .vscode/
├── cmake/
├── docs/
└── projects/
```
