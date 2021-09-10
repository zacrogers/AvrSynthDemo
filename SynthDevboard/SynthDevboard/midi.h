/*
 * midi.h
 *
 * Created: 30-Jan-20 12:41:57 AM
 *  Author: Zac
 */ 

#ifndef MIDI_H_
#define MIDI_H_

#ifdef __cplusplus
extern "C" {
#endif

#define A440 (float)664.44 //calibrated for nco, needs to be changed if clock or interrupt routines change
#define A4   (uint8_t)69

static float mtof(uint8_t midi_note);

static float mtof(uint8_t midi_note)
{
	return A440 * pow(2., (((int8_t)midi_note - A4) / 12.)); /* freq = note * 2^(N/12) */
}

#ifdef __cplusplus
}
#endif

#endif /* MIDI_H_ */