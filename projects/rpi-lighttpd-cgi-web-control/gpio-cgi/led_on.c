#include <gpiod.h>
#include <stdio.h>
#include <time.h>

#define GPIO_LINE 23
#define LOG_FILE "/tmp/led.log"
#define CONSUMER "rpi-lighttpd-cgi-web-control"

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;

    // HTTP header (required for CGI)
    printf("Content-type: text/html\r\n\r\n");

    // open GPIO controller and get output line
    chip = gpiod_chip_open_by_name("gpiochip0");
    line = gpiod_chip_get_line(chip, GPIO_LINE);

    // reserve GPIO as output and turn LED ON
    gpiod_line_request_output(line, CONSUMER, 0);
    gpiod_line_set_value(line, 1);

    // write timestamp to log
    FILE *f = fopen(LOG_FILE, "a");

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);

    fprintf(f, "LED ON  %s\n", buf);
    fclose(f);

    // response body
    printf("<html><body>LED ON</body></html>\n");

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
