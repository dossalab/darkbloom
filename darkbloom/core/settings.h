#ifndef SETTINGS_H
#define SETTINGS_H

#include <utils/time.h>

struct device_settings {
	int temperature_setpoint;
	struct time lights_on_time;
	struct time lights_off_time;
	struct time watering_time;
};

extern struct device_settings settings;

#endif

