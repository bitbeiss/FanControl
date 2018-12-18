/* 
* ADCInput.h
*
* Created: 18.11.18 19:18:47
* Author: filmu
*/


#ifndef __ADCINPUT_H__
#define __ADCINPUT_H__

#include <avr/io.h>
#include "Prescalers.h"

extern "C" void ADC_vect(void) __attribute__ ((signal));

class ADCInput
{
//variables
public:
	class CallbackReceiver
	{
		public:
		virtual void ReceiveADCValue(uint16_t value) = 0;
	};

	CallbackReceiver* m_callbackTarget;

protected:
private:
	volatile static uint16_t m_value;
	static ADCInput* s_adcInputs[8];
	static const uint8_t PRESCALER_MASK = ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));

//functions
public:
	ADCInput(uint8_t adc_input_pin, CallbackReceiver* target);
	~ADCInput();
	
	void SetPrescaler(ADCPrescaler prescaler);
	void Start();
	
protected:
private:
	friend void ADC_vect(void);

public:
	const static uint16_t MAX_VALUE = 1023;
}; //ADCInput
	
	
#endif //__ADCINPUT_H__
