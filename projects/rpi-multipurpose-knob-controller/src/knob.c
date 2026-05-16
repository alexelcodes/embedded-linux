#include "knob.h"
#include "controller.h"

#include <gpiod.h>
#include <unistd.h>

#define CHIPNAME "gpiochip0"

#define CLK_GPIO 17
#define DT_GPIO 27

static struct gpiod_chip *chip;
static struct gpiod_line *clk;
static struct gpiod_line *dt;

void knob_start(void)
{
    chip = gpiod_chip_open_by_name(CHIPNAME);

    clk = gpiod_chip_get_line(chip, CLK_GPIO);
    dt = gpiod_chip_get_line(chip, DT_GPIO);

    gpiod_line_request_input(clk, "encoder");
    gpiod_line_request_input(dt, "encoder");

    int last_clk = gpiod_line_get_value(clk);

    // Poll rotary encoder signals continuously
    while (1)
    {
        int clk_val = gpiod_line_get_value(clk);

        // Detect change on CLK to determine rotation
        if (clk_val != last_clk)
        {
            int dt_val = gpiod_line_get_value(dt);

            // Determine rotation direction
            if (dt_val != clk_val)
                controller_handle_up();
            else
                controller_handle_down();
        }

        last_clk = clk_val;

        // Small delay to reduce CPU usage
        usleep(1000);
    }
}