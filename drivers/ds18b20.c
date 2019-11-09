#include <drivers/ds18b20.h>
#include <emu/onewire.h>
#include <stdint.h>

static uint8_t crc_update(uint8_t crc, uint8_t data)
{
	crc = crc ^ data;

	for (int i = 0; i < 8; i++) {
		if (crc & 0x01)
			crc = (crc >> 1) ^ 0x8C;
		else
			crc >>= 1;
	}

	return crc;
}

static uint8_t crc_okay(uint8_t *ram, uint8_t size)
{
	uint8_t crc = 0;

	for (int i = 0; i < 8; i++) {
		crc = crc_update(crc, ram[i]);
	}

	if (crc == ram[8]) {
		return 1;
	}

	return 0;
}

static int ds18b20_read_ram(uint8_t *ram, int size)
{
	if (onewire_reset() != 0) {
		return -1;
	}

	onewire_write(0xCC);  /* Skip ROM */
	onewire_write(0xBE);  /* Read */

	for (int i = 0; i < size; i++)
		ram[i] = onewire_read();

	return 0;
}

int ds18b20_init(void)
{
	uint8_t ram[9];
	if (ds18b20_read_ram(ram, sizeof(ram)) != 0) {
		return -1;
	}

	return 0;
}

int ds18b20_convert(void)
{
	if (onewire_reset() != 0) {
		return -1;
	}

	onewire_write(0xCC); /* Skip ROM */
	onewire_write(0x44); /* Convert */

	return 0;
}

int ds18b20_get_temp(int *temp)
{
	uint8_t ram[9];
	if (ds18b20_read_ram(ram, sizeof(ram)) != 0) {
		return -1;
	}

	if (ram[7] == 0x10) {
		if (crc_okay(ram, 9)) {
			int16_t temp_raw = ram[1] << 8 | ram[0];
			*temp = temp_raw * 10 / 16;
			return 0;
		}

		return -1;
	} else {
		return -1;
	}

	return 0;
}

