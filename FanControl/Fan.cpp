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
#include "Prescalers.h"
#include "Fan.h"


// default constructor
Fan::Fan() : m_tachymeter(INT0), m_strengthInput(PA0, this)//, m_adc_callback(this)
{	
	// we are not recording a signal here, just reading some human input, therefore response time and time resolution are not essential
	m_strengthInput.SetPrescaler(_128);
	//m_adc_callback = ADCInput::Callback(this, (&(Fan::ReceiveUserStrengthInput)));
	//m_strengthInput.OnHasNewValue = &m_adc_callback;
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
	//m_pwmOutput.SetValue(...)
	return;
}

void Fan::ReceiveADCValue(uint16_t value)
{
	m_strength = value;
}
