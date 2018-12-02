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
m_adcInput(PA0, this),
m_pwmOutput(2, PWMPrescalers::_1) // pwm output frequency is supposed to be higher than 20k -> 8M/256 = 31.25k, no prescaler required
{	
	// we are not recording a signal here, just reading some human input, therefore response time and time resolution are not essential
	// therefore we are using the highest prescaler
	m_adcInput.SetPrescaler(ADCPrescalers::_128);
	m_adcInput.Start();
} //Fan

// default destructor
Fan::~Fan()
{
} //~Fan


long Fan::GetPulseTimeMicroseconds() {
	return m_tachymeter.GetPeriodLengthMicroseconds();
}
// measure RPM permanently, cache value
int Fan::GetRPM() {
	return m_tachymeter.GetRPM();
}
void Fan::SetStrength(float percent) {
	m_pwmOutput.SetDutyCyclePercent(percent);
}
int Fan::GetDutyCycle() {
	return (int) (m_strength * 100);
}

void Fan::ReceiveADCValue(uint16_t value)
{
	// value coming in is in 10bit, so we divide by 1023 and multiply by 100 to get percent
	m_strength = value / 1023.0;
	this->SetStrength(m_strength * 100);
}
