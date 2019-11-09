#ifndef I2C_H
#define I2C_H

#include <stdint.h>

struct i2c_base {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t TIMINGR;
	volatile uint32_t TIMEOUTR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t PECR;
	volatile uint32_t RXDR;
	volatile uint32_t TXDR;
};

#define I2C1_BASE	0x40005400
#define I2C2_BASE	0x40005800
#define I2C1		(struct i2c_base *)I2C1_BASE
#define I2C2		(struct i2c_base *)I2C2_BASE

/*
 * Timing register value is computed with the AN4235 xls file,
 * fast Mode 400kHz with I2CCLK = 8MHz
 */
#define I2C_400KHZ	0x00310309

void i2c_init(struct i2c_base *base, uint32_t timing);
uint8_t i2c_read(struct i2c_base *base, uint8_t addr, uint8_t reg);
void i2c_read_data(struct i2c_base *base, uint8_t addr,
		uint8_t reg, uint8_t *data, int maxlen);

void i2c_write(struct i2c_base *base, uint8_t addr, uint8_t reg, uint8_t val);
void i2c_write_data(struct i2c_base *base, uint8_t addr,
		uint8_t reg, uint8_t *data, int len);

#endif

