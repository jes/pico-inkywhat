/* InkyWHAT library for Pi Pico
 *
 * Logic copied from https://github.com/pimoroni/inky/blob/master/library/inky/inky.py
 *
 * James Stanley 2022
 */

#include "inky.h"

const int busy_pin = 27;
const int reset_pin = 26;
const int dc_pin = 22;
const uint cs_pin = 17;
const uint sck_pin = 18;
const uint mosi_pin = 19;

/* This is the "black" lookup table from the Python library; this is encoding
 * instructions to the eInk display about how to cycle the voltages applied
 * to each pixel. See the Python library for a better explanation.
 */
const uint8_t black_lut[] = {
    0x48, 0xa0, 0x10, 0x13, 0x00, 0x00, 0x00,
    0x48, 0xa0, 0x80, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x48, 0xa5, 0x00, 0xbb, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x04, 0x04, 0x04, 0x04,
    0x10, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x08, 0x08, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
};

//uint8_t inky_pixbuf[15000]; /* 400x300 bits */
#include "pix.h"

void inky_init(void) {
    /* initialise GPIO pins */
    gpio_init(reset_pin);
    gpio_set_dir(reset_pin, GPIO_OUT);
    gpio_put(reset_pin, 1);

    gpio_init(dc_pin);
    gpio_set_dir(dc_pin, GPIO_OUT);
    gpio_put(reset_pin, 0);

    gpio_init(busy_pin);
    gpio_set_dir(busy_pin, GPIO_IN);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);

    /* initialise SPI */
    spi_init(spi0, 488000);
    spi_set_slave(spi0, false);
    spi_set_format(spi0, 8, 1, 1, SPI_MSB_FIRST);
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);

    /* reset the inky */
    gpio_put(reset_pin, 0);
    sleep_ms(100);
    gpio_put(reset_pin, 1);
    sleep_ms(100);

    _inky_send_command_data(0x12, 0, 0); /* Soft Reset */
    _inky_busy_wait();
}

void inky_toggle(void) {
    _inky_send_command(0x74, 0x54); /* Set Analog Block Control */
    _inky_send_command(0x7e, 0x3b); /* Set Digital Block Control */

    /* in the Python library this seems to transmit the number of pixel
     * rows, as a little-endian 16-bit value, followed by a \x00;
     * we have 300 rows = 0x12c
     */
    _inky_send_command_data(0x01, "\x2c\x01\x00", 3); /* Gate setting */

    _inky_send_command(0x03, 0x17); /* Gate Driving Voltage */
    _inky_send_command_data(0x04, "\x41\xac\x32", 3); /* Source Driving Voltage */

    _inky_send_command(0x3a, 0x07); /* Dummy line period */
    _inky_send_command(0x3b, 0x04); /* Gate line width */
    _inky_send_command(0x11, 0x03); /* Data entry mode setting 0x03 = X/Y increment */

    _inky_send_command(0x2c, 0x3c); /* VCOM Register, 0x3c = -1.5v? */

    _inky_send_command(0x3c, 0); /* Python library sends this with no comment */
    /* For white "border colour": */
    _inky_send_command(0x3c, 0x31); /* GS Transition Define A + VSS + LUT0 */

    /* Python library does something here for colour displays, which we ignore */

    _inky_send_command_data(0x32, black_lut, sizeof(black_lut));

    /* [0x00, (self.cols // 8) - 1] */
    _inky_send_command_data(0x44, "\x00\x31", 2); /* Set RAM X Start/End */

    /* [0x00, 0x00] + packed_height */
    _inky_send_command_data(0x45, "\x00\x00\x2c\x01", 4); /* Set RAM Y Start/End */

    _inky_send_command(0x4e, 0x00); /* Set RAM X Pointer Start */
    _inky_send_command_data(0x4f, "\x00\x00", 2); /* Set RAM Y Pointer Start */
    _inky_send_command_data(0x24, inky_pixbuf, sizeof(inky_pixbuf)); /* Pixel data */

    _inky_send_command(0x4e, 0x00); /* Set RAM X Pointer Start */
    _inky_send_command_data(0x4f, "\x00\x00", 2); /* Set RAM Y Pointer Start */
    _inky_send_command_data(0x26, inky_pixbuf, sizeof(inky_pixbuf)); /* Pixel data */

    _inky_send_command(0x22, 0xc7); /* Display Update Sequence */
    _inky_send_command_data(0x20, 0, 0); /* Trigger Display Update */

    _inky_busy_wait();
    _inky_send_command(0x10, 0x01); /* Enter Deep Sleep */
}

void _inky_send_command(uint8_t command, uint8_t arg) {
    gpio_put(cs_pin, 0);
    gpio_put(dc_pin, 0);
    spi_write_blocking(spi0, &command, 1);
    gpio_put(dc_pin, 1);
    spi_write_blocking(spi0, &arg, 1);
    gpio_put(cs_pin, 1);
}

void _inky_send_command_data(uint8_t command, const uint8_t *data, size_t len) {
    gpio_put(cs_pin, 0);
    gpio_put(dc_pin, 0);
    spi_write_blocking(spi0, &command, 1);
    gpio_put(dc_pin, 1);
    spi_write_blocking(spi0, data, len);
    gpio_put(cs_pin, 1);
}

void _inky_busy_wait(void) {
    while (!gpio_get(busy_pin))
        sleep_ms(10);
}
