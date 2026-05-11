# Raspberry Pi GPIO Button & LED Controller

Simple Raspberry Pi GPIO project using `libgpiod` and C.

The application measures button press duration and generates a matching LED pulse train output. The project was created while experimenting with Linux GPIO userspace access, timing behavior, and Raspberry Pi cross-compilation workflows.

---

## Features

- GPIO control using `libgpiod`
- LED output control
- Button input handling
- Internal GPIO pull-up configuration
- Button press duration measurement
- LED pulse train generation
- Log file output
- Cross-compilation support for Raspberry Pi

---

## Hardware

- Raspberry Pi running Raspberry Pi OS (Bookworm 32-bit)
- LED
- Push button
- 220 Ω resistor
- Breadboard and jumper wires
- Oscilloscope (optional)

---

## GPIO Mapping

| Function | GPIO   |
| -------- | ------ |
| LED      | GPIO23 |
| Button   | GPIO22 |

---

## Hardware Setup

### Example setup

![Hardware setup](images/example-setup.jpeg)

### Raspberry Pi pin locations

![GPIO mapping](images/pin-locations.png)

---

## Build Requirements

Install required packages on Raspberry Pi:

```bash
sudo apt update

sudo apt install -y \
    gpiod \
    libgpiod-dev \
    gdbserver \
    build-essential
```

---

## Build

```bash
mkdir -p build
cd build

cmake ..
cmake --build .
```

---

## Run

```bash
./rpi_gpio_button_led
```

The application creates a log file:

```text
button.log
```

Example:

```text
press=700 ms, pulses=7
press=300 ms, pulses=3
```

---

## How It Works

- The button input uses a pull-up resistor configuration
- Released button state = logical `1`
- Pressed button state = logical `0`

The application:

1. waits for a button press
2. measures button press duration
3. stores the measurement into a log file
4. generates an LED pulse train

Pulse train format:

- 10 ms HIGH
- 90 ms LOW

One pulse represents approximately 100 ms of button press duration.

Example:

- 730 ms button press
- approximately 7 LED pulses

---

## Additional Documentation

- [GPIO pull-up configuration](docs/gpio-pullup.md)
- [Hardware setup and timing measurements](docs/hardware-setup.md)

---

## Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── docs
│   ├── gpio-pullup.md
│   └── hardware-setup.md
├── images
│   ├── example-setup.jpeg
│   ├── pin-locations.png
│   └── two-ways-to-wire-switch.jpg
└── src
    └── button-led.c
```

---

## Notes

This project uses the Linux GPIO character device interface through `libgpiod`.

The older sysfs GPIO interface is deprecated in modern Linux kernels.
