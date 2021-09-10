/*
* seven_segment.h
*
* 7 segment display driver using 74hc595 shift register
*
* Created: 21-Jan-20 9:51:47 AM
*  Author: Zac
*/ 
#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

#include <stdint.h>
#include "gpio.h"

/* 
*  Lower case letters are offset from upper case in ascii table by  
*  32 so this is used for converting case
*/
#define ASCII_OFFSET 32
#define SEGMENTS     7
#define NUM_DIGITS   10
#define NUM_LETTERS  20

namespace z_lib
{
	static const uint8_t DIGITS[NUM_DIGITS] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 
											   0x5B, 0x5F, 0x70, 0xFF, 0x7B};
                            
											   /* A     b     C     d     E     F */ 
	static const uint8_t LETTERS[NUM_LETTERS] = {0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47,
											   /* G     H     i     J     L     n */
												 0x5E, 0x37, 0x06, 0x3C, 0x0E, 0x15,
											   /* o     p     q     r     S     t */
												 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F,
											   /* U     y  */
												 0x1C, 0x3B};          
                                            
	/* Alphabet for getting letters - upper case */
	static const char ALPHABET[NUM_LETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
											   'I', 'J', 'L', 'N', 'O', 'P', 'Q', 'R', 
											   'S', 'T', 'U', 'Y'};

	class SevenSegment
	{
		public:
			SevenSegment(Gpio* data, Gpio* clk, Gpio* latch);

			void set_num    (uint8_t num);
			void set_letter (char letter);

		private:
			Gpio* data;
			Gpio* clk;
			Gpio* latch;

			void           write_to_disp  (uint8_t character);  /* Write character to display */
			static uint8_t get_letter     (const char letter);  /* Get index of letter in ALPHABET[] */
			static bool    is_upper       (const char* letter); /* Check case of ascii char */
			static char    lower_to_upper (const char* lower);  /* Convert case of ascii char */
	};
}

#endif /* SEVEN_SEGMENT_H_ */