#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_usb_init();
    int pin = PICO_DEFAULT_LED_PIN;
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    while (1) {
        printf("Hello, world!\n");
        gpio_put(pin, 1);
        sleep_ms(250);
        gpio_put(pin, 0);
        sleep_ms(250);
    }
    return 0;
}
