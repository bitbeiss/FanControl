/*! \file RotationTachymeter.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header for RotationTachymeter class.
*/


#ifndef __ROTATIONTACHYMETER_H__
#define __ROTATIONTACHYMETER_H__

#include <avr/io.h>
#include "main.h"

extern "C" void INT0_vect(void) __attribute__ ((signal));

class RotationTachymeter
{
// constants
public:
	/*! Prescaler to use for measuring the duration between input pulses */
	static const uint8_t PRESCALER = 64;
	
	/*! Factor to get from timer counts to microseconds with prescaler 64 */
	static const uint16_t PERIOD_WIDTH_MULTIPLIER = 1000000 * PRESCALER / F_CPU;
	
	/*! Times 60 for rps to rpm, times 1M for rpus to rps */
	static const uint32_t RPM_MULTIPLIER = (1000000 * 60)/(2 * PERIOD_WIDTH_MULTIPLIER);
	
//variables
public:
	
protected:
private:
	volatile uint16_t m_rpm;
	volatile uint16_t m_period_width_us;
	static volatile uint16_t m_timer_value;

//functions
public:
	RotationTachymeter(uint8_t interrupt_pin);
	~RotationTachymeter();
	
	uint16_t GetRotationPeriodLengthInMicroseconds();
	uint16_t GetRotationSpeedInRoundsPerMinute();
	
	friend void INT0_vect(void);
	
protected:
private:
	void initTimers();
	void initInterrupt(uint8_t interrupt_pin);

}; //RotationTachymeter


#endif //__ROTATIONTACHYMETER_H__
