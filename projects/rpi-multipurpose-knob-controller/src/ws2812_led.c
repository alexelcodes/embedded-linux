#include "ws2812_led.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPI_DEV "/dev/spidev0.0"

/*
 * WS2812 driver using SPI.
 *
 * SPI speed (~6.5 MHz) and byte patterns were chosen
 * to approximate WS2812 timing (~1.25 µs per bit).
 */
#define SPI_SPEED 6500000U

/*
 * SPI patterns for WS2812 bits.
 *
 * More leading '1's → longer HIGH (bit '1')
 * Fewer leading '1's → shorter HIGH (bit '0')
 */
#define LED_ZERO 0x80 // 1000 0000
#define LED_ONE 0xF8  // 1111 1000

/* SPI file descriptor */
static int spi_fd = -1;

/*
 * Convert one color byte into 8 SPI bytes.
 *
 * Each bit of the input byte becomes one SPI pattern:
 * - bit 0 -> LED_ZERO
 * - bit 1 -> LED_ONE
 *
 * Bits are sent MSB first.
 */
static void encode_byte(uint8_t value, uint8_t *out)
{
    for (int i = 7; i >= 0; i--)
        *out++ = (value & (1u << i)) ? LED_ONE : LED_ZERO;
}

/*
 * Send one RGB color frame to the WS2812.
 *
 * One LED needs 3 color bytes:
 * R, G, B
 *
 * Each color byte becomes 8 SPI bytes,
 * so the full buffer size is 24 bytes.
 */
static int send_color(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t tx[24];
    uint8_t *p = tx;

    encode_byte(r, p);
    p += 8;
    encode_byte(g, p);
    p += 8;
    encode_byte(b, p);

    return (write(spi_fd, tx, sizeof(tx)) == sizeof(tx)) ? 0 : -1;
}

/*
 * Open and configure SPI.
 */
int ws2812_init(void)
{
    spi_fd = open(SPI_DEV, O_RDWR);
    if (spi_fd < 0)
        return -1;

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = SPI_SPEED;

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0)
        return -1;
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
        return -1;
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        return -1;

    return 0;
}

/*
 * Set WS2812 color.
 *
 * Send one RGB frame.
 */
int ws2812_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    if (spi_fd < 0)
        return -1;

    return send_color(r, g, b);
}

/*
 * LED off.
 */
int ws2812_off(void)
{
    return ws2812_set_color(0, 0, 0);
}