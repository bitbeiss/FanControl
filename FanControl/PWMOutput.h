/*! 
*	\file PWMOutput
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header for the PWMOutput class.
*/


#ifndef __PWMOUTPUT_H__
#define __PWMOUTPUT_H__

#include <avr/io.h>
#include "Prescalers.h"

class PWMOutput
{
//variables
public:
protected:
private:
	static const uint8_t PRESCALER_MASK = ((1 << CS22) | (1 << CS21) | (1 << CS20));
	
	volatile uint8_t* m_ddr_addr;
	volatile uint8_t* m_ocr8bit_addr;
	volatile uint16_t* m_ocr16bit_addr;
	bool m_use_8bit_timer;
	

//functions
public:
	PWMOutput(uint8_t timer_to_use, PWMPrescaler prescaler);
	~PWMOutput();
	
	void SetOutputValue(uint8_t value); // sets the value output as a PWM signal on the pin
protected:
private:

}; //PWMOutput

#endif //__PWMOUTPUT_H__
