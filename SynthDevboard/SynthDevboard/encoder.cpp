/*
 * encoder.cpp
 *
 * Created: 31-Jan-20 4:03:27 PM
 *  Author: Zac
 */ 

#include "encoder.h"

namespace z_lib
{
	Encoder::Encoder(Gpio *pin1, Gpio *pin2, uint8_t min, uint8_t max)
	{
		this->pin1 = pin1;
		this->pin2 = pin2;
		
		this->min_val = min;
		this->max_val = max;
		
		this->count = 0;
		
		this ->gray_val = read_gray_code();
	}
	
	uint8_t Encoder::read_gray_code(void)
	{
		uint8_t val = 0;
		
		if (pin1->read())
		{
			val |= (1 << 1);
		}
		if (pin2->read())
		{
			val |= (1 << 0);
		}
		return val;
	}
	
	uint8_t Encoder::read()
	{
		gray_val_tmp = read_gray_code();
		
		if(gray_val != gray_val_tmp)
		{
			if((gray_val==3 && gray_val_tmp==1) || (gray_val==0 && gray_val_tmp==2))
			{
				if(count < this->max_val)
				{
					count++;
				}
			}
			else if((gray_val==2 && gray_val_tmp==0) || (gray_val==1 && gray_val_tmp==3))
			{
				if(count > this->min_val)
				{
					count--;
				}
			}

			gray_val = gray_val_tmp;
		}		
		
		return count;			
	}
}