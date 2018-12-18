/* 
* Fan.h
*
* Created: 13.11.18 09:52:02
* Author: filmu
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
	const static uint8_t ADC_INPUT_DIVIDER = 4;

//functions
public:
	Fan();
	~Fan();
	
	long GetPulseTimeMicroseconds();
	int GetRPM(); // measure RPM permanently, cache value
	void SetStrength(uint8_t value);
	int GetDutyCycle();
	
	void ReceiveADCValue(uint16_t value) override;

protected:
private:
	//static void ReceiveUserStrengthInput(uint16_t strength);
	

}; //Fan

#endif //__FAN_H__
