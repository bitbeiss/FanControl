/*! 
* 	\class Fan
* 	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Controls and monitors a physical cooling fan, using ADCInput for user input, PWMOutput for controlling and RotationTachymeter for measuring fan speed
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotationTachymeter.h"
#include "ADCInput.h"
#include "PWMOutput.h"
#include "Prescalers.h"
#include "FanControlMessage.h"
#include "FanControlProtocol.h"
#include "Fan.h"




//! default constructor
Fan::Fan(DataLink& data_link) :
m_tachymeter(INT0),
m_adcInput(PA0, this),
m_fan_control_protocol(data_link),
m_pwmOutput(2, PWMPrescalers::_1) /* pwm output frequency is supposed to be higher than 20k -> 8M/256 = 31.25k, no prescaler required for PWM 
									2 means: 8bit timer used*/
{	
	m_is_in_remote_control_mode = true;
	
	/* We are not recording a signal here, just reading some human input. 
		Therefore response time and time resolution are not essential and
		so the highest prescaler (128) should be sufficient. */
	m_adcInput.SetPrescaler(ADCPrescalers::_128);
	m_adcInput.StartFreeRunningConversion();
} //Fan

//! default destructor
Fan::~Fan()
{
} //~Fan


//! Returns the duration of one fan revolution in microseconds
uint32_t Fan::GetFanRevolutionPeriodInMicroseconds() {
	return m_tachymeter.GetRotationPeriodLengthInMicroseconds();
}

//! Returns the turning speed of the fan in whole rounds per minutes.
uint16_t Fan::GetFanSpeedInRoundsPerMinute() {
	return m_tachymeter.GetRotationSpeedInRoundsPerMinute();
}

//! Set the speed at which the fan should turn as a unitless value between 0 and 255.
void Fan::SetSpeed(uint8_t value) {
	m_strength = value;
	m_pwmOutput.SetOutputValue(value);
}

//! Returns the fan speed as a number between 0 and 255 where 0 corresponds to 0% and 255 to 100% duty cycle.
uint8_t Fan::GetFanSpeedAsSingleByte() {
	return m_strength;
}

//! Callback function receiving the potentiometer value from the ADC and using it to set the fan speed
void Fan::ReceiveADCValue(uint16_t value)
{
	if (m_is_in_remote_control_mode) return;
	return;
	
	// Value coming in is in 10bit, so we divide by 1023 and multiply by 100 to get percent.
	//!< mapping the number from ADC to a number between 0 and 255, as required by function SetStrength.
	this->SetSpeed((uint8_t) (value >> 2));
}

void Fan::Update()
{
	// check if the data link is active, set m_is_in_remote_control_mode accordingly
	
	// if the data link is set, update the data link - read control messages and write data messages
	if (m_fan_control_protocol.ReceiveLastControlMessage(m_received_control_message)) {
		uint8_t mask = m_received_control_message.GetMessageType();
		
		switch(mask) {
			case 64:
			{
				volatile uint8_t value = (uint8_t) m_received_control_message.GetValue();
				//PORTA = value;
				SetSpeed(value);
			}
				break;
			case 32:
				break;
			case 16:
				break;
			default: break;
		}
	}
	
	m_fan_control_protocol.SendDataMessage(
		GetFanSpeedAsSingleByte(),
		GetFanSpeedInRoundsPerMinute(),
		GetFanRevolutionPeriodInMicroseconds());
	
	// otherwise just read the ADC input and set the fan speed accordingly (happens in ReceiveADCValue)
}
