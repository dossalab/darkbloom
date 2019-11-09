#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>
#include <mcu/gpio.h>

int onewire_reset(void);
void onewire_write(uint8_t data);
uint8_t onewire_read(void);
void onewire_init(struct gpio *base, int pin);

#endif

