#include <utils/time.h>

/* == */
int time_eq(struct time *a, struct time *b)
{
	return (a->hour == b->hour && a->min == b->min && a->sec == b->sec);
}

/* >= */
int time_eg(struct time *a, struct time *b)
{
	if (a->hour > b->hour) {
		return 1;
	}

	if (a->hour == b->hour && a->min >= b->min) {
		return 1;
	}

	return 0;
}

/* < */
int time_less(struct time *a, struct time *b)
{
	if (a->hour < b->hour) {
		return 1;
	}

	if (a->hour == b->hour && a->min < b->min) {
		return 1;
	}

	return 0;
}

void time_inc(struct time *t, int flags)
{
	if (flags & T_INC_SEC) {
		t->sec++;

		if (t->sec >= 60) {
			t->sec = 0;
			t->min++;
		}
	} else {
		t->min++;
	}

	if (t->min >= 60) {
		t->min = 0;
		t->hour++;

		if (t->hour >= 24) {
			t->hour = 0;
		}
	}
}

void time_dec(struct time *t, int flags)
{
	if (flags & T_DEC_SEC) {
		t->sec--;

		if (t->sec < 0) {
			t->sec = 59;
			t->min--;
		}
	} else {
		t->min--;
	}

	if (t->min < 0) {
		t->min = 59;
		t->hour--;

		if (t->hour < 0) {
			t->hour = 23;
		}
	}

}

int time_in_interval(struct time *t, struct time *start, struct time *end)
{
	return (time_eg(t, start) && time_less(t, end));
}

