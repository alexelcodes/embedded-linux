#include "mode_led.h"

#include <gpiod.h>

#define CHIPNAME "gpiochip0"
#define MODE_LED_GPIO 23

static struct gpiod_chip *chip;
static struct gpiod_line *line;

void mode_led_init(void)
{
    // Open GPIO chip and configure mode indicator LED as output
    chip = gpiod_chip_open_by_name(CHIPNAME);
    line = gpiod_chip_get_line(chip, MODE_LED_GPIO);

    gpiod_line_request_output(line, "mode_led", 0);
}

void mode_led_set(int value)
{
    // Set mode LED state (0 = OFF, 1 = ON)
    gpiod_line_set_value(line, value);
}