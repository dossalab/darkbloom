#include <drivers/ssd1306.h>
#include <core/screens.h>
#include <core/actions.h> /* For watering countdown */
#include <board/board.h>
#include <stdint.h>

#include <res/fonts.h> /* for custom icons */
#include <core/globals.h> /* for global temperature */
#include <core/settings.h>

enum {
	SCREEN_MAIN,
	SCREEN_SETTINGS
};

enum {
	DATA_TEMP,
	DATA_SWITCH,
	DATA_TIME
};

union page_data {
	int *as_temp;
	void (*as_switch)(int);
	struct time *as_time;
};

struct settings_page {
	char *title;
	int data_type;
	union page_data data;
};

static struct settings_page pages[] = {
	/* Setings */
	{
		.title = "[Temperature]",
		.data_type = DATA_TEMP,
		.data.as_temp = &(settings.temperature_setpoint)
	},
	{
		.title = "[Lights on]",
		.data_type = DATA_TIME,
		.data.as_time = &(settings.lights_on_time)
	},
	{
		.title = "[Lights off]",
		.data_type = DATA_TIME,
		.data.as_time = &(settings.lights_off_time)
	},
	{
		.title = "[Watering time]",
		.data_type = DATA_TIME,
		.data.as_time = &(settings.watering_time)
	},

	/* Tests */
	{
		.title = "[Test lights]",
		.data_type = DATA_SWITCH,
		.data.as_switch = &control_light
	},
	{
		.title = "[Test heater]",
		.data_type = DATA_SWITCH,
		.data.as_switch = &control_heater
	},
	{
		.title = "[Test pump]",
		.data_type = DATA_SWITCH,
		.data.as_switch = &control_pump
	},
};
#define SETTINGS_COUNT	(sizeof(pages) / sizeof(pages[0]))

static int current_screen = SCREEN_MAIN;
static int current_page = 0;

static int dot_on = 0;
void screen_redraw(void)
{
	ssd1306_clear(0);

	if (current_screen == SCREEN_MAIN) {
		if (global_temperature_correct) {
			ssd1306_printf(0, 5, P_LEFT | P_LARGE_DIGITS,
				"%d.%d",
				global_temperature / 10,
				global_temperature % 10
			);
		} else {
			ssd1306_puts(0, 5, P_LEFT | P_LARGE_DIGITS, "--.-");
		}

		if (global_time_correct) {
			ssd1306_printf(128, 5, P_RIGHT | P_LARGE_DIGITS,
					"%02d%c%02d",
					global_time.hour,
					dot_on? ':' : SPACE_LARGE,
					global_time.min
			);
		} else {
			ssd1306_printf(128, 5, P_RIGHT | P_LARGE_DIGITS,
					"--:--"
			);
		}
	} else {
		struct settings_page *page = &pages[current_page];

		ssd1306_puts(64, 0, P_CENTER, page->title);

		switch (page->data_type) {
		case DATA_TEMP:
			ssd1306_printf(64, 16, P_CENTER,
					"%d.%dC",
					*page->data.as_temp / 10,
					*page->data.as_temp % 10
			);

			break;

		case DATA_SWITCH:
			ssd1306_puts(64, 16, P_CENTER, "Use <up/down>");
			break;

		case DATA_TIME:
			ssd1306_printf(64, 16, P_CENTER,
					"%02d:%02d",
					page->data.as_time->hour,
					page->data.as_time->min
			);

			break;
		}
	}

	ssd1306_update();
}

void screen_toggle_dots(void)
{
	dot_on = !dot_on;
}

void screen_return_main(void)
{
	current_page = 0;
	current_screen = SCREEN_MAIN;
	screen_redraw();
}

void screen_switch_page(void)
{
	current_screen = SCREEN_SETTINGS;

	current_page++;
	if (current_page >= SETTINGS_COUNT) current_page = 0;
	screen_redraw();
}

void screen_press_button(int code)
{
	if (current_screen == SCREEN_MAIN) {
		/* Probably no key handler for main screen? */
	} else {
		struct settings_page *page = &pages[current_page];
		switch (page->data_type) {
		case DATA_TEMP:
			if (code == BTN_UP) {
				(*page->data.as_temp)++;
			} else
			if (code == BTN_DOWN) {
				(*page->data.as_temp)--;
			}

			break;

		case DATA_SWITCH:
			if (code == BTN_UP) {
				page->data.as_switch(1);
			} else
			if (code == BTN_DOWN) {
				page->data.as_switch(0);
			}

			break;

		case DATA_TIME:
			if (code == BTN_UP) {
				time_inc(page->data.as_time, 0);
			} else
			if (code == BTN_DOWN) {
				time_dec(page->data.as_time, 0);
			}

			break;
		}
	}

	screen_redraw();
}

