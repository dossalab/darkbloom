#include <stdio.h>
#include <stdint.h>
#include <core/actions.h>
#include <core/screens.h>
#include <core/settings.h>
#include <drivers/ds18b20.h>
#include <drivers/ssd1306.h>
#include <drivers/pcf8563.h>
#include <utils/time.h>
#include <utils/tasks.h>
#include <board/board.h>

int global_temperature = 0;
int global_temperature_correct = 0;
struct time global_time;
int global_time_correct = 0;

static void display_off(void)
{
	ssd1306_set_enable(0);
}

static void hook_display_timers(void)
{
	tasks_set(30000, display_off, 1);
	tasks_set(5000, screen_return_main, 1);
}

void check_buttons(void)
{
	buttons_update();

	if (board_buttons.up) {
		hook_display_timers();
		ssd1306_set_enable(1);

		screen_press_button(BTN_UP);
		board_buttons.up = 0;
	}

	if (board_buttons.down) {
		hook_display_timers();
		ssd1306_set_enable(1);

		screen_press_button(BTN_DOWN);
		board_buttons.down = 0;
	}

	if (board_buttons.sw) {
		hook_display_timers();

		if (ssd1306_is_enabled()) {
			screen_switch_page();
		}

		ssd1306_set_enable(1);

		board_buttons.sw = 0;
	}
}

static void update_temp(void)
{
	static int ds18b20_updated;

	if (ds18b20_updated) {
		if (ds18b20_get_temp(&global_temperature) == 0) {
			global_temperature_correct = 1;
		} else {
			global_temperature_correct = 0;
		}

		screen_redraw();
		ds18b20_updated = 0;
	} else {
		ds18b20_convert();
		ds18b20_updated = 1;
	}
}

void update_time(void)
{
	static struct time last_time;

	if (pcf8563_get_time(&global_time) < 0) {
		global_time_correct = 0;
	} else {
		global_time_correct = 1;
	}

	if (!time_eq(&global_time, &last_time)) {
		screen_toggle_dots();
		screen_redraw();
	}

	last_time = global_time;
}

int main()
{
	board_init();
	tasks_init(SYSCLK_1MS_PRESC);

	tasks_set(1000, update_temp, 0);
	tasks_set(100, update_time, 0);
	tasks_set(500, actions_check, 0);
	tasks_set(50, check_buttons, 0);

	hook_display_timers();

	while (1) {
		tasks_proceed();
	}
}

