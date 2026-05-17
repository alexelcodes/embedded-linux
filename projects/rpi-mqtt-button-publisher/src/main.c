#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>
#include "mqtt.h"

#define ADDRESS "tcp://172.27.228.195:1883"
#define TOPIC "test"
#define CHIP "gpiochip0"
#define LINE 22

int main(void)
{
    struct gpiod_chip *chip = gpiod_chip_open_by_name(CHIP);
    struct gpiod_line *line = gpiod_chip_get_line(chip, LINE);

    struct gpiod_line_request_config config = {
        .consumer = "rpi-mqtt-button-publisher",
        .request_type = GPIOD_LINE_REQUEST_DIRECTION_INPUT,
        .flags = GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP};

    MQTTClient client = mqtt_init(ADDRESS);

    gpiod_line_request(line, &config, 0);

    int last = -1;

    while (1)
    {
        int raw = gpiod_line_get_value(line);
        int value = (raw == 0) ? 1 : 0;

        if (value != last)
        {
            last = value;

            if (value == 1)
                mqtt_publish(client, TOPIC, "button_pressed");
            else
                mqtt_publish(client, TOPIC, "button_released");

            printf("Sent: %d\n", value);
        }

        MQTTClient_yield();
        usleep(100000);
    }

    return 0;
}
