/*
 * adsr.h
 *
 * Created: 28-Jan-20 11:43:55 AM
 *  Author: Zac
 */ 


#ifndef ADSR_H_
#define ADSR_H_

#include <stdint.h>

#define MIN_AMPLITUDE (float)0.0
#define MAX_AMPLITUDE (float)1.0

namespace z_lib
{
	class Adsr
	{
		public:
			Adsr(uint8_t a, uint8_t d, uint8_t s, uint8_t r);
		
			/* Value setters */
			void    attack   (uint8_t a);
			void    decay    (uint8_t d);
			void    sustain  (uint8_t s);
			void    release  (uint8_t r);
				
			/* Control methods */
			void    note_on  (void);
			void    note_off (void);
			float   get_next (void); /* Increment tick, get next amplitude value */
		
		private:
			enum class State
			{
				INIT = 0,
				ATTACK,
				DECAY,
				SUSTAIN,
				RELEASE
			};
		
			uint8_t a;  /* Time */
			uint8_t d;  /* Time */
			uint8_t s;  /* Amplitude */
			uint8_t r;  /* Time */
		
			uint8_t tick;
			uint8_t curr_amplitude;
			State   curr_state;
		
	};
}




#endif /* ADSR_H_ */