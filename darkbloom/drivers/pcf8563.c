#include <drivers/pcf8563.h>
#include <mcu/i2c.h>

#define PCF8563_I2C_ADDR	0x51

static uint8_t bcd_to_dec(uint8_t value)
{
	return ((value / 16) * 10 + value % 16);
}

static uint8_t dec_to_bcd(uint8_t value)
{
	return (value / 10 * 16 + value % 10);
}

int pcf8563_init()
{
	return -1;
}

int pcf8563_get_time(struct time *t)
{
	uint8_t data[7] = {0};

	i2c_read_data(I2C1, PCF8563_I2C_ADDR, 0x02, data, sizeof(data));

	t->sec = bcd_to_dec(data[0] & 0b01111111);
	t->min = bcd_to_dec(data[1] & 0b01111111);
	t->hour = bcd_to_dec(data[2] & 0b00111111);

	return 0;
}

int pcf8563_set_time(struct time *t)
{
	uint8_t data[] = {
		dec_to_bcd(t->sec), /* sec */
		dec_to_bcd(t->min), /* min */
		dec_to_bcd(t->hour), /* hour */
		dec_to_bcd(0),
		dec_to_bcd(0),
		dec_to_bcd(0),
		dec_to_bcd(0),
	};

	i2c_write_data(I2C1, PCF8563_I2C_ADDR, 0x02, data, sizeof(data));

	return 0;
}

