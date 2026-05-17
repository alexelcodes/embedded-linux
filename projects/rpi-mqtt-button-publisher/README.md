# Raspberry Pi MQTT Button Publisher

MQTT button event publisher for Raspberry Pi using `libgpiod`, Eclipse Paho MQTT, and C.

The application monitors a GPIO button and publishes MQTT messages when the button state changes.

---

## Features

- MQTT publish messaging
- GPIO button input handling
- internal GPIO pull-up configuration
- button press and release detection
- MQTT broker communication
- Eclipse Paho MQTT client library
- Raspberry Pi cross-compilation workflow

---

## Hardware

- Raspberry Pi running Raspberry Pi OS
- push button
- breadboard and jumper wires

---

## GPIO Mapping

| Function | GPIO |
| -------- | ---- |
| Button   | 22   |

---

## MQTT Configuration

Default MQTT settings:

| Parameter | Value          |
| --------- | -------------- |
| Broker    | 172.27.228.195 |
| Port      | 1883           |
| Topic     | test           |

Published messages:

```text
button_pressed
button_released
```

---

## Dependencies

The project requires:

- libgpiod
- Eclipse Paho MQTT C library

The Paho MQTT library must be built and installed in the Raspberry Pi sysroot before building this project.

Additional setup notes:

- [Development Notes](docs/development-notes.md)

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
./rpi_mqtt_button_publisher
```

Example output:

```text
Sent: 1
Sent: 0
Sent: 1
```

---

## How It Works

The application:

1. initializes GPIO input using `libgpiod`
2. configures internal pull-up resistor
3. connects to the MQTT broker
4. monitors button state changes
5. publishes MQTT messages on state transitions

Button logic:

- released button = logical `0`
- pressed button = logical `1`

The application only publishes messages when the GPIO state changes.

---

## MQTT Communication

The Raspberry Pi application acts as an MQTT publisher.

When the button state changes:

- button press → `button_pressed`
- button release → `button_released`

Messages are sent to the configured MQTT topic through the Paho MQTT client library.

---

## Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── docs
│   └── development-notes.md
└── src
    ├── main.c
    ├── mqtt.c
    └── mqtt.h
```
