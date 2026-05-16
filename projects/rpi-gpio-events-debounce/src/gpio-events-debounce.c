#define _POSIX_C_SOURCE 200809L

#include <gpiod.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define CONSUMER "gpio-events"

#define BUTTON_PIN 22
#define RED_PIN 23
#define YELLOW_PIN 24
#define GREEN_PIN 25

#define DEBOUNCE_NS 5000000L

typedef struct
{
    struct timespec timeout;
    struct gpiod_line *line;
} gpio_event_context_t;

static long time_difference_ns(
    struct timespec current,
    struct timespec previous)
{
    long seconds = current.tv_sec - previous.tv_sec;
    long nanoseconds = current.tv_nsec - previous.tv_nsec;

    return seconds * 1000000000L + nanoseconds;
}

static void *gpio_event_thread(void *arguments)
{
    gpio_event_context_t *context = arguments;
    struct gpiod_line_event event;

    static struct timespec last_event_time = {0, 0};
    static int button_pressed = 0;
    static int press_counter = 0;

    printf("GPIO event thread started\n");

    while (1)
    {
        int result = gpiod_line_event_wait(
            context->line,
            &context->timeout);

        if (result < 0)
        {
            continue;
        }

        if (result == 0)
        {
            printf("*");
            fflush(stdout);
            continue;
        }

        if (gpiod_line_event_read(context->line, &event) < 0)
        {
            continue;
        }

        if (last_event_time.tv_sec != 0 &&
            time_difference_ns(event.ts, last_event_time) < DEBOUNCE_NS)
        {
            continue;
        }

        usleep(DEBOUNCE_NS / 1000);

        if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
        {
            if (button_pressed)
            {
                continue;
            }

            if (gpiod_line_get_value(context->line) != 0)
            {
                continue;
            }

            button_pressed = 1;
            last_event_time = event.ts;

            printf(
                "\nbutton press #%d timestamp: %8ld.%09ld\n",
                press_counter,
                (long)event.ts.tv_sec,
                (long)event.ts.tv_nsec);

            press_counter++;
        }
        else if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE)
        {
            if (gpiod_line_get_value(context->line) == 0)
            {
                continue;
            }

            button_pressed = 0;
            last_event_time = event.ts;
        }
    }
}

int main(void)
{
    pthread_t gpio_thread;

    struct gpiod_chip *chip;
    struct gpiod_line *button_line;
    struct gpiod_line *red_line;
    struct gpiod_line *yellow_line;
    struct gpiod_line *green_line;

    chip = gpiod_chip_open_by_name("gpiochip0");

    if (chip == NULL)
    {
        perror("Failed to open gpiochip0");
        return 1;
    }

    button_line = gpiod_chip_get_line(chip, BUTTON_PIN);

    if (button_line == NULL)
    {
        perror("Failed to get button GPIO line");
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_both_edges_events(button_line, CONSUMER) < 0)
    {
        perror("Failed to request GPIO events");
        gpiod_chip_close(chip);
        return 1;
    }

    red_line = gpiod_chip_get_line(chip, RED_PIN);
    yellow_line = gpiod_chip_get_line(chip, YELLOW_PIN);
    green_line = gpiod_chip_get_line(chip, GREEN_PIN);

    if (red_line == NULL ||
        yellow_line == NULL ||
        green_line == NULL)
    {
        perror("Failed to get LED GPIO lines");
        gpiod_line_release(button_line);
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(red_line, CONSUMER, 0) < 0 ||
        gpiod_line_request_output(yellow_line, CONSUMER, 0) < 0 ||
        gpiod_line_request_output(green_line, CONSUMER, 0) < 0)
    {
        perror("Failed to configure LED GPIO lines");
        gpiod_line_release(button_line);
        gpiod_chip_close(chip);
        return 1;
    }

    gpio_event_context_t context = {
        .timeout = {5, 0},
        .line = button_line};

    if (pthread_create(
            &gpio_thread,
            NULL,
            gpio_event_thread,
            &context) != 0)
    {
        perror("Failed to create GPIO thread");

        gpiod_line_release(red_line);
        gpiod_line_release(yellow_line);
        gpiod_line_release(green_line);
        gpiod_line_release(button_line);
        gpiod_chip_close(chip);

        return 1;
    }

    int counter = 0;

    printf("Entering main loop\n");

    while (1)
    {
        gpiod_line_set_value(red_line, (counter & 1) != 0);
        gpiod_line_set_value(yellow_line, (counter & 2) != 0);
        gpiod_line_set_value(green_line, (counter & 4) != 0);

        usleep(200000);

        counter++;
    }

    return 0;
}
