/* 
* 	\class Fan
* 	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief The Fan class contains functions and data directly linked to the fan. (Except for period and speed related calculus which is found in the more general RotationTachymeter class.)
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
m_pwmOutput(2, PWMPrescalers::_1) /*!< pwm output frequency is supposed to be higher than 20k -> 8M/256 = 31.25k, no prescaler required */
{	
	/*! We are not recording a signal here, just reading some human input. 
		Therefore response time and time resolution are not essential and
		so we are using the highest prescaler (128). */
	m_adcInput.SetPrescaler(ADCPrescalers::_128);
	m_adcInput.StartFreeRunningConversion();
} //Fan

// default destructor
Fan::~Fan()
{
} //~Fan


long Fan::GetFanRevolutionPeriodInMicroseconds() {
	return m_tachymeter.GetRotationPeriodLengthInMicroseconds();
}

//! Returns the turning speed of the fan in whole rounds per minutes.
int Fan::GetFanSpeedInRoundsPerMinute() {
	return m_tachymeter.GetRotationSpeedInRoundsPerMinute();
}

//! Set the speed at which the fan should turn as a unitless value between 0 and 255.
void Fan::SetSpeed(uint8_t value) {
	m_pwmOutput.SetOutputValue(value);
}

//! Returns the fan speed as a number between 0 and 255 where 0 corresponds to 0% and 255 to 100% duty cycle.
uint8_t Fan::GetFanSpeedAsSingleByte() {
	return m_strength;
}

//! Callback function receiving the potentiometer value from the ADC and using it to set the fan speed
void Fan::ReceiveADCValue(uint16_t value)
{
	// Value coming in is in 10bit, so we divide by 1023 and multiply by 100 to get percent.
	m_strength = (uint8_t) (value >> 2); //!< mapping the number from ADC to a number between 0 and 255, as required by function SetStrength.
	this->SetSpeed(m_strength);
}
