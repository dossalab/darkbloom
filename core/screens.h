#ifndef SCREENS_H
#define SCREENS_H

enum {
	BTN_UP,
	BTN_DOWN
};

void screen_toggle_dots(void);
void screen_redraw(void);
void screen_return_main(void);
void screen_switch_page(void);
void screen_press_button(int code);

#endif

