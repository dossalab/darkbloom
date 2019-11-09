#ifndef PCF8563_H
#define PCF8563_H

#include <utils/time.h>

int pcf8563_init();
int pcf8563_get_time(struct time *t);
int pcf8563_set_time(struct time *t);

#endif

