/* 
* PWMOutput.cpp
*
* Created: 19.11.18 20:53:07
* Author: filmu
*/


#include "Prescalers.h"
#include "PWMOutput.h"

// default constructor
PWMOutput::PWMOutput(uint8_t timer_to_use, PWMPrescaler prescaler)//volatile uint8_t* ddr_addr, uint8_t pin, bool use_16bit_timer)
{
	uint8_t output_pin;
	m_use_8bit_timer = timer_to_use != 1;
	
	switch(timer_to_use) {
		case 0:
			m_ddr_addr = &DDRB;
			output_pin = PB3;
			m_ocr8bit_addr = &OCR0;
			break;
		case 1:
			m_ddr_addr = &DDRD;
			output_pin = PD4; //output_pin = PB5;
			m_ocr16bit_addr = &OCR1A;
			break;
		case 2:
			m_ddr_addr = &DDRD;
			output_pin = PD7;
			m_ocr8bit_addr = &OCR2;
			break;
	}
	
	*m_ddr_addr |= (1 << output_pin); // set correct Port Pin to write mode
	
	// reset duty cycle
	if (m_use_8bit_timer) {
		*m_ocr8bit_addr = 128;
	} else {
		*m_ocr16bit_addr = 32767;
	}
	
	TCCR2 |= (1 << COM21); // set non-inverting mode
	TCCR2 |= (1 << WGM21) | (1 << WGM20); // set fast PWM Mode
	//TCCR2 |= (1 << WGM20); // set phase correct PWM Mode
	//TCCR2 |= (1 << CS21); // set prescaler to 1
	
	uint8_t clock_select_numeric;
	switch(prescaler)
	{
		case PWMPrescalers::_stop:
			clock_select_numeric = 0; break;
		case PWMPrescalers::_1:
			clock_select_numeric = (1 << CS20); break;
		case PWMPrescalers::_8:
			clock_select_numeric = (1 << CS21); break;
		case PWMPrescalers::_32:
			clock_select_numeric = (1 << CS21) | (1 << CS20); break;
		case PWMPrescalers::_64:
			clock_select_numeric = (1 << CS22); break;
		case PWMPrescalers::_128:
			clock_select_numeric = (1 << CS22) | (1 << CS20); break;
		case PWMPrescalers::_256:
			clock_select_numeric = (1 << CS22) | (1 << CS21); break;
		case PWMPrescalers::_1024:
			clock_select_numeric = ((1 << CS22) | (1 << CS21) | (1 << CS20)); break;
	}
	TCCR2 = (TCCR2 & ~PWMOutput::PRESCALER_MASK) | (clock_select_numeric & PWMOutput::PRESCALER_MASK);
} //PWMOutput

// default destructor
PWMOutput::~PWMOutput()
{
} //~PWMOutput


void PWMOutput::SetDutyCyclePercent(float percent) {
	if (percent < 0) percent = 0;
	
	if (m_use_8bit_timer) {
		*m_ocr8bit_addr = (uint8_t) (percent * 2.55);
		} else {
		*m_ocr16bit_addr = (uint16_t) (percent * 655.35);
	}
}