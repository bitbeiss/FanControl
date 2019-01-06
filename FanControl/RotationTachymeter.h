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
	static const unsigned long PERIOD_WIDTH_MULTIPLIER = 1000000 * 64 / F_CPU; /*! Factor to get from timer counts to microseconds with prescaler 64 */
	static const unsigned long RPM_MULTIPLIER = (1000000 * 60)/(2 * PERIOD_WIDTH_MULTIPLIER); /*! Times 60 for rps to rpm, times 1m for rpus to rps */
	
//variables
public:
	
protected:
private:
	volatile int m_rpm;
	volatile long m_period_width_us;
	static volatile uint16_t m_timer_value;

//functions
public:
	RotationTachymeter(uint8_t interrupt_pin);
	~RotationTachymeter();
	
	long GetRotationPeriodLengthInMicroseconds();
	int GetRotationSpeedInRoundsPerMinute();
	
	friend void INT0_vect(void);
	
protected:
private:
	void initTimers();
	void initInterrupts(uint8_t interrupt_pin);

}; //RotationTachymeter


#endif //__ROTATIONTACHYMETER_H__
