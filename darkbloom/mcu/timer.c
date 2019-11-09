#include <mcu/timer.h>

#define CR1_CEN_BIT	(1 << 0)
#define CR1_OPM_BIT	(1 << 3)
#define SR_UIF_BIT	(1 << 0)
#define EGR_UG_BIT	(1 << 0)

static inline void reload_shadow(struct timer_base *base)
{
	base->EGR |= EGR_UG_BIT;
}

void timer_init(struct timer_base *base, int presc)
{
	base->PSC = presc - 1;
	reload_shadow(base);

	base->CR1 |= CR1_OPM_BIT;
}

void timer_start(struct timer_base *base)
{
	base->CNT = 0;
	base->ARR = 65535 - 1;
	reload_shadow(base);

	base->CR1 |= CR1_CEN_BIT;
}

uint32_t timer_stop(struct timer_base *base)
{
	base->CR1 &= ~CR1_CEN_BIT;

	return base->CNT;
}

void timer_delay(struct timer_base *base, uint32_t value)
{
	base->ARR = value;
	base->CNT = 0;
	reload_shadow(base);

	base->SR &= ~SR_UIF_BIT; /* reset ovf flag */

	base->CR1 |= CR1_CEN_BIT;

	while ((base->SR & SR_UIF_BIT)==0);
	base->SR &= ~SR_UIF_BIT;
}

