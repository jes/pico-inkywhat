#ifndef INKY_H_INC
#define INKY_H_INC

#include "pico/stdlib.h"
#include "hardware/spi.h"

void inky_init(void);
void inky_toggle(void);

void _inky_send_command(uint8_t command, uint8_t arg);
void _inky_send_command_data(uint8_t command, const uint8_t *data, size_t len);
void _inky_busy_wait(void);

#endif
