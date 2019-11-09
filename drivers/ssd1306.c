#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <mcu/i2c.h>
#include <drivers/ssd1306.h>
#include <string.h> /* memset */
#include <res/fonts.h>

#define SSD1306_I2C_ADDR		0x3C

#define SSD1306_MEMORYMODE		0x20
#define SSD1306_COLUMNADDR		0x21
#define SSD1306_PAGEADDR		0x22
#define SSD1306_SETCONTRAST		0x81
#define SSD1306_CHARGEPUMP		0x8D
#define SSD1306_SEGREMAP		0xA0
#define SSD1306_DISPLAYALLON_RESUME	0xA4
#define SSD1306_DISPLAYALLON		0xA5
#define SSD1306_NORMALDISPLAY		0xA6
#define SSD1306_INVERTDISPLAY		0xA7
#define SSD1306_SETMULTIPLEX		0xA8
#define SSD1306_DISPLAYOFF		0xAE
#define SSD1306_DISPLAYON		0xAF
#define SSD1306_COMSCANINC		0xC0
#define SSD1306_COMSCANDEC		0xC8
#define SSD1306_SETDISPLAYOFFSET	0xD3
#define SSD1306_SETDISPLAYCLOCKDIV	0xD5
#define SSD1306_SETPRECHARGE		0xD9
#define SSD1306_SETCOMPINS		0xDA
#define SSD1306_SETVCOMDETECT		0xDB
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_EXTERNALVCC		0x01
#define SSD1306_SWITCHCAPVCC		0x02

static uint8_t fb[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static int ssd1306_enabled = 0;

static inline void ssd1306_command(uint8_t command)
{
	i2c_write(I2C1, SSD1306_I2C_ADDR, 0x00, command);
}

static inline void ssd1306_data(uint8_t *data, int len)
{
	i2c_write_data(I2C1, SSD1306_I2C_ADDR, 0x40, data, len);
}

/* For 0.91" 128x32 oled panel */
void ssd1306_init(void)
{
	ssd1306_command(SSD1306_DISPLAYOFF);

	/* ?? */
	ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_command(0x80);

	ssd1306_command(SSD1306_SETMULTIPLEX);
	ssd1306_command(32 - 1);
	ssd1306_command(SSD1306_SETDISPLAYOFFSET);
	ssd1306_command(0x0);
	ssd1306_command(SSD1306_SETSTARTLINE | 0x0);

	/* Internal VCC pump */
	ssd1306_command(SSD1306_CHARGEPUMP);
	ssd1306_command(0x14);

	ssd1306_command(SSD1306_MEMORYMODE);
	ssd1306_command(0x00);
	ssd1306_command(SSD1306_SEGREMAP | 0x1);
	ssd1306_command(SSD1306_COMSCANDEC);
	ssd1306_command(SSD1306_SETCOMPINS);
	ssd1306_command(0x02);

	/* Contrast depends on VCC source */
	ssd1306_command(SSD1306_SETCONTRAST);
	ssd1306_command(0x8F);

	/* Precharge mode for internal VCC pump */
	ssd1306_command(SSD1306_SETPRECHARGE);
	ssd1306_command(0xF1);

	ssd1306_command(SSD1306_SETVCOMDETECT);
	ssd1306_command(0x40);
	ssd1306_command(SSD1306_DISPLAYALLON_RESUME);
	ssd1306_command(SSD1306_NORMALDISPLAY);

	ssd1306_command(SSD1306_DISPLAYON);

	ssd1306_enabled = 1;
}

void ssd1306_update(void)
{
	if (!ssd1306_enabled)
		return;

	ssd1306_command(SSD1306_COLUMNADDR);
	ssd1306_command(0); /* Start address*/
	ssd1306_command(SSD1306_WIDTH - 1); /* End address */

	ssd1306_command(SSD1306_PAGEADDR);
	ssd1306_command(0); /* Page start address */
	ssd1306_command(3); /* Page end address (4 - 1 bytes for 32px) */

	for (uint16_t i = 0; i < SSD1306_WIDTH*SSD1306_HEIGHT/8; i++) {
		ssd1306_data(&fb[i], 16);
		i+=15;
	}
}

void ssd1306_set_enable(int enable)
{
	if (enable) {
		ssd1306_command(SSD1306_DISPLAYON);
		ssd1306_update();
	} else {
		ssd1306_command(SSD1306_DISPLAYOFF);
	}

	ssd1306_enabled = enable;

}

int ssd1306_is_enabled(void)
{
	return ssd1306_enabled;
}

void ssd1306_set_invert(int invert)
{
	if (invert) {
		ssd1306_command(SSD1306_INVERTDISPLAY);
	} else {
		ssd1306_command(SSD1306_NORMALDISPLAY);
	}
}

/* Drawing */
static inline void set_pixel(int x, int y)
{
	fb[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
}

int pad_8(int a)
{
	int r = a % 8;
	if (r > 0) {
		return a + (8 - r);
	}

	return a;
}

void ssd1306_draw_char(int tar_x, int tar_y, char ch, const struct font *f)
{
	int x = 0, y = 0;
	int bit_counter = 7;
	int bytes_per_line = pad_8(f->w) / 8;
	int offset = (ch - f->offset) * f->h * bytes_per_line;
	const uint8_t *byte = f->data + offset;

	while (y < f->h) {
		if (*byte & (1 << bit_counter)) {
			set_pixel(tar_x + x, tar_y + y);
		}

		x++;
		bit_counter--;

		if (bit_counter < 0 || x >= f->w) {
			bit_counter = 7;
			byte++;
		}

		if (x >= f->w) {
			x = 0;
			y++;
		}
	}
}


void ssd1306_puts(int tar_x, int tar_y, int flags, char *str)
{
	const struct font *f = &font_8x16;
	int x = tar_x;

	if (flags & P_LARGE_DIGITS) {
		f = &font_12x20_digits;
	}

	if (flags & P_TINY) {
		f = &font_7x9;
	}

	if (flags & P_CENTER) {
		x -= (strlen(str) * f->w) / 2;
	}

	if (flags & P_RIGHT) {
		x -= strlen(str) * f->w;
	}

	while (*str) {
		ssd1306_draw_char(x, tar_y, *str, f);
		x += f->w;
		str++;
	}
}

void ssd1306_printf(int tar_x, int tar_y, int flags, char *fmt, ...)
{
	int n;
	va_list arg;
	char buf[64];

	va_start(arg, fmt);
	n = vsnprintf(buf, 64, fmt, arg);
	va_end(arg);

	if (n > 0) {
		ssd1306_puts(tar_x, tar_y, flags, buf);
	}
}

void ssd1306_clear(int color)
{
	if (color)
		color = 0xFF;

	memset(fb, color, sizeof(fb));
}

void ssd1306_splash(const uint8_t *bitmap)
{
	memcpy(fb, bitmap, sizeof(fb));
}

