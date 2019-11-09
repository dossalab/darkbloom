/* - Board config -
 *
 * Led is on PA4
 *
 * Buttons:
 * - btn_sw on PA5
 * - btn_down on PA6
 * - btn_up on PA7
 *
 * 1-Wire bus: PB1
 *
 * I2C oled:
 * - SDA: PF0
 * - SCL: PF1
 *
 * Buzzer is on PA0
 *
 * Controls:
 * - pump: PA1
 * - heater: PA2
 * - light control: PA3
 *
 * System timer: systick
 */

#include <board/board.h>

#include <mcu/gpio.h>
#include <mcu/i2c.h>
#include <mcu/rcc.h>
#include <mcu/cortex.h>
#include <emu/onewire.h>

#include <drivers/ds18b20.h>
#include <drivers/ssd1306.h>
#include <drivers/pcf8563.h>

#ifdef USE_SPLASH
	#include <res/splash.h>
#endif

#define LED_PORT	GPIOA
#define LED_PIN		4

/* Loads */
#define PUMP_PORT	GPIOA
#define PUMP_PIN	1
#define HEATER_PORT	GPIOA
#define HEATER_PIN	2
#define LIGHTS_PORT	GPIOA
#define LIGHTS_PIN	3

/* Buttons */
#define BTN_PORT	GPIOA
#define BTN_UP		7
#define BTN_DOWN	6
#define BTN_SW		5

struct button_state board_buttons;

void control_light(int state)
{
	if (state) {
		gpio_set(LIGHTS_PORT, LIGHTS_PIN);
	} else {
		gpio_clr(LIGHTS_PORT, LIGHTS_PIN);
	}
}

void control_pump(int state)
{
	if (state) {
		gpio_set(PUMP_PORT, PUMP_PIN);
	} else {
		gpio_clr(PUMP_PORT, PUMP_PIN);
	}
}

void control_heater(int state)
{
	if (state) {
		gpio_set(HEATER_PORT, HEATER_PIN);
	} else {
		gpio_clr(HEATER_PORT, HEATER_PIN);
	}
}

void control_led(int state)
{
	if (state) {
		gpio_set(LED_PORT, LED_PIN);
	} else {
		gpio_clr(LED_PORT, LED_PIN);
	}
}

static inline void controls_init(void)
{
	gpio_out(LIGHTS_PORT, LIGHTS_PIN);
	gpio_out(PUMP_PORT, PUMP_PIN);
	gpio_out(HEATER_PORT, HEATER_PIN);
	gpio_out(LED_PORT, LED_PIN);
}

static inline void buttons_init(void)
{
	gpio_in(BTN_PORT, BTN_UP);
	gpio_in(BTN_PORT, BTN_DOWN);
	gpio_in(BTN_PORT, BTN_SW);
}

void buttons_update(void)
{
	if (gpio_read(BTN_PORT, BTN_UP) == 0) {
		/* button is pressed */
		if (!board_buttons.up_lock)
			board_buttons.up = 1;

		board_buttons.up_lock = 1;
	} else {
		board_buttons.up_lock = 0;
	}

	if (gpio_read(BTN_PORT, BTN_DOWN) == 0) {
		/* button is pressed */
		if (!board_buttons.down_lock)
			board_buttons.down = 1;

		board_buttons.down_lock = 1;
	} else {
		board_buttons.down_lock = 0;
	}

	if (gpio_read(BTN_PORT, BTN_SW) == 0) {
		/* button is pressed */
		if (!board_buttons.sw_lock)
			board_buttons.sw = 1;

		board_buttons.sw_lock = 1;
	} else {
		board_buttons.sw_lock = 0;
	}
}

static inline void display_init(void)
{
	rcc_enable_line(RCC_I2C1);

	gpio_af(GPIOA, 9, 4); /* AF1 - I2C1 */
	gpio_af(GPIOA, 10, 4);

	gpio_open_drain(GPIOF, 0);
	gpio_open_drain(GPIOF, 1);

	i2c_init(I2C1, I2C_400KHZ);

	ssd1306_init();

	#ifdef USE_SPLASH
		ssd1306_splash(oled_splash);
	#else
		ssd1306_puts(64, 10, P_CENTER, "Loading...");
	#endif

	ssd1306_update();
}

static inline void thermal_init(void)
{
	onewire_init(GPIOB, 1);
	ds18b20_init();
}

void board_init(void)
{
	rcc_enable_line(RCC_GPIOA);
	rcc_enable_line(RCC_GPIOF);
	rcc_enable_line(RCC_GPIOB);

	buttons_init();
	thermal_init();
	display_init();
	controls_init();
	pcf8563_init();
}

