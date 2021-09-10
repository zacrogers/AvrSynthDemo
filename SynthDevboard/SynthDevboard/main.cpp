/*
 * SynthDevboard.cpp
 *
 * Created: 21-Jan-20 9:49:29 AM
 * Author : Zac
 */ 

#include "main.h"

/* Namespace includes */
using z_lib::Gpio;
using z_lib::SevenSegment;
using z_lib::Encoder;
using z_lib::ShiftReg;

using z_lib::Nco;
using z_lib::Wave;
using z_lib::Adsr;

/* Gpio pin setup */
Gpio enc_pins[2]     = {Gpio(&PORTD, PIND3, Gpio::Mode::IN), Gpio(&PORTD, PIND4, Gpio::Mode::IN)};
Gpio enc_btn         = Gpio(&PORTB, PINB0, Gpio::Mode::IN);

Gpio pot_mux_sel[2]  = {Gpio(&PORTC, PINC1, Gpio::Mode::OUT), Gpio(&PORTC, PINC0, Gpio::Mode::OUT)};
	
Gpio btn_mux_sel[3]  = {Gpio(&PORTB, PINB2, Gpio::Mode::OUT), 
	                    Gpio(&PORTB, PINB3, Gpio::Mode::OUT),
			      	    Gpio(&PORTB, PINB4, Gpio::Mode::OUT)};

Gpio btn_mux_in      = Gpio(&PORTB, PINB1, Gpio::Mode::IN);

Gpio sev_seg_gpio[3] = {Gpio(&PORTC, PINC3, Gpio::Mode::OUT),
	                    Gpio(&PORTC, PINC2, Gpio::Mode::OUT),
				        Gpio(&PORTB, PINB5, Gpio::Mode::OUT)};

Gpio sr_pins[3]      = {Gpio(&PORTD, PIND5, Gpio::Mode::OUT),
	                    Gpio(&PORTD, PIND6, Gpio::Mode::OUT),
                        Gpio(&PORTD, PIND7, Gpio::Mode::OUT)};
	
/* Peripheral setup */			   
ShiftReg      shift_reg  = ShiftReg(&sr_pins[0], &sr_pins[1], &sr_pins[2]);
SevenSegment  sev_seg    = SevenSegment(&sev_seg_gpio[0], &sev_seg_gpio[1], &sev_seg_gpio[2]);
Encoder       enc        = Encoder(&enc_pins[0], &enc_pins[1], 0, 7);

Nco           oscillator = Nco(Wave::SINE);
Adsr          envelope   = Adsr(50,200,200,200);

/* Global variables */
#define A220 (float)332.22 // need to adjust when ISRs are changed, main clock is messed with

uint8_t octave = 2;
volatile float env_amp = 0.0;
volatile float ampl = 1.0;

int midi_seq[] = {69, 71, 72, 74, 69, 71, 72, 74};	
	
int A_MINOR[] = {69, 71, 72, 74, 76, 77, 79};
	
int pot_vals[NUM_POTS]    = {0, 0, 0, 0};
int btn_vals[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0};
	
int new_freq = 0;

/* Sequencer variables */
volatile int del_time     = 500;
volatile int del_time_new = 500;
volatile int del_tick     = 0;

volatile int seq_step     = 0;
volatile int seq_len      = 8;

bool isrflag1 = false;
bool note_on  = false;
bool led_bar  = false;

volatile uint8_t encoder_val = 57;

PlayState play_state = PlayState::PLAYING;

int main(void)
{
	setup_adc();
	
	setup_timer0();             // Oscillator clock timer
	setup_timer1();             // Sequencer tempo timer
	setup_timer2();             // Controls timer

	sei();                      // Enable interrupts
	
	mcp4725_init(MCP4725_ADDR); // Init DAC
	
    while (1) 
    {			
		/* Poll potentiometers */
		for(uint8_t pot = 0; pot < NUM_POTS; ++pot)
		{
			pot_vals[pot] = pot_mux_read(pot);
		}
		
		del_time_new = map(pot_vals[0], 0, 255, 100, 1000);
		ampl = map(pot_vals[1], 0, 255, 1, 10);
		
		/* Poll buttons if sequencer stopped */
		if(play_state == PlayState::STOPPED)
		{	
			for(uint8_t btn = 0; btn < NUM_BUTTONS; ++btn)
			{
				if(btn_mux_read(btn) != 0)
				{
					midi_seq[btn] = new_freq;
				}
			}
		}
    }
}

/********************************
******* Interrupt routines ******
*********************************/

volatile int osc_val = 0;

