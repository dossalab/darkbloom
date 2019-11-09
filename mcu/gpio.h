#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIOA_BASE	0x48000000 /* GPIO Port A */
#define GPIOB_BASE	0x48000400 /* GPIO Port B */
#define GPIOC_BASE	0x48000800 /* GPIO Port C */
#define GPIOF_BASE	0x48001400 /* GPIO Port F */

struct gpio {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	volatile uint32_t BRR;
};

#define GPIOA	((struct gpio*)GPIOA_BASE)
#define GPIOB	((struct gpio*)GPIOB_BASE)
#define GPIOC	((struct gpio*)GPIOC_BASE)
#define GPIOF	((struct gpio*)GPIOF_BASE)

void gpio_af(struct gpio *bank, int pin, int af);
void gpio_in(struct gpio *bank, int pin);
void gpio_out(struct gpio *bank, int pin);
void gpio_pullup(struct gpio *bank, int pin);
void gpio_open_drain(struct gpio *bank, int pin);

void gpio_set(struct gpio *bank, int pin);
void gpio_clr(struct gpio *bank, int pin);
void gpio_toggle(struct gpio *bank, int pin);

int gpio_read(struct gpio *bank, int pin);

#endif

