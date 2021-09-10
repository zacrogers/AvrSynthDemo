/*
 * nco.cpp
 *
 * Created: 26-Jan-20 6:11:26 PM
 *  Author: Zac
 */ 
#include "nco.h"

namespace z_lib
{
	Nco::Nco(Wave wave)
	{
		interrupt_period = (1.0 / F_CLK);
		phase            = 0.0;
		increment        = 0.0;
		
		set_waveform(wave);
	}

	uint8_t Nco::next_signal(void)
	{
		phase += increment;

		while(phase >= LUT_LENGTH)
		{
			phase -= LUT_LENGTH;
		}
		
		return lut[(uint16_t)phase];
	}

	void Nco::set_frequency(float freq)
	{
		increment = LUT_LENGTH * freq * interrupt_period;
	}
	
	void Nco::set_waveform(Wave wave)
	{
		switch(wave)
		{
			case Wave::SINE:
				for(int n = 0; n < LUT_LENGTH; ++n)
				{
					lut[n] = 255 * ((sin(2. * M_PI * n / LUT_LENGTH) + 1) / 2);
				}
				break;

			case Wave::SQUARE:
				for(int n = 0; n < LUT_LENGTH; ++n)
				{
					if(n < (LUT_LENGTH / 2))
					{
						lut[n] = 127;
					}
					else
					{
						lut[n] = 0;
					}
				}
				
				break;

			case Wave::TRIANGLE:
				break;

			case Wave::SAW:
				for(int n = 0; n < LUT_LENGTH; ++n)
				{
					lut[n] = n;
				}
				break;

			case Wave::REV_SAW:
				for(int n = LUT_LENGTH; n > 0; --n)
				{
					lut[n] = n;
				}
				break;
		}
	}
	void Nco::set_wavetable (uint8_t *wavetable)
	{
        for(int n = 0; n < LUT_LENGTH; ++n)
        {
	        lut[n] = wavetable[n];
        }		
	}
}