#define DISABLE_TIM0 (TCCR0B = 0)
#define ENABLE_TIM0  (TCCR0B |= TIM0_PSC_64)

#define DISABLE_TIM1 (TCCR1B = 0)
#define ENABLE_TIM1  (TCCR1B |= TIM1_PSC_64)

/* Oscillator clock */
ISR(TIMER0_COMPA_vect)
{
	mcp4725_write(oscillator.next_signal() / ampl);
}

/* Sequencer tempo */
ISR(TIMER1_COMPA_vect)
{
	TCNT1L = 0;
	if(del_tick == del_time)
	{	
		if(seq_step == seq_len)
		{
			seq_step = 0;
		}
		
		sev_seg.set_num(seq_step + 1);
		oscillator.set_frequency(mtof(midi_seq[seq_step]) / octave);
		
		seq_step++;
		del_tick = 0;
		del_time = del_time_new; /* Has to be inside isr to prevent hanging  */
	}
	del_tick++;
}

ISR(TIMER2_COMPA_vect)
{
	encoder_val = enc.read();
	
	if (play_state == PlayState::PLAYING && enc_btn.read())
	{
		play_state = PlayState::STOPPED;
		DISABLE_TIM0;
		DISABLE_TIM1;
	}
	
	else if (play_state == PlayState::STOPPED && enc_btn.read())
	{
		play_state = PlayState::PLAYING;
		ENABLE_TIM0;
		ENABLE_TIM1;
	}	
	
	new_freq = A_MINOR[map(encoder_val, 0, 7, 7, 0)];
	shift_reg.set_value(map(encoder_val, 0, 7, 7, 0));
}

/*************************
*** Timer initializers ***
**************************/

/* Oscillator clock timer (5kHz) */
void setup_timer0(void)
{
	TCCR0A = 0;
	TCCR0A |= (1 << WGM01);  // Set as ctc 
	TCCR0B |= TIM0_PSC_64;   // Set prescaler
	OCR0A  |= 50;            // Set compare value
	TIMSK0 |= (1 << OCIE0A); // Enable match & compare A
}

/* Sequencer tempo timer */
void setup_timer1(void)
{
	OCR1AL = 250;           // Set compare value
	TCCR1A = (1 << WGM12);  // Set as ctc 
	TCCR1B = TIM1_PSC_64;   // Set prescaler
	TIMSK1 = (1 << OCIE1A);	// Enable match & compare A
}

/* Controls timer */
void setup_timer2(void)
{
	OCR2A = 150;            // Set compare value
	TCCR2A |= (1 << WGM22); // Set as ctc 
	TCCR2B |= TIM2_PSC_256; // Set prescaler
	TIMSK2 = (1 << OCIE2A);	// Enable match & compare A
}


#define ADC_CHAN_7 (1 << MUX0) |(1 << MUX1) | (1 << MUX2)

/* ADC prescaler macros */
#define ADC_PSC_2   (1 << ADPS0)
#define ADC_PSC_4   (1 << ADPS1) 
#define ADC_PSC_8   (1 << ADPS0) | (1 << ADPS1)
#define ADC_PSC_16  (1 << ADPS2)
#define ADC_PSC_32  (1 << ADPS0) | (1 << ADPS2)
#define ADC_PSC_64  (1 << ADPS1) | (1 << ADPS2)
#define ADC_PSC_128 (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)

/* using interrupts breaks _delay_ms so this is for testing */
void dirty_del()
{
	volatile int i = 0;
	while (++i < 32500);
}

void setup_adc(void)
{
	ADMUX |= 7 | (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | ADC_PSC_128;
}

uint8_t adc_read (uint8_t chan)
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR) | (chan & 0x0F);
	ADCSRA |= (1 << ADSC) ;
	
	while (ADCSRA & (1 << ADSC)); // Wait for read completion
	
	return ADCH;	
}

uint8_t btn_mux_read(uint8_t chan)
{
	/* Set multiplexer channel */
	for(uint8_t pin = 0; pin < NUM_BUTTONS; ++pin)
	{
		if(chan & (1 << pin))
		{
			btn_mux_sel[pin].set();
		}
		else
		{
			btn_mux_sel[pin].clear();
		}
	}
	
	return btn_mux_in.read();
}


uint8_t pot_mux_read(uint8_t chan)
{
	/* Set multiplexer channel */
	for(uint8_t pin = 0; pin < NUM_POTS; ++pin)
	{
		if(chan & (1 << pin))
		{
			pot_mux_sel[pin].set();
		}
		else
		{
			pot_mux_sel[pin].clear();
		}
	}
	
	return adc_read(7);	
}
