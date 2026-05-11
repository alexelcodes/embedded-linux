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
