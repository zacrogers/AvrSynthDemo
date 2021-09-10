/*
 * timers.c
 *
 * Created: 25-Jan-20 10:43:50 PM
 *  Author: Zac
 */ 
#include "timers.h"


void init_timer0(uint16_t prescaler)
{
	switch(prescaler)
	{
		case 1:
			TCCR0B |= TIM0_PSC_1;
			break;
			
		case 8:
			TCCR0B |= TIM0_PSC_8;
			break;
			
		case 64:
			TCCR0B |= TIM0_PSC_64;
			break;
			
		case 256:
			TCCR0B |= TIM0_PSC_256;
			break;
			
		case 1024:
			TCCR0B |= TIM0_PSC_1024;
			break;
	}
}