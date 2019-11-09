#ifndef BOARD_H
#define BOARD_H

struct button_state {
	unsigned int up:1;
	unsigned int up_lock:1;
	unsigned int down:1;
	unsigned int down_lock:1;
	unsigned int sw:1;
	unsigned int sw_lock:1;
};

#define SYSCLK			8000000UL
#define SYSCLK_1MS_PRESC	8000

#define USE_SPLASH

extern struct button_state board_buttons;

void buttons_update(void);

void control_light(int state);
void control_pump(int state);
void control_heater(int state);
void control_led(int state);

void board_init(void);

#endif

