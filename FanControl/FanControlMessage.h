/* 
* FanControlMessage.h
*
* Created: 12.1.19 19:56:30
* Author: filmu
*/


#ifndef __FANCONTROLMESSAGE_H__
#define __FANCONTROLMESSAGE_H__

#include <avr/io.h>

class FanControlMessage
{
private:
	uint8_t m_mask; // will be 64 for byte, 32 for rpm, 16 for revolution period
	uint16_t m_value;

//functions
public:
	FanControlMessage();
	FanControlMessage(uint8_t mask, uint16_t value);
	~FanControlMessage();
	
	uint8_t GetMessageType();
	uint16_t GetValue();
	
	void Set(uint8_t mask, uint16_t value);
	
	
}; //FanControlMessage

#endif //__FANCONTROLMESSAGE_H__
