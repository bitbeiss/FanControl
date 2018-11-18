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

class Fan : public ADCInput::CallbackReceiver
{
//variables
public:
protected:
private:
	RotationTachymeter m_tachymeter;
	uint16_t m_strength;
	ADCInput m_strengthInput;

//functions
public:
	Fan();
	~Fan();
	
	int GetPulseTimeMicroseconds();
	int GetRPM(); // measure RPM permanently, cache value
	void SetStrength(float percent);
	
	void ReceiveADCValue(uint16_t value) override;

protected:
private:
	static void ReceiveUserStrengthInput(uint16_t strength);
	

}; //Fan

#endif //__FAN_H__
