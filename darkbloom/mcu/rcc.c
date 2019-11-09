#include <mcu/rcc.h>

struct rcc_block{
	volatile uint32_t *domain;
	uint32_t bitmask;
};

static struct rcc_block rcc_blocks[] = {
	[RCC_GPIOA] = {&RCC->AHBENR, (1 << 17) },
	[RCC_GPIOB] = {&RCC->AHBENR, (1 << 18) },
	[RCC_GPIOC] = {&RCC->AHBENR, (1 << 19) },
	[RCC_GPIOF] = {&RCC->AHBENR, (1 << 22) },

	[RCC_I2C1] = {&RCC->APB1ENR, (1 << 21) },
	[RCC_I2C2] = {&RCC->APB1ENR, (1 << 22) },

	[RCC_TIMER2] = {&RCC->APB1ENR, (1 << 0) },
	[RCC_TIMER3] = {&RCC->APB1ENR, (1 << 1) }
};

void rcc_enable_line(enum rcc_line line)
{
	*rcc_blocks[line].domain |= rcc_blocks[line].bitmask;
}

