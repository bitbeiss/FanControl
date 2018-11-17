/* 
* RotationTachymeter.h
*
* Created: 13.11.18 10:01:47
* Author: filmu
*/


#ifndef __ROTATIONTACHYMETER_H__
#define __ROTATIONTACHYMETER_H__

#include "main.h"


class RotationTachymeter
{
// constants
public:
	static const unsigned long PERIOD_WIDTH_MULTIPLIER = 1000000 * 64 / F_CPU; // factor to get from timer counts to microseconds with prescaler 64
	static const unsigned long RPM_MULTIPLIER = (1000000 * 60)/(2 * PERIOD_WIDTH_MULTIPLIER); // times 60 for rps to rpm, times 1m for rpus to rps
	
//variables
public:
	static volatile int m_timer_value;
	
protected:
private:
	volatile int m_rpm, m_period_width_us;

//functions
public:
	RotationTachymeter();
	~RotationTachymeter();
	
	int GetPeriodLengthMicroseconds();
	int GetRPM();
	
protected:
private:
	void initTimers();
	void initInterrupts();

}; //RotationTachymeter

//volatile int RotationTachymeter::m_timer_value = 0;


#endif //__ROTATIONTACHYMETER_H__
