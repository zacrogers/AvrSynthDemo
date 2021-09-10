/*
 * seven_segment.cpp
 *
 * 7 segment display driver using 74hc595 shift register
 *
 * Created: 21-Jan-20 10:22:20 AM
 *  Author: Zac
 */ 

#include "seven_segment.h"

namespace z_lib
{
	SevenSegment::SevenSegment(Gpio* data, Gpio* clk, Gpio* latch)
	{
		this->data = data;
		this->clk = clk;
		this->latch = latch;
	}
	
	void SevenSegment::set_num(uint8_t num)
	{
		uint8_t digit = 0;
		
		if(num < NUM_DIGITS)
		{
			digit = DIGITS[num];
		}
		write_to_disp(digit);
	}

	void SevenSegment::set_letter(char letter)
	{
		write_to_disp(LETTERS[get_letter(letter)]);
	}
	
	void SevenSegment::write_to_disp(uint8_t character)
	{
		this->latch->clear();

		for(uint8_t segment = 0; segment < SEGMENTS; ++segment)
		{
			this->clk->clear();
			
			if(character & (0x01 << segment))
			{
				this->data->clear();
			}
			else
			{
				this->data->set();
			}
			this->clk->set();
		}
		this->latch->set();
	}
	
	/* Get index of letter in ALPHABET to get display pattern in LETTERS */
	uint8_t SevenSegment::get_letter(const char letter)
	{
		uint8_t letter_index = 0;
	    
		for(uint8_t let = 0; let < NUM_LETTERS; ++let)
		{
			if(is_upper(&letter))
			{
				if(ALPHABET[let] == letter)
				{
					letter_index = let;
				}
			}
			else
			{
				if(ALPHABET[let] == lower_to_upper(&letter))
				{
					letter_index = let;
				}
			}
		}
		return letter_index;
	}

	bool SevenSegment::is_upper(const char* letter)
	{
		bool ret_val = false;
	    
		if(letter)
		{
			ret_val = ((*letter < 91) ? true : false); /* Max upper case letter in ascii table is Z at 90 */
		}
		return ret_val;
	}

	char SevenSegment::lower_to_upper(const char* lower)
	{
		char upper = 0;
	    
		if(lower)
		{
			upper = (*lower - ASCII_OFFSET);
		}
		return upper;
	}	
}