# Raspberry Pi GPIO Events and Debounce

GPIO event handling project using `libgpiod`, POSIX threads, and C on Raspberry Pi.

The project detects button events using GPIO edge detection and filters switch bouncing with a simple software debounce mechanism.

---

## Features

- GPIO event detection using `libgpiod`
- rising and falling edge handling
- button input handling
- LED output control
- POSIX thread for GPIO event handling
- software debounce

---

## Hardware

- Raspberry Pi running Raspberry Pi OS
- push button
- red LED
- yellow LED
- green LED
- resistors
- breadboard and jumper wires
- oscilloscope (optional)

---

## GPIO Mapping

| Function   | GPIO |
| ---------- | ---- |
| Button     | 22   |
| Red LED    | 23   |
| Yellow LED | 24   |
| Green LED  | 25   |

---

## Hardware Setup

![Example circuit](images/example-circuit.png)

---

## Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## Run

```bash
./rpi_gpio_events_debounce
```

Detected button presses are printed to the terminal.

---

## How It Works

The button GPIO line is configured to detect both edges:

- falling edge = button press
- rising edge = button release

A separate POSIX thread waits for GPIO events using `gpiod_line_event_wait()`.

The debounce logic ignores repeated edges that occur inside a short debounce time window.

---

## Why Debounce Is Needed

Mechanical buttons do not produce a clean single transition.

When pressed or released, the signal may rapidly switch between HIGH and LOW several times before settling.

Without debounce, one physical button press may be detected as multiple presses.

---

## Additional Documentation

- [Polling vs GPIO Events](docs/polling-vs-events.md)

---

## Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── docs
│   └── polling-vs-events.md
├── images
│   └── example-circuit.png
└── src
    └── gpio-events-debounce.c
```
