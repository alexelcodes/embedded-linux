#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LED_LINE 23
#define BTN_LINE 22
#define CONSUMER "rpi-gpio-button-led"
#define LOG_FILE "button.log"

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line *led;
    struct gpiod_line *btn;

    chip = gpiod_chip_open_by_name("gpiochip0");
    led = gpiod_chip_get_line(chip, LED_LINE);
    btn = gpiod_chip_get_line(chip, BTN_LINE);

    // enable internal pull-up for button
    system("gpioget --bias=pull-up gpiochip0 22");

    gpiod_line_request_output(led, CONSUMER, 0);
    gpiod_line_request_input(btn, CONSUMER);

    FILE *f = fopen(LOG_FILE, "a");

    // button wiring: pull-up -> released = 1, pressed = 0
    while (1)
    {
        // wait for press
        while (gpiod_line_get_value(btn) == 1)
            usleep(1000);

        // measure press length in 100 ms steps
        int count100 = 0;
        while (gpiod_line_get_value(btn) == 0)
        {
            usleep(100000); // 100 ms
            count100++;
        }

        // log press length
        if (f)
        {
            fprintf(f, "press=%d ms, pulses=%d\n", count100 * 100, count100);
            fflush(f);
        }

        // output pulse train
        for (int i = 0; i < count100; i++)
        {
            gpiod_line_set_value(led, 1);
            usleep(10000); // 10 ms high
            gpiod_line_set_value(led, 0);
            usleep(90000); // 90 ms low
        }
    }

    return 0;
}
