/*! 
*	\file Fan.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header for Fan class.
*/


#ifndef __FAN_H__
#define __FAN_H__

#include "RotationTachymeter.h"
#include "ADCInput.h"
#include "PWMOutput.h"
#include "FanControlProtocol.h"


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
	FanControlProtocol m_fan_control_protocol;
	FanControlMessage m_received_control_message;
	
	bool m_is_in_remote_control_mode;
	

//functions
public:
	Fan(DataLink& data_link);
	~Fan();
	
	void Update();
	
	uint32_t GetFanRevolutionPeriodInMicroseconds();
	uint16_t GetFanSpeedInRoundsPerMinute(); //! Returns the turning speed of the fan in whole rounds per minutes
	void SetSpeed(uint8_t value); //! Set the speed at which the fan should turn as a unitless value between 0 and 255
	uint8_t GetFanSpeedAsSingleByte();
	
	void ReceiveADCValue(uint16_t value); // Override CallbackReceiver (parent) function.

protected:
private:
	

}; //Fan

#endif //__FAN_H__
