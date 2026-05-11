# Raspberry Pi GPIO Hardware Setup

This project uses a simple Raspberry Pi GPIO setup with:

- one LED output
- one push-button input
- optional oscilloscope measurements using PicoScope

---

## Required Components

- Raspberry Pi
- Breadboard
- LED
- 220 Ω resistor
- Push button
- Jumper wires
- PicoScope oscilloscope (optional)

---

## GPIO Connections

| Component | GPIO   |
| --------- | ------ |
| LED       | GPIO23 |
| Button    | GPIO22 |

---

## LED Circuit

Connect:

- GPIO23 → resistor → LED → GND

The resistor limits current through the LED and protects both the LED and GPIO pin.

---

## Button Circuit

Connect:

- GPIO22 → button → GND

The project uses the Raspberry Pi internal pull-up resistor, so no external pull-up resistor is required.

Button logic:

| State    | GPIO Value |
| -------- | ---------- |
| Released | HIGH (1)   |
| Pressed  | LOW (0)    |

---

## Pin Reference

Raspberry Pi GPIO numbering:

![GPIO pin map](../images/pin-locations.png)

---

## Example Hardware Setup

![Example setup](../images/example-setup.jpeg)

---

## Oscilloscope Measurements

The GPIO output signal can be monitored using PicoScope.

Recommended measurements:

- pulse width
- frequency
- cycle time
- timing jitter

---

## Important Safety Notes

Never connect oscilloscope ground directly to random GPIO pins.

Always connect oscilloscope ground to Raspberry Pi GND.

Incorrect grounding may short GPIO pins and damage the board.

---

## Timing Experiments

The project can be used to observe Linux userspace GPIO timing behavior.

Interesting tests include:

- adding `usleep()` delays
- removing delays
- enabling/disabling `printf()`
- enabling `fflush(stdout)`

These experiments demonstrate:

- scheduler jitter
- buffering effects
- Linux userspace timing limitations
- impact of console I/O on timing behavior

---

## Observations

GPIO timing generated from a normal Linux userspace application is not precise.

Operations such as:

- `printf()`
- terminal output
- file flushing
- context switching

can significantly affect timing accuracy and waveform stability.
