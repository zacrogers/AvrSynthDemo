/*
 * shift_register.h
 *
 * Created: 03-Feb-20 7:51:48 PM
 *  Author: Zac
 */ 


#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#define TOTAL_BITS 8
#define REGISTER_BACKWARDS // Uncomment if leds attached to shift register are upside down 

#include <stdint.h>
#include "gpio.h"

namespace z_lib
{
	class ShiftReg
	{
		public:
		   /* Display mode for leds.
			BAR   = solid bar of leds
			POINT = single led on at a time 
		    */
			enum class Mode
			{
				BAR = 0,
				POINT    
			};	

			ShiftReg(Gpio* data, Gpio* latch, Gpio* clock, Mode mode);
			ShiftReg(Gpio* data, Gpio* latch, Gpio* clock);
			
			void set_mode(Mode mode);
			void set_value(uint8_t display_val);
       
		private:
			Gpio*  data;
			Gpio*  latch;
			Gpio*  clock;    
			Mode  mode;

			//const uint16_t BAR_MAP[9] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
			const uint16_t BAR_MAP[9] = {0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00};
	};
}

#endif /* SHIFT_REGISTER_H_ */