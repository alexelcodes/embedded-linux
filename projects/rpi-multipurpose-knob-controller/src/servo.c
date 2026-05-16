#define _POSIX_C_SOURCE 200809L

#include "servo.h"

#include <stdio.h>

#define PWMCHIP "/sys/class/pwm/pwmchip0"
#define PWM0 "/sys/class/pwm/pwmchip0/pwm0"

#define PERIOD_NS 20000000

#define SERVO_MIN_NS 1000000
#define SERVO_MAX_NS 2000000

// Helper function to write integer values to PWM sysfs files
static void write_int(const char *path, int v)
{
    FILE *f = fopen(path, "w");
    fprintf(f, "%d", v);
    fflush(f);
    fclose(f);
}

void servo_init(void)
{
    // Initialize hardware PWM for servo control
    write_int(PWMCHIP "/export", 0);
    write_int(PWM0 "/enable", 0);
    write_int(PWM0 "/period", PERIOD_NS);
    write_int(PWM0 "/duty_cycle", 1500000);
    write_int(PWM0 "/enable", 1);
}

void servo_set(int percent)
{
    // Convert percentage (0–100%) to PWM duty cycle for servo position
    int duty = SERVO_MIN_NS +
               (SERVO_MAX_NS - SERVO_MIN_NS) * percent / 100;

    write_int(PWM0 "/duty_cycle", duty);
}