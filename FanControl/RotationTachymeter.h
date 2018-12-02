/* 
* RotationTachymeter.h
*
* Created: 13.11.18 10:01:47
* Author: filmu
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
	static const unsigned long PERIOD_WIDTH_MULTIPLIER = 1000000 * 64 / F_CPU; // factor to get from timer counts to microseconds with prescaler 64
	static const unsigned long RPM_MULTIPLIER = (1000000 * 60)/(2 * PERIOD_WIDTH_MULTIPLIER); // times 60 for rps to rpm, times 1m for rpus to rps
	
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
	
	long GetPeriodLengthMicroseconds();
	int GetRPM();
	
	friend void INT0_vect(void);
	
protected:
private:
	void initTimers();
	void initInterrupts(uint8_t interrupt_pin);

}; //RotationTachymeter

//volatile int RotationTachymeter::m_timer_value = 0;


#endif //__ROTATIONTACHYMETER_H__
