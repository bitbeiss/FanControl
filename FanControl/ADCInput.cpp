/* 
* ADCInput.cpp
*
* Created: 18.11.18 19:18:47
* Author: filmu
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ADCInput.h"
#include "Prescalers.h"


ADCInput* ADCInput::s_adcInputs[8];

// default constructor
ADCInput::ADCInput(uint8_t adc_input_pin, CallbackReceiver* target) : m_callbackTarget(target)
{
	ADCInput::s_adcInputs[adc_input_pin] = this;
	DDRA  &= ~(1 << adc_input_pin);
	ADCSRA |= (1 << ADEN); // enable ADC
	
} //ADCInput

// default destructor
ADCInput::~ADCInput()
{
} //~ADCInput


void ADCInput::Start()
{
	ADCSRA |= (1 << ADIE); // enable Interrupt
	ADCSRA |= (1 << ADATE); // allow setting ADC to free running mode
	SFIOR &= ~( (1<<ADTS0) | (1<<ADTS1) | (1<<ADTS2) );
	ADCSRA |= (1 << ADSC); // start ADC
}

void ADCInput::SetPrescaler(enum ADCPrescaler prescaler)
{
	uint8_t numeric_value = 0;
	
	switch(prescaler) {
		case _2:
		break;
		case _4:
		numeric_value = 2; break;
		case _8:
		numeric_value = 3; break;
		case _16:
		numeric_value = 4; break;
		case _32:
		numeric_value = 5; break;
		case _64:
		numeric_value = 6; break;
		case _128:
		numeric_value = 7; break;
	}
	
	ADCSRA = (ADCSRA & ~ADCInput::PRESCALER_MASK) | (numeric_value & ADCInput::PRESCALER_MASK);
}

void ADC_vect(void) {
	uint8_t trigger_pin = ADMUX & ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

	ADCInput* adc = ADCInput::s_adcInputs[trigger_pin];
	if (adc != 0) {
		adc->m_callbackTarget->ReceiveADCValue((ADCH << 8) | ADCL);
	}
	//ADCSRA |= (1 << ADSC);		// Start Conversion
}