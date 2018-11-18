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

	//void (*OnHasNewValue)(uint16_t value);
	/*class Callback
	{
		public:
			Callback(CallbackReceiver* target) : m_target(target) {}//, m_callback(callback_function) {}			
			CallbackReceiver* m_target;

//Callback(void* target, void (*callback_function)(uint16_t value)) : m_target(target), m_callback(callback_function) {}
//void* m_target;
//void (*m_callback)(uint16_t value);
	};
	*/

	//Callback* OnHasNewValue;
	CallbackReceiver* m_callbackTarget;

protected:
private:
	static ADCInput* s_adcInputs[8];
	static const uint8_t PRESCALER_MASK = 0x07;

//functions
public:
	ADCInput(uint8_t adc_input_pin, CallbackReceiver* target);
	~ADCInput();
	
	void SetPrescaler(enum ADCPrescaler prescaler);
	void Start();
	
	friend void ADC_vect(void);
	
	
	
protected:
private:

}; //ADCInput
	
	
#endif //__ADCINPUT_H__
