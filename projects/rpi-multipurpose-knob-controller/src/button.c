#include "button.h"
#include "controller.h"

#include <gpiod.h>
#include <unistd.h>

#define CHIPNAME "gpiochip0"
#define BUTTON_GPIO 24

static struct gpiod_chip *chip;
static struct gpiod_line *line;

void button_start(void)
{
    chip = gpiod_chip_open_by_name(CHIPNAME);
    line = gpiod_chip_get_line(chip, BUTTON_GPIO);

    // Enable internal pull-up so the button reads HIGH when released and LOW when pressed
    gpiod_line_request_input_flags(
        line,
        "button",
        GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);

    int last = 1;

    // Poll the button state continuously
    while (1)
    {
        int val = gpiod_line_get_value(line);

        // Detect button press (transition from HIGH to LOW)
        if (val == 0 && last == 1)
        {
            controller_handle_mode_button();
        }

        last = val;

        // Small delay to reduce CPU usage
        usleep(10000);
    }
}