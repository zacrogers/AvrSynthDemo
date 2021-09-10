/*
 * adsr.cpp
 *
 * Created: 28-Jan-20 11:45:26 AM
 *  Author: Zac
 */ 
#include "adsr.h"

namespace z_lib
{
	Adsr::Adsr(uint8_t a, uint8_t d, uint8_t s, uint8_t r)
	{
		this->a        = a;
		this->d        = d;
		this->s        = s;
		this->r        = r;
		
		tick           = 0;
		curr_amplitude = MIN_AMPLITUDE;
		curr_state     = State::INIT;
	}
	
	void Adsr::attack(uint8_t a) {this->a = a;}
	
	void Adsr::decay(uint8_t d)  {this->d = d;}
	
	void Adsr::sustain(uint8_t s){this->s = s;}
	
	void Adsr::release(uint8_t r){this->r = r;}
	
	void Adsr::note_on(void)
	{
		curr_state = State::ATTACK;
	}
	
	void Adsr::note_off(void)
	{
		curr_state = State::RELEASE;
	}
	
	float Adsr::get_next(void)
	{
		switch (curr_state)
		{
			case State::INIT:
			{
				curr_amplitude = MIN_AMPLITUDE;
				break;
			}
			
			case State::ATTACK:
			{
				if(curr_amplitude < MAX_AMPLITUDE)
				{
					curr_amplitude+=(MAX_AMPLITUDE/this->a);
				}
				
				if(tick == this->a)
				{
					//curr_amplitude = MAX_AMPLITUDE;
					//curr_state = State::DECAY;
					curr_state = State::INIT;
					tick = 0;
				}
				tick++;
				
				break;
			}
			
			case State::DECAY:
			{
				if(curr_amplitude > this->s)
				{
					curr_amplitude-=(MAX_AMPLITUDE/this->d);
				}
				
				if(tick == this->d)
				{
					//curr_state = State::SUSTAIN;
					curr_state = State::INIT;
					tick = 0;
				}
				tick++;
				
				break;
			}
			
			case State::SUSTAIN:
			{
				curr_amplitude = this->s;
				break;
			}
			
			case State::RELEASE:
			{
				if(curr_amplitude > 0)
				{
					curr_amplitude-=(MAX_AMPLITUDE/this->r);
				}
				
				if(tick == this->r)
				{
					curr_amplitude = MIN_AMPLITUDE;
					curr_state = State::INIT;
					tick = 0;
				}
				tick++;
				
				break;
			}
		}
		return curr_amplitude;
	}
}