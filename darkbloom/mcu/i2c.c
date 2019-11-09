#include <mcu/i2c.h>

/* CR1 bits */
#define CR1_PE_BIT	(1 << 0)

/* CR2 bits */
#define CR2_RD_WRN_BIT	(1 << 10) /* transfer direction */
#define CR2_START_BIT	(1 << 13)
#define CR2_STOP_BIT	(1 << 14)
#define CR2_AUTOEND_BIT	(1 << 25)

/* ISR bits */
#define ISR_TXE_BIT	(1 << 0)
#define ISR_TXIS_BIT	(1 << 1)
#define ISR_RXNE_BIT	(1 << 2)
#define ISR_NACKF_BIT	(1 << 4)
#define ISR_STOPF_BIT	(1 << 5)
#define ISR_TC_BIT	(1 << 6)
#define ISR_BUSY_BIT	(1 << 15)

/* ICR bits */
#define ICR_NACKCF_BIT	(1 << 4)
#define ICR_STOPCF_BIT	(1 << 5)

void i2c_init(struct i2c_base *base, uint32_t timing)
{
	base->TIMINGR = timing;
	base->CR1 |= CR1_PE_BIT;
}

static void i2c_start(struct i2c_base *base, uint8_t addr, int count)
{
	while (base->ISR & ISR_BUSY_BIT);
	base->CR2 = CR2_START_BIT | (count << 16) | (addr << 1);
}

static void i2c_write_byte(struct i2c_base *base, uint8_t byte)
{
	while ((base->ISR & ISR_TXIS_BIT) == 0) {
		if (base->ISR & ISR_NACKF_BIT) {
			return;
		}
	}

	base->TXDR = byte;
}

static uint8_t i2c_read_byte(struct i2c_base *base)
{
	while ((base->ISR & ISR_RXNE_BIT) == 0) {
		if (base->ISR & ISR_NACKF_BIT) {
			return 0;
		}
	}

	return (uint8_t)base->RXDR;
}

static void i2c_request(struct i2c_base *base, int addr, int len)
{
	base->CR2 = CR2_START_BIT | CR2_RD_WRN_BIT | (len << 16) | (addr << 1);
}

static void i2c_wait_tc(struct i2c_base *base)
{
	if (base->ISR & ISR_NACKF_BIT) {
		return;
	}

	while ((base->ISR & ISR_TC_BIT) == 0);
	base->ISR &= ~ISR_TC_BIT;
}

static void i2c_stop(struct i2c_base *base)
{
	if (base->ISR & ISR_NACKF_BIT) {
		base->ICR |= ICR_NACKCF_BIT;
	}

	base->CR2 |= CR2_STOP_BIT;
}

void i2c_read_data(struct i2c_base *base, uint8_t addr,
		uint8_t reg, uint8_t *data, int maxlen)
{
	i2c_start(base, addr, 1);

	i2c_write_byte(base, reg);
	i2c_wait_tc(base);

	i2c_request(base, addr, maxlen);
	for (int i = 0; i < maxlen; i++) {
		data[i] = i2c_read_byte(base);
	}

	i2c_stop(base);
}

uint8_t i2c_read(struct i2c_base *base, uint8_t addr, uint8_t reg)
{
	uint8_t val = 0;

	i2c_start(base, addr, 1);

	i2c_write_byte(base, reg);
	i2c_wait_tc(base);

	i2c_request(base, addr, 1);
	val = i2c_read_byte(base);
	i2c_stop(base);

	return val;
}

void i2c_write(struct i2c_base *base, uint8_t addr, uint8_t reg, uint8_t val)
{
	i2c_start(base, addr, 2);
	i2c_write_byte(base, reg);
	i2c_write_byte(base, val);
	i2c_wait_tc(base);
	i2c_stop(base);
}

void i2c_write_data(struct i2c_base *base, uint8_t addr,
		uint8_t reg, uint8_t *data, int len)
{
	i2c_start(base, addr, len + 1);
	i2c_write_byte(base, reg);

	for (int i = 0; i < len; i++) {
		i2c_write_byte(base, data[i]);
	}

	i2c_wait_tc(base);
	i2c_stop(base);
}

