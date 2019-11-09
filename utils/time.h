#ifndef TIME_H
#define TIME_H

struct time {
	int hour;
	int min;
	int sec;
};

#define T_INC_SEC	(1 << 0)
#define T_DEC_SEC	(1 << 0)

int time_eg(struct time *a, struct time *b);
int time_less(struct time *a, struct time *b);

int time_eq(struct time *a, struct time *b);
void time_inc(struct time *t, int flags);
void time_dec(struct time *t, int flags);
int time_in_interval(struct time *t, struct time *start, struct time *end);

#endif

