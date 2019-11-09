/*
 * fonts are mostly taken from
 * https://github.com/idispatch/raster-fonts
 */

#ifndef FONT_6x8_H
#define FONT_6x8_H

#include <stdint.h>

extern const uint8_t font_data_8x16[];
extern const uint8_t font_data_7x9[];
extern const uint8_t font_data_12x20_digits[];

struct font {
	int offset; /* for mapping characters to ascii codes */
	int w, h;
	const uint8_t *data;
};

static const struct font font_8x16 = {
	.offset = 32,
	.w = 8,
	.h = 16,
	.data = font_data_8x16
};

static const struct font font_7x9 = {
	.offset = 32,
	.w = 7,
	.h = 9,
	.data = font_data_7x9
};

#define SPACE_LARGE	','
static const struct font font_12x20_digits = {
	.offset = 44,
	.w = 12,
	.h = 20,
	.data = font_data_12x20_digits
};

#endif

