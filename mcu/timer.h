#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

struct timer_base {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
};

#define TIMER2_BASE	0x40000000
#define TIMER3_BASE	0x40000400

#define TIMER2	((struct timer_base*)TIMER2_BASE)
#define TIMER3	((struct timer_base*)TIMER3_BASE)

void timer_init(struct timer_base *base, int presc);

void timer_start(struct timer_base *base);
uint32_t timer_stop(struct timer_base *base);
void timer_delay(struct timer_base *base, uint32_t value);

#endif

