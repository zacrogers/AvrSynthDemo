/*
 * encoder.h
 *
 * Created: 31-Jan-20 3:21:30 PM
 *  Author: Zac
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

#include "gpio.h"

namespace z_lib
{
	class Encoder
	{
		public:
			Encoder(Gpio *pin1, Gpio *pin2, uint8_t min, uint8_t max);
			
			uint8_t read();
			uint8_t read_gray_code(void);
			
		private:	
			Gpio *pin1;
			Gpio *pin2;
			
			uint8_t min_val;
			uint8_t max_val;
		
			uint8_t count;
			uint8_t gray_val;
			uint8_t gray_val_tmp;		
	};
}

#endif /* ENCODER_H_ */