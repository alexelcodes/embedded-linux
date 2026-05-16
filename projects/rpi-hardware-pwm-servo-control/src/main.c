#define _POSIX_C_SOURCE 200809L

#include "pwm_hw.h"

#include <signal.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>

#define TIMER_INTERVAL_NS 40000000

#define SERVO_MIN_NS 400000
#define SERVO_MAX_NS 2200000

static timer_t pwm_timer;

/*
 * S-curve profile values in 0.1% units.
 *
 * Example:
 * 1000 = 100.0%
 * 4    = 0.4%
 */
static const int s_curve_profile[] = {
    0, 4, 13, 30, 56, 93, 142, 203,
    270, 343, 419, 497, 576, 652,
    725, 792, 852, 903, 943, 970,
    986, 996, 1000};

#define PROFILE_SIZE (sizeof(s_curve_profile) / sizeof(s_curve_profile[0]))

static void timer_handler(int signal_number)
{
    (void)signal_number;

    static size_t step = 0;

    /*
     * Create forward + reverse movement:
     *
     * 0 -> N-1 -> 0
     */
    size_t index = step % (2 * (PROFILE_SIZE - 1));

    if (index >= PROFILE_SIZE)
    {
        index = 2 * (PROFILE_SIZE - 1) - index;
    }

    /*
     * Convert percentage value to PWM duty cycle.
     */
    int duty_cycle_ns =
        SERVO_MIN_NS +
        (s_curve_profile[index] * (SERVO_MAX_NS - SERVO_MIN_NS)) / 1000;

    pwm_set_duty_ns(duty_cycle_ns);

    step++;
}

int main(void)
{
    if (pwm_init() != 0)
    {
        return 1;
    }

    struct sigaction signal_action = {0};

    signal_action.sa_handler = timer_handler;

    sigaction(SIGALRM, &signal_action, NULL);

    struct sigevent timer_event = {0};

    timer_event.sigev_notify = SIGEV_SIGNAL;
    timer_event.sigev_signo = SIGALRM;

    if (timer_create(CLOCK_REALTIME, &timer_event, &pwm_timer) != 0)
    {
        return 1;
    }

    struct itimerspec timer_spec = {0};

    timer_spec.it_value.tv_nsec = TIMER_INTERVAL_NS;
    timer_spec.it_interval = timer_spec.it_value;

    if (timer_settime(pwm_timer, 0, &timer_spec, NULL) != 0)
    {
        return 1;
    }

    while (1)
    {
        pause();
    }

    return 0;
}
