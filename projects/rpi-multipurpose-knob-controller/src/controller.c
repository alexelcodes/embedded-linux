#include "controller.h"
#include "ws2812_led.h"
#include "mode_led.h"
#include "servo.h"
#include <stdint.h>
#include <unistd.h>

/*
 *
 * In LED mode, the rotary encoder controls the color of a WS2812 RGB LED
 * using a simple rainbow algorithm.
 */

// Operating modes
typedef enum
{
    MODE_LED = 0,
    MODE_SERVO
} mode_t;

static mode_t mode = MODE_LED;

// Position in the color cycle (0..255)
static int color_pos = 0;

// Servo position (0..100%)
static int servo_level = 50;

/*
 * Convert position (0..255) into RGB color.
 *
 * Color order (clockwise):
 * RED -> YELLOW -> GREEN -> CYAN -> BLUE -> MAGENTA -> RED
 *
 * The range is split into three sections:
 * 0..84   : red -> green
 * 85..169 : green -> blue
 * 170..255: blue -> red
 */
static void apply_ws2812_rainbow(int pos)
{
    uint8_t r, g, b;

    if (pos < 0)
        pos = 0;
    if (pos > 255)
        pos = 255;

    if (pos < 85)
    {
        // red -> green
        r = 255 - pos * 3;
        g = pos * 3;
        b = 0;
    }
    else if (pos < 170)
    {
        pos -= 85;

        // green -> blue
        r = 0;
        g = 255 - pos * 3;
        b = pos * 3;
    }
    else
    {
        pos -= 170;

        // blue -> red
        r = pos * 3;
        g = 0;
        b = 255 - pos * 3;
    }

    ws2812_set_color(r, g, b);

    // Pause required for WS2812 timing
    usleep(1000);
}

void controller_init(void)
{
    // Init mode LED (OFF = LED mode)
    mode_led_init();
    mode_led_set(0);

    // Init servo and WS2812
    servo_init();
    ws2812_init();

    // Apply initial values
    apply_ws2812_rainbow(color_pos);
    servo_set(servo_level);
}

void controller_handle_mode_button(void)
{
    // Toggle between LED and servo control
    if (mode == MODE_LED)
    {
        mode = MODE_SERVO;
        mode_led_set(1); // LED ON
    }
    else
    {
        mode = MODE_LED;
        mode_led_set(0); // LED OFF
    }
}

void controller_handle_up(void)
{
    if (mode == MODE_LED)
    {
        // Move forward in color cycle
        if (color_pos < 255)
            color_pos++;

        apply_ws2812_rainbow(color_pos);
    }
    else
    {
        // Move servo in one direction
        if (servo_level > 0)
            servo_level--;

        servo_set(servo_level);
    }
}

void controller_handle_down(void)
{
    if (mode == MODE_LED)
    {
        // Move backward in color cycle
        if (color_pos > 0)
            color_pos--;

        apply_ws2812_rainbow(color_pos);
    }
    else
    {
        // Move servo in the opposite direction
        if (servo_level < 100)
            servo_level++;

        servo_set(servo_level);
    }
}
