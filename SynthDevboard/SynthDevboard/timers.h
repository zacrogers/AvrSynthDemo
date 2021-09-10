/*
 * timers.h
 *
 * Created: 21-Jan-20 9:24:24 PM
 *  Author: Zac
 */ 

#ifndef TIMERS_H_
#define TIMERS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <avr/io.h>
#include <avr/interrupt.h> 


/* Timer 0 Prescaler Macros */
#define TIM0_PSC_1    (1 << CS00)
#define TIM0_PSC_8    (1 << CS01)
#define TIM0_PSC_64   (1 << CS01) | (1 << CS00)
#define TIM0_PSC_256  (1 << CS02)
#define TIM0_PSC_1024 (1 << CS10) | (1 << CS12)

#define TIM0A_CTC     (1 << WGM01)


/* Timer 1 Prescaler Macros */
#define TIM1_PSC_1    (1 << CS10)
#define TIM1_PSC_8    (1 << CS11)
#define TIM1_PSC_64   (1 << CS11) | (1 << CS10)
#define TIM1_PSC_256  (1 << CS12)
#define TIM1_PSC_1024 (1 << CS10) | (1 << CS12)

/* Timer 2 Prescaler Macros */
#define TIM2_PSC_1    (1 << CS20)
#define TIM2_PSC_8    (1 << CS21)
#define TIM2_PSC_32   (1 << CS21) | (1 << CS20)
#define TIM2_PSC_64   (1 << CS22) 
#define TIM2_PSC_128  (1 << CS22) | (1 << CS20)
#define TIM2_PSC_256  (1 << CS22) | (1 << CS21)
#define TIM2_PSC_1024 (1 << CS22) | (1 << CS21) | (1 << CS20)


/* Function prototypes */
void init_timer0(uint16_t prescaler);
void init_timer1(void);
void init_timer2(void);

#ifdef __cplusplus
}
#endif


#endif /* TIMERS_H_ */