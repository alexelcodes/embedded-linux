# C Compilation Process

This document gives a short overview of how C source code is transformed into a runnable executable on Linux systems.

---

# Compilation Pipeline

The process from C source code to a running application typically consists of four stages:

1. preprocessing
2. compilation
3. linking
4. loading

---

# 1. Preprocessor

The preprocessor handles directives before compilation begins.

Main tasks:

- expanding macros (`#define`)
- inserting header files (`#include`)
- conditional compilation (`#ifdef`, `#ifndef`)

The output of this stage is called a translation unit.

Example:

```c
#include <stdio.h>

#define VALUE 10
```

After preprocessing, the contents of included headers and macros are expanded into a larger intermediate source file.

---

# 2. Compiler

The compiler translates C source code into machine code for the target architecture.

The output of this stage is an object file.

Object files:

- contain machine instructions
- contain symbol/debug information
- are not executable yet
- are compiled independently

Example object file extensions:

- `.o` on Linux
- `.obj` on Windows

---

# 3. Linker

The linker combines object files and libraries into a final executable binary.

The linker resolves:

- function references
- global symbols
- library dependencies

The final output is typically:

- an executable ELF binary on Linux
- a PE executable on Windows

The linker may use:

- static libraries (`.a`)
- shared libraries (`.so`)

---

# 4. Loader

The loader is responsible for starting the executable at runtime.

Main tasks:

- loading the executable into memory
- loading required shared libraries
- resolving dynamic symbols
- transferring execution to the application entry point

In Linux systems, shared libraries help reduce memory usage because multiple applications can use the same library instance simultaneously.

Bare-metal systems usually do not have a loader because there is no operating system.

---

# GCC

The most common C compiler toolchain on Linux systems is GCC.

GCC automatically performs preprocessing, compilation, and linking unless explicitly instructed otherwise.

Basic example:

```bash
gcc main.c -o app
```

This command:

- preprocesses the source
- compiles the source
- links the executable
- produces an executable named `app`

---

# Native Compilation

Native compilation means:

- building code on a system
- for the same system architecture

Example:

- building x86-64 Linux binaries on an x86-64 Linux machine

---

# Useful Commands

Compile application:

```bash
gcc main.c -o app
```

Compile with debug symbols:

```bash
gcc -g main.c -o app
```

Check executable architecture:

```bash
file app
```

Run application:

```bash
./app
```

---

# VS Code Build Configuration

VS Code can automate building and debugging using tasks and launch configurations.

## Example `tasks.json`

`.vscode/tasks.json`

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build",
      "type": "shell",
      "command": "gcc",
      "args": ["-g", "${file}", "-o", "${fileBasenameNoExtension}"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

---

# VS Code Debug Configuration

## Example `launch.json`

`.vscode/launch.json`

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Build and debug active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}/${fileBasenameNoExtension}",
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb",
      "preLaunchTask": "build",
      "miDebuggerPath": "/usr/bin/gdb",
      "stopAtEntry": false
    }
  ]
}
```

---

# Typical VS Code Workflow

1. Open a project folder
2. Create a C source file
3. Configure build tasks
4. Configure debugger launch settings
5. Build using:

```text
Shift + Ctrl/Cmd + B
```

6. Start debugging using:

```text
F5
```

Breakpoints can then be used for interactive debugging with GDB.
