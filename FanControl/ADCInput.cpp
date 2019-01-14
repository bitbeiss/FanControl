/*!
*	\class ADCInput
*	\brief Converts analog signal to digital by using the microcontroller's ADC
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
	adc_input_pin &= 0x07; // data sheet specifies input pins ADC0-7 (numeric 0-7); we are masking the upper bits as a safety measure
	ADCInput::s_adcInputs[adc_input_pin] = this;
	DDRA  &= ~(1 << adc_input_pin);
	ADMUX |= adc_input_pin; 
	ADCSRA |= (1 << ADEN); // enable ADC
	
} //ADCInput

// default destructor
ADCInput::~ADCInput()
{
} //~ADCInput

//! Set the analog/digital conversion mode to interrupt based free-running mode and start the conversion immediately.
void ADCInput::StartFreeRunningConversion()
{
	ADCSRA |= (1 << ADIE); // enable Interrupt
	ADCSRA |= (1 << ADATE); // allow setting ADC to free running mode
	SFIOR &= ~( (1<<ADTS0) | (1<<ADTS1) | (1<<ADTS2) );
	ADCSRA |= (1 << ADSC); // start ADC
}

//! Enable the given prescaler (powers of 2 between 2 and 128 according to data sheet) on the ADC.
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
		ADCInput::m_value = ADCL; // read the lower nibble
		ADCInput::m_value |= (ADCH << 8); // read the upper nibble
		adc->m_callbackTarget->ReceiveADCValue(ADCInput::m_value);
	}
}