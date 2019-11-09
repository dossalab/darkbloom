#include <core/settings.h>
#include <board/board.h>
#include <stddef.h>
#include <core/globals.h>
#include <core/actions.h>
#include <utils/tasks.h>

#define WATERING_DURATION	2000

static void watering_stop(void)
{
	control_pump(0);
}

void actions_check(void)
{
	if (global_temperature_correct) {
		if (global_temperature > settings.temperature_setpoint) {
			control_heater(0);
		} else {
			control_heater(1);
		}
	} else {
		control_heater(0);
	}

	if (time_in_interval(&global_time, &settings.lights_on_time,
				&settings.lights_off_time)) {
		control_light(1);
	} else {
		control_light(0);
	}
}

