/*! \class ADCInput
*	\brief Implementation of ADCInput class.
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ADCInput.h"
#include "Prescalers.h"


ADCInput* ADCInput::s_adcInputs[8];
volatile uint16_t ADCInput::m_value;

// default constructor
ADCInput::ADCInput(uint8_t adc_input_pin, CallbackReceiver* target) : m_callbackTarget(target)
{
	ADCInput::s_adcInputs[adc_input_pin] = this;
	DDRA  &= ~(1 << adc_input_pin);
	ADMUX |= (adc_input_pin & 0x07); //!< Choose the input pin to connect to the ADC. 0x07 is the hexcode for pin 8.
	ADCSRA |= (1 << ADEN); // enable ADC
	
} //ADCInput

// default destructor
ADCInput::~ADCInput()
{
} //~ADCInput

//! Set the analog/digital conversion mode to interrupt based free-running mode.
void ADCInput::StartFreeRunningConversion()
{
	ADCSRA |= (1 << ADIE); // enable Interrupt
	ADCSRA |= (1 << ADATE); // allow setting ADC to free running mode
	SFIOR &= ~( (1<<ADTS0) | (1<<ADTS1) | (1<<ADTS2) );
	ADCSRA |= (1 << ADSC); // start ADC
}

//! Assign the appropriate numeric values (used to set the bits) for various prescaler-deviders.
void ADCInput::SetPrescaler(ADCPrescaler prescaler)
{
	uint8_t numeric_value = 0;
	
	switch(prescaler) {
		case ADCPrescalers::_2:
		break;
		case ADCPrescalers::_4:
		numeric_value = 2; break;
		case ADCPrescalers::_8:
		numeric_value = 3; break;
		case ADCPrescalers::_16:
		numeric_value = 4; break;
		case ADCPrescalers::_32:
		numeric_value = 5; break;
		case ADCPrescalers::_64:
		numeric_value = 6; break;
		case ADCPrescalers::_128:
		numeric_value = 7; break;
	}
	
	ADCSRA = (ADCSRA & ~ADCInput::PRESCALER_MASK) | ( (numeric_value << 0) & ADCInput::PRESCALER_MASK );
}

//! Read the converted 10bit representation of the processed analog input.
void ADC_vect(void) {
	uint8_t trigger_pin = ADMUX & ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

	ADCInput* adc = ADCInput::s_adcInputs[trigger_pin];
	if (adc != 0) {
		ADCInput::m_value = ADCL;
		ADCInput::m_value |= (ADCH << 8);
		adc->m_callbackTarget->ReceiveADCValue(ADCInput::m_value);
	}
}