#include <mcu/gpio.h>

void gpio_in(struct gpio *bank, int pin)
{
	bank->MODER &= ~(0b11 << 2 * pin);
}

void gpio_out(struct gpio *bank, int pin)
{
	bank->MODER &= ~(0b11 << 2 * pin);
	bank->MODER |=  (0b01 << 2 * pin);
}

void gpio_af(struct gpio *bank, int pin, int af)
{
	bank->MODER &= ~(0b11 << 2 * pin);
	bank->MODER |=  (0b10 << 2 * pin);

	if (pin < 8) {
		bank->AFRL &= ~(0b1111 << 4 * pin);
		bank->AFRL |=  (af << 4 * pin);
	} else {
		bank->AFRH &= ~(0b1111 << 4 * (pin - 8));
		bank->AFRH |=  (af << 4 * (pin - 8));
	}
}

void gpio_open_drain(struct gpio *bank, int pin)
{
	// HACK!!
	bank->OSPEEDR &= ~(0b11 << 2 * pin);
	bank->OSPEEDR |=  (0b11 << 2 * pin);

	bank->OTYPER |= (1 << pin);
}

void gpio_pullup(struct gpio *bank, int pin)
{
	bank->PUPDR &= ~(0b11 << 2 * pin);
	bank->PUPDR |=  (0b01 << 2 * pin);
}

void gpio_set(struct gpio *bank, int pin)
{
	bank->ODR |= (1 << pin);
}

void gpio_clr(struct gpio *bank, int pin)
{
	bank->ODR &= ~(1 << pin);
}

void gpio_toggle(struct gpio *bank, int pin)
{
	bank->ODR ^= (1 << pin);
}

int gpio_read(struct gpio *bank, int pin)
{
	return (bank->IDR & (1 << pin))? 1 : 0;
}

