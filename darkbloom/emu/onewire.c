/* software 1-Wire emulation on PB1 */

#include <emu/onewire.h>
#include <mcu/cortex.h>

/* Some default port, prevents null pointer dereferencing */
static struct gpio *onewire_port = GPIOB;
static int onewire_pin = 1;

/* Stupid delay for 8 Mhz clock */
static inline void delay_us(int us)
{
	for (volatile int i = 0; i < us; i++);
}

static inline void bus_down(void)
{
	onewire_port->ODR &= ~(1 << onewire_pin);
}

static inline void bus_up(void)
{
	onewire_port->ODR |= (1 << onewire_pin);
}

static inline int bus_get(void)
{
	return onewire_port->IDR & (1 << onewire_pin);
}

int onewire_reset(void)
{
	int ret = -1;

	bus_down();
	delay_us(500);
	bus_up();

	delay_us(60);

	if (bus_get() == 0) {
		ret = 0;
	}

	delay_us(200);

	return ret;
}


void onewire_write(uint8_t data)
{
	for (int i = 0; i < 8; i++) {
		if (data & (1 << i)) {
			bus_down();
			delay_us(1);
			bus_up();
			delay_us(59);
		} else {
			bus_down();
			delay_us(60);
			bus_up();
		}
	}
}


uint8_t onewire_read(void)
{
	uint8_t byte = 0;

	for (int i = 0; i < 8; i++) {
		bus_down();
		__DISABLE_IRQ;
		bus_up();

		if (bus_get()) {
			byte |= (1 << i);
		}

		__ENABLE_IRQ;
		delay_us(45);
	}

	return byte;
}

void onewire_init(struct gpio *base, int pin)
{
	onewire_port = base;
	onewire_pin = pin;

	gpio_out(base, pin);
	gpio_open_drain(base, pin);
}

