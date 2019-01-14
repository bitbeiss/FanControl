/* 
* FanControlProtocol.cpp
*
* Created: 12.1.19 18:07:56
* Author: filmu
*/


#include "FanControlProtocol.h"
#include "FanControlMessage.h"

void FanControlProtocol::SendDataMessage(uint8_t fan_speed_byte, uint16_t fan_speed_in_rpm, uint32_t fan_revolution_time)
{
	m_data_package[0] = 0x01; // SOH
	m_data_package[1] = 0x01; // SOH
	m_data_package[2] = 0b01110000; // mask
	m_data_package[3] = fan_speed_byte;
	m_data_package[4] = (uint8_t) (fan_speed_in_rpm >> 8); // upper byte
 	m_data_package[5] = (uint8_t) (fan_speed_in_rpm >> 0); // lower byte
	m_data_package[6] = (uint8_t) (fan_revolution_time >> 8); // upper byte
	m_data_package[7] = (uint8_t) (fan_revolution_time >> 0); // lower byte
	
	m_data_link.Transmit(m_data_package, 8);
}

bool FanControlProtocol::ReceiveLastControlMessage(FanControlMessage& target_message)
{
	// SECTION receive data via serial interface
begin_again:
	
	bool soh_found = false;
	uint8_t peek_index = 0;
	for(peek_index = 0; peek_index < m_receive_buffer->GetLoad(); peek_index++) {
		if (m_receive_buffer->Peek(&m_current_read_char, peek_index)) {
				
			if (m_current_read_char == 0x01) {
				if (soh_found) // we have just found a message start
				{
					break;
				} else {
					soh_found = true;	
				}
			} else {
				soh_found = false;
			}
		} else {
			return false;
		}
	}
		
		
	if (soh_found) {
		if (peek_index > 1) {
			m_receive_buffer->ForwardReadPointer(peek_index-1);
			// read pointer is now on mask byte
		}
		peek_index = 2;
			
		if (m_receive_buffer->Peek(&m_current_read_char, peek_index)) {
			
			uint8_t mask = m_current_read_char & 0b01111111;
				
			switch(mask) {
				case 64:
				{
					if (m_receive_buffer->Peek(&m_current_read_char, ++peek_index)) {
						//PORTA = m_current_read_char;
						target_message.Set(mask, m_current_read_char);
						// done
						m_receive_buffer->ForwardReadPointer(peek_index+1);
						return true;
					}
					else return false;
				}
				case 32: case 16:
				{
					uint16_t value16 = 0;
					if (m_receive_buffer->Peek(&m_current_read_char, ++peek_index)) {
						value16 |= m_current_read_char << 8;
						if (m_receive_buffer->Peek(&m_current_read_char, ++peek_index)) {
							value16 |= m_current_read_char << 0;
							target_message.Set(mask, value16);
							// done
							m_receive_buffer->ForwardReadPointer(peek_index+1);
							return true;
						} else return false;
					}
					else return false;
				}
				default:
					m_receive_buffer->ForwardReadPointer(1);
					goto begin_again;
			}
		}
	} else if (m_receive_buffer->GetLoad() > MAX_BUFFER_SIZE) {
		m_receive_buffer->Clear();
	}
	return false;
}

/*
bool FanControlProtocol::readConstant(uint8_t value) {
	if (m_receive_buffer->Peek(&m_current_read_char) && value == m_current_read_char) {
		return true;
	} else {
		return false;
	}
}

bool FanControlProtocol::readByte(uint8_t& value) {
	if (m_receive_buffer->Peek(&m_current_read_char)) {
		value = m_current_read_char;
		return true;
	} else {
		return false;
	}
}
*/

bool FanControlProtocol::readInt16(uint16_t& value) {
	if (m_receive_buffer->Peek(&m_current_read_char)) {
		value = m_current_read_char;
		// assign to byte 1
		
		return true;
		} else {
		return false;
	}
}


// default constructor
FanControlProtocol::FanControlProtocol(DataLink& data_link) : m_data_link(data_link), m_fan_control_message(0, 0)
{
	m_receive_buffer = m_data_link.GetReceiveBuffer();
} //FanControlProtocol

// default destructor
FanControlProtocol::~FanControlProtocol()
{
} //~FanControlProtocol
