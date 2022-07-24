#include <stdio.h>
#include "pico/stdlib.h"
#include "inky.h"

int main() {
    stdio_usb_init();
    int pin = PICO_DEFAULT_LED_PIN;
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    inky_init();
    while (1) {
        printf("Hello, world!\n");
        gpio_put(pin, 1);
        inky_toggle();
        sleep_ms(250);
        gpio_put(pin, 0);
        inky_toggle();
        sleep_ms(250);
    }
    return 0;
}
