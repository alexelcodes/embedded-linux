#define _POSIX_C_SOURCE 200809L

#include "pwm_hw.h"

#include <stdio.h>

#define PWM_CHIP_PATH "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL_PATH "/sys/class/pwm/pwmchip0/pwm0"

#define PWM_PERIOD_NS 20000000
#define PWM_CENTER_NS 1300000

static int write_integer_to_file(const char *path, int value)
{
    FILE *file = fopen(path, "w");

    if (file == NULL)
    {
        return -1;
    }

    fprintf(file, "%d", value);

    fflush(file);
    fclose(file);

    return 0;
}

int pwm_init(void)
{
    write_integer_to_file(PWM_CHIP_PATH "/export", 0);

    if (write_integer_to_file(PWM_CHANNEL_PATH "/enable", 0) != 0)
        return -1;

    if (write_integer_to_file(PWM_CHANNEL_PATH "/period", PWM_PERIOD_NS) != 0)
        return -1;

    if (write_integer_to_file(PWM_CHANNEL_PATH "/duty_cycle", PWM_CENTER_NS) != 0)
        return -1;

    if (write_integer_to_file(PWM_CHANNEL_PATH "/enable", 1) != 0)
        return -1;

    return 0;
}

int pwm_set_duty_ns(int duty_cycle_ns)
{
    return write_integer_to_file(
        PWM_CHANNEL_PATH "/duty_cycle",
        duty_cycle_ns);
}
