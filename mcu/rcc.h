#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define RCC_BASE	0x40021000

struct rcc {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBRSTR;
	volatile uint32_t CFGR2;
	volatile uint32_t CFGR3;
	volatile uint32_t CR2;
};

#define RCC	((struct rcc*)RCC_BASE)

enum rcc_line {
	RCC_GPIOA,
	RCC_GPIOB,
	RCC_GPIOC,
	RCC_GPIOF,
	RCC_I2C1,
	RCC_I2C2,
	RCC_TIMER2,
	RCC_TIMER3
};

void rcc_enable_line(enum rcc_line line);

#endif

