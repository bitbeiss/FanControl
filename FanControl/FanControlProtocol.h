/* 
* FanControlProtocol.h
*
* Created: 12.1.19 18:07:56
* Author: filmu
*/


#ifndef __FANCONTROLPROTOCOL_H__
#define __FANCONTROLPROTOCOL_H__

#include "UsartController.h"
#include "FanControlMessage.h"

class FanControlProtocol
{
//variables
public:
	const static int RECEIVE_BUFFER_LEN = 10;
	const static int MAX_BUFFER_SIZE = 10;

protected:
private:
	DataLink& m_data_link;
	char m_data_package[8];
	uint8_t m_current_read_char;
	uint8_t m_receive_buf_len;
	//uint8_t m_protocol_buffer[RECEIVE_BUFFER_LEN];
	
	FanControlMessage m_fan_control_message;
	CircularBuffer* m_receive_buffer;
	

//functions
public:
	FanControlProtocol(DataLink& data_link);
	~FanControlProtocol();
	
	void SendDataMessage(uint8_t fan_speed_percent, uint16_t fan_speed_in_rpm, uint32_t fan_revolution_time);
	bool ReceiveLastControlMessage(FanControlMessage& target_message);
	
	
private:
	bool readConstant(uint8_t value);
	bool readByte(uint8_t& value);
	bool readInt16(uint16_t& value);

}; //FanControlProtocol

#endif //__FANCONTROLPROTOCOL_H__
