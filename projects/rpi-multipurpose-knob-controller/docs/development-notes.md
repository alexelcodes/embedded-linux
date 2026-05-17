# Development Notes

Simple multi-purpose controller using a Raspberry Pi.  
The device has two operating modes: **LED control** and **servo control**.
A **KY-040 rotary encoder** is used to control the device and the encoder push button switches between the modes.

## Hardware connections

- GPIO17 – rotary encoder CLK
- GPIO27 – rotary encoder DT
- GPIO24 – encoder push button (mode select)
- GPIO23 – mode indicator LED
- GPIO18 – PWM servo control
- GPIO10 – SPI MOSI → WS2812 DIN

## Operating modes

### LED control mode

- The rotary encoder changes the LED color
- Mode LED is **OFF**

### Servo control mode

- The rotary encoder controls the servo position
- WS2812 LED color remains unchanged
- Mode LED is **ON**

## Implementation notes

- Hardware PWM is used for the servo
- The encoder push button uses an internal pull-up configured in the software using `libgpiod`
- The button and rotary encoder are read using polling in separate threads
- The controller module stores the current state of:
  - operating mode
  - WS2812 color position
  - servo position

### Enabling hardware PWM

Hardware PWM was enabled by editing the Raspberry Pi configuration file:

```text
/boot/firmware/config.txt
```

and adding:

```text
dtoverlay=pwm-2chan
```

This enables PWM channels used for servo control.

### First start after reboot

After rebooting the Raspberry Pi, the program may fail if it is started without `sudo`.

This happens because the program configures hardware PWM using the `/sys/class/pwm` interface in the `servo.c` module.

After the PWM channels are exported and configured, the program can usually be started without `sudo`.

### Useful GPIO commands

Show all GPIO pin states:

```bash
pinctrl
```

Check specific GPIO pins:

```bash
pinctrl get 18
pinctrl get 19
```

For quick testing, an internal pull-up can also be enabled manually:

```bash
sudo pinctrl set <GPIO_NUMBER> ip pu
```

The application itself configures the encoder button pull-up using `libgpiod`.

## Source files

```text
src
├── main.c
├── controller.c
├── button.c
├── knob.c
├── servo.c
├── ws2812_led.c
└── mode_led.c
```

**main.c**  
Initializes the controller and starts two threads for reading the button and the rotary encoder.

**controller.c**  
Maintains the current operating mode, WS2812 color position and servo position. Handles events from the button and rotary encoder.

**button.c**  
Reads the encoder push button using GPIO and triggers mode switching.

**knob.c**  
Reads the rotary encoder (CLK and DT signals) and detects the rotation direction to generate up/down events.

**servo.c**  
Controls the servo position using hardware PWM.

**mode_led.c**  
Controls the indicator LED that shows the current operating mode.

**ws2812_led.c**  
Implements the SPI-based driver for the WS2812 RGB LED.

## WS2812 RGB LED

### Hardware changes

A **WS2812D RGB LED** is controlled through the Raspberry Pi SPI interface.

Connection:

- GPIO10 – SPI MOSI → DIN (data input)
- 5V – LED power
- GND – common ground

SPI interface was enabled on Raspberry Pi:

```bash
sudo raspi-config
```

Navigate to:

```text
Interface Options → SPI → Enable
```

Verification:

```bash
pinctrl get 10
```

Expected:

```text
10: a0    pd | lo // GPIO10 = SPI0_MOSI
```

## Driver implementation

WS2812 does not use a standard protocol like SPI or I2C.  
Instead, it requires **precise timing-based signaling**, where:

- bit `0` → short HIGH, long LOW
- bit `1` → long HIGH, short LOW

Timing (from datasheet):

- Total bit time ≈ **1.25 µs**
- RESET time > **280 µs**

---

### Approach

The signal was generated using **SPI (MOSI only)** by encoding each WS2812 bit into a full SPI byte.

Key idea:

- Use SPI clock to approximate timing
- Use different bit patterns to represent `0` and `1`

---

### Final working parameters

```c
#define SPI_SPEED 6500000U

#define LED_ZERO 0x80 // 1000 0000
#define LED_ONE  0xF8 // 1111 1000
```

Explanation:

- SPI speed (~6.5 MHz) approximates WS2812 timing
- One encoded SPI byte is close to the WS2812 bit timing
- `LED_ZERO` → short HIGH pulse
- `LED_ONE` → long HIGH pulse

These values were obtained experimentally based on datasheet timing and testing on real hardware.

---

### Reset behavior

WS2812 requires a reset (low signal) before sending new data.

In practice, a short delay between color updates was sufficient to provide stable operation.

---

## New LED control logic

Instead of brightness, the rotary encoder controls the LED color.

A simple rainbow algorithm is used:

- 0–85 → red → green
- 86–170 → green → blue
- 171–255 → blue → red

Resulting color transition:

```text
RED → YELLOW → GREEN → CYAN → BLUE → MAGENTA → RED
```

---

## Result

- Encoder changes LED color
- Button switches LED / servo mode
- Mode LED indicates active mode
