/*! \class Fan
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief
*/


#ifndef __FAN_H__
#define __FAN_H__

#include "RotationTachymeter.h"
#include "ADCInput.h"
#include "PWMOutput.h"

class Fan : public ADCInput::CallbackReceiver
{
//variables
public:
protected:
private:
	RotationTachymeter m_tachymeter;
	uint8_t m_strength;
	ADCInput m_adcInput;
	PWMOutput m_pwmOutput;
	
	//const static uint8_t ADC_INPUT_DIVIDER = ADCInput::MAX_VALUE >> 2;
	//const static uint8_t ADC_INPUT_DIVIDER = 4;

//functions
public:
	Fan();
	~Fan();
	
	long GetFanRevolutionPeriodInMicroseconds();
	int GetFanSpeedInRoundsPerMinute(); // returns the turning speed of the fan in whole rounds per minutes
	void SetSpeed(uint8_t value); // set the speed at which the fan should turn as a unitless value between 0 and 255
	uint8_t GetFanSpeedAsSingleByte();
	
	void ReceiveADCValue(uint16_t value); // override

protected:
private:
	//static void ReceiveUserStrengthInput(uint16_t strength);
	

}; //Fan

#endif //__FAN_H__
