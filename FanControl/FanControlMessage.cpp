/* 
* FanControlMessage.cpp
*
* Created: 12.1.19 19:56:30
* Author: filmu
*/


#include "FanControlMessage.h"

void FanControlMessage::Set(uint8_t mask, uint16_t value)
{
	m_mask = mask;
	m_value = value;
}


// default constructor
FanControlMessage::FanControlMessage()
{} //FanControlMessage
	
	
// default constructor
FanControlMessage::FanControlMessage(uint8_t mask, uint16_t value) : m_mask(mask), m_value(value)
{} //FanControlMessage

// default destructor
FanControlMessage::~FanControlMessage()
{
} //~FanControlMessage

uint8_t FanControlMessage::GetMessageType()
{
	return m_mask;
}

uint16_t FanControlMessage::GetValue()
{
	return m_value;
}
