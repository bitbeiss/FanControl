/* 
* Fan.cpp
*
* Created: 13.11.18 09:52:02
* Author: filmu
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotationTachymeter.h"
#include "ADCInput.h"
#include "PWMOutput.h"
#include "Prescalers.h"
#include "Fan.h"


// default constructor
Fan::Fan() :
m_tachymeter(INT0),
m_strengthInput(PA0, this),
m_pwmOutput(2, PWMPrescalers::_1) // pwm output frequency is supposed to be higher than 20k -> 8M/256 = 31.25k, no prescaler required
{	
	// we are not recording a signal here, just reading some human input, therefore response time and time resolution are not essential
	// therefore we are using the highest prescaler
	m_strengthInput.SetPrescaler(ADCPrescalers::_128);
	m_strengthInput.Start();
} //Fan

// default destructor
Fan::~Fan()
{
} //~Fan


int Fan::GetPulseTimeMicroseconds() {
	return m_tachymeter.GetPeriodLengthMicroseconds();
}
// measure RPM permanently, cache value
int Fan::GetRPM() {
	return m_tachymeter.GetRPM();
}
void Fan::SetStrength(float percent) {
	m_pwmOutput.SetDutyCyclePercent(percent);
}

void Fan::ReceiveADCValue(uint16_t value)
{
	// value coming in is in 10bit, so we divide by 1024 and multiply by 100 to get percent
	m_strength = value / 1024.0;
	this->SetStrength(m_strength * 100);
}
