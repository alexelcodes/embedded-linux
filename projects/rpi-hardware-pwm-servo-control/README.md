# Raspberry Pi Hardware PWM Servo Control

Hardware PWM servo control project for Raspberry Pi using C and the Linux PWM sysfs interface.

The project uses Raspberry Pi hardware PWM together with POSIX timers to generate smooth servo movement using an S-curve motion profile.

---

## Features

- hardware PWM on Raspberry Pi
- servo motor control
- POSIX timers
- Linux PWM sysfs interface
- S-curve motion profile

---

## Hardware

- Raspberry Pi
- servo motor
- oscilloscope (optional)

---

## PWM Configuration

The project uses Raspberry Pi hardware PWM through:

```text
/sys/class/pwm/pwmchip0/
```

PWM output pin:

| Function | GPIO |
| -------- | ---- |
| PWM OUT  | 18   |

Typical servo PWM settings:

| Parameter | Value |
| --------- | ----- |
| Frequency | 50 Hz |
| Period    | 20 ms |

Duty cycle range:

| Position | Pulse Width |
| -------- | ----------- |
| Min      | ~0.4 ms     |
| Center   | ~1.3 ms     |
| Max      | ~2.2 ms     |

---

## Enable Hardware PWM

Edit Raspberry Pi boot configuration:

```bash
sudo nano /boot/config.txt
```

Add:

```text
dtoverlay=pwm
```

Reboot:

```bash
sudo reboot
```

Verify:

```bash
lsmod | grep pwm
```

Expected:

```text
pwm_bcm2835
```

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
./rpi_hardware_pwm_servo_control
```

The application continuously updates PWM duty cycle values using an S-curve lookup table.

---

## How It Works

The application:

1. initializes Raspberry Pi hardware PWM
2. configures a 20 ms PWM period
3. creates a periodic POSIX timer
4. updates PWM duty cycle every timer tick
5. moves the servo using an S-curve motion profile

The motion profile is implemented using a lookup table with forward and reverse movement.

---

## S-Curve Motion Profile

The servo motion uses an S-curve profile to reduce abrupt acceleration changes.

Benefits:

- smoother movement
- reduced mechanical stress
- less vibration
- less servo noise

---

## Software PWM vs Hardware PWM

Software-generated PWM from Linux userspace introduces timing jitter because the Linux scheduler interrupts normal application execution.

Hardware PWM avoids this problem because PWM pulses are generated directly by Raspberry Pi hardware.

This makes hardware PWM more stable and suitable for servo control.

Additional notes:

- [Software PWM vs Hardware PWM](docs/software-vs-hardware-pwm.md)

---

## Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── docs
│   └── software-vs-hardware-pwm.md
└── src
    ├── main.c
    ├── pwm_hw.c
    └── pwm_hw.h
```
