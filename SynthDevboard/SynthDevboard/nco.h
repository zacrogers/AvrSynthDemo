/*
 * NCO.h
 * Numerically controlled oscillator
 *
 * Created: 23-Jan-20 10:59:08 PM
 *  Author: Zac
 */ 


#ifndef NCO_H_
#define NCO_H_

#include <math.h>
#include <stdint.h>

#ifndef F_CLK
#define F_CLK (float)5000
#endif

#define _220Hz (float)332.22 // Reference frequency
#define LUT_LENGTH 256

namespace z_lib
{
	enum class Wave
	{
		SINE = 0,
		SQUARE,
		TRIANGLE,
		SAW,
		REV_SAW
	};

	class Nco
	{
		public:
			Nco(Wave wave);
			Nco(uint8_t *wavetable);
			
			uint8_t  next_signal   (void);                /* Get next sample, increment phase */
			void     set_frequency (float freq);          /* Set output frequency of oscillator */
            void     set_waveform  (Wave wave);           /* Set oscillator as basic waveform */
            void     set_wavetable (uint8_t *wavetable);  /* Set oscillator as user wavetable */

		private:
			uint8_t  lut[LUT_LENGTH];
			float    interrupt_period;
			float    phase;                                /* Current position in phase along waveform */
			float    increment;                            /* Amount phase is incremented by each tick */
	};
}




#endif /* NCO_H_ */