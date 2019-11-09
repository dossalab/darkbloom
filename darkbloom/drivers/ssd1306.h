#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define USE_SSD1306_PRINTF

#define SSD1306_WIDTH		128
#define SSD1306_HEIGHT		32

/* Flags for ssd1306_printf */
#define P_LEFT		(1 << 0)
#define P_RIGHT		(1 << 1)
#define P_CENTER	(1 << 2)
#define P_LARGE_DIGITS	(1 << 3)
#define P_TINY		(1 << 4)

void ssd1306_init(void);
void ssd1306_update(void);
void ssd1306_set_invert(int invert);

void ssd1306_set_enable(int enable);
int ssd1306_is_enabled(void);

/* Drawing */
void ssd1306_puts(int tar_x, int tar_y, int flags, char *str);
void ssd1306_printf(int tar_x, int tar_y, int flags, char *fmt, ...);

void ssd1306_clear(int color);
void ssd1306_splash(const uint8_t *bitmap);

#endif
