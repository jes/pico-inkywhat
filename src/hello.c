#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    int pin = PICO_DEFAULT_LED_PIN;
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    while (1) {
        gpio_put(pin, 1);
        sleep_ms(250);
        gpio_put(pin, 0);
        sleep_ms(250);
    }
    return 0;
}
