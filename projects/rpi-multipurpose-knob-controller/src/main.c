#include "controller.h"
#include "button.h"
#include "knob.h"

#include <pthread.h>

// Thread wrapper for button handling
static void *button_thread(void *arg)
{
    (void)arg;
    button_start();
    return NULL;
}

// Thread wrapper for rotary encoder handling
static void *knob_thread(void *arg)
{
    (void)arg;
    knob_start();
    return NULL;
}

int main(void)
{
    // Initialize controller, servo, mode LED, and WS2812
    controller_init();

    pthread_t t_button;
    pthread_t t_knob;

    // Start button thread
    pthread_create(&t_button, NULL, button_thread, NULL);

    // Start rotary encoder thread
    pthread_create(&t_knob, NULL, knob_thread, NULL);

    // Wait forever
    pthread_join(t_button, NULL);
    pthread_join(t_knob, NULL);

    return 0;
}