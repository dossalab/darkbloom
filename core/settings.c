#include <core/settings.h>

struct device_settings settings = {
	.temperature_setpoint = 250,
	.lights_on_time = {8, 0},
	.lights_off_time = {22, 0},
	.watering_time = {8, 0}
};

