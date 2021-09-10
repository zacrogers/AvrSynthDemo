/*
 * mux.h
 *
 * Created: 01-Feb-20 9:26:48 PM
 *  Author: Zac
 */ 


#ifndef MUX_H_
#define MUX_H_

#include <stdint.h>
#include "gpio.h"

namespace z_lib
{
	class Mux
	{
		public:	
			Mux(Gpio* s0, Gpio* s1, Gpio* s2, Gpio* com)
			{
				select_pins[0] = s0;	
				select_pins[1] = s1;	
				select_pins[2] = s2;	
				
				this->com = com;
			}
			
			uint8_t read(uint8_t channel)
			{
				set_channel(channel);
				return com->read();
			}
			
		
		private:
			Gpio* select_pins[3];
			Gpio* com;
			
			void set_channel(uint8_t channel)
			{
				for(uint8_t pin = 0; pin < 3; ++pin)
				{
					if(channel & (1 << pin))
					{
						select_pins[pin]->set();
					}
					else
					{
						select_pins[pin]->clear();
					}
				}
			}
	};
	
}


#endif /* MUX_H_ */