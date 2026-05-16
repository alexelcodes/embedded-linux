# Software PWM vs Hardware PWM on Raspberry Pi

This project originally started with software-generated PWM using Linux POSIX timers.

The software PWM approach worked, but oscilloscope measurements showed significant timing jitter caused by normal Linux userspace execution.

Because servo motors depend on stable pulse timing, the project was later migrated to Raspberry Pi hardware PWM.

---

## What Is PWM

PWM (Pulse Width Modulation) is a signal generation technique where the pulse width controls device behavior.

Servo motors typically expect:

- 50 Hz PWM frequency
- 20 ms signal period
- pulse width between approximately 1 ms and 2 ms

Example:

| Pulse Width | Servo Position  |
| ----------- | --------------- |
| 1.0 ms      | minimum angle   |
| 1.5 ms      | center position |
| 2.0 ms      | maximum angle   |

---

## Software PWM

Software PWM generates pulses directly from application code.

In this project, the original implementation used:

- POSIX timers
- GPIO toggling from userspace
- Linux signal handlers

Typical software PWM flow:

1. start periodic timer
2. set GPIO HIGH
3. create second timer for pulse end
4. set GPIO LOW

---

## Problems With Software PWM

Linux is not a hard real-time operating system.

Userspace applications compete with:

- the Linux scheduler
- background processes
- interrupts
- cache activity
- console I/O

Because of this, GPIO timing is not perfectly deterministic.

Oscilloscope measurements showed:

- pulse jitter
- unstable pulse widths
- timing drift
- inconsistent servo behavior

Additional operations such as:

- `printf()`
- file I/O
- terminal output

made timing even less stable.

---

## What Is Jitter

Jitter is the deviation between the expected timing and the actual timing of a signal.

Example:

Expected pulse:

```text
1.500 ms
```

Actual pulses:

```text
1.472 ms
1.531 ms
1.488 ms
```

Even small variations can affect servo smoothness and generate unwanted vibration or noise.

---

## Hardware PWM

Hardware PWM uses dedicated Raspberry Pi PWM peripherals instead of generating pulses from application code.

The Linux kernel PWM driver continuously generates PWM signals directly in hardware.

The userspace application only updates configuration values such as:

- period
- duty cycle

Advantages:

- very low jitter
- stable pulse timing
- reduced CPU usage
- smoother servo movement
- better reliability

---

## Linux PWM Sysfs Interface

The project controls PWM through:

```text
/sys/class/pwm/pwmchip0/
```

Example operations:

```bash
echo 0 > /sys/class/pwm/pwmchip0/export

echo 20000000 > /sys/class/pwm/pwmchip0/pwm0/period

echo 1500000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle

echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable
```

The application performs the same operations programmatically using C file I/O.

---

## S-Curve Motion Profile

The project updates PWM duty cycle values using an S-curve lookup table.

Compared to linear movement, S-curve motion:

- reduces abrupt acceleration changes
- reduces mechanical stress
- produces smoother movement
- reduces servo vibration and noise

The servo continuously moves forward and backward through the lookup table values.

---

## Conclusion

Software PWM is useful for learning and experimentation, but Linux userspace timing limitations make it unsuitable for accurate servo control.

Hardware PWM provides significantly more stable timing and produces smoother and more reliable servo operation.

This project demonstrates both approaches and highlights the importance of hardware-assisted timing in embedded Linux systems.
