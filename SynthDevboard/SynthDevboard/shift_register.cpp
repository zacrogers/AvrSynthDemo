/*
 * shift_register.cpp
 *
 * Created: 03-Feb-20 8:23:02 PM
 *  Author: Zac
 */ 


#include "shift_register.h"

namespace z_lib
{
	ShiftReg::ShiftReg(Gpio* data, Gpio* latch, Gpio* clock, Mode mode)
	{
		this->data  = data;
		this->latch = latch;
		this->clock = clock;
		this->mode  = mode;
	}
	
	ShiftReg::ShiftReg(Gpio* data, Gpio* latch, Gpio* clock)
	{
		this->data  = data;
		this->latch = latch;
		this->clock = clock;
		this->mode  = Mode::POINT;
	}

	void ShiftReg::set_mode(Mode mode)
	{
		this->mode = mode;
	}
	
	void ShiftReg::set_value(uint8_t display_val)
	{
		uint8_t register_val = 0x00;

		if(display_val <= TOTAL_BITS)
		{
			register_val = BAR_MAP[display_val];
		}
		
		latch->clear();
		
#ifdef REGISTER_BACKWARDS

		/* To make leds visible in correct order on devboard*/
		for(int led = TOTAL_BITS; led >= 0; --led)
		{
			clock->clear();

			if((0x01 << display_val) & (0x01 << led))
			{
				data->set();
			}
			else
			{
				data->clear();
			}
			clock->set();
		}
		latch->set();

#else
		
		for(int led = 0; led < TOTAL_BITS; led++)
		{
			clock->clear();
			
			if(this->mode == Mode::BAR)
			{
				if(register_val & (0x01 << led))
				{
					data->set();
				}
				else
				{
					data->clear();
				}
			}
			
			else if(this->mode == Mode::POINT)
			{
				if((0x01 << display_val) & (0x01 << led))
				{
					data->set();
				}
				else
				{
					data->clear();
				}
			}
			clock->set();
		}
		
#endif		

	}
}