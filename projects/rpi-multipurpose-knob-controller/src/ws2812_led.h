#ifndef WS2812_LED_H
#define WS2812_LED_H

#include <stdint.h>

int ws2812_init(void);
int ws2812_set_color(uint8_t r, uint8_t g, uint8_t b);
int ws2812_off(void);

#endif