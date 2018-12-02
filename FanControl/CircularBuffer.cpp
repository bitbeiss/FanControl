/* 
* CircularBuffer.cpp
*
* Created: 2.12.18 13:39:15
* Author: filmu
*/


#include "CircularBuffer.h"

// default constructor
CircularBuffer::CircularBuffer()
{
} //CircularBuffer

// default destructor
CircularBuffer::~CircularBuffer()
{
} //~CircularBuffer


bool CircularBuffer::Push(uint8_t value)
{
	m_buffer[m_pWrite++] = value;
	
	if (m_pWrite >= BUFFER_SIZE)
		m_pWrite = 0;
	
	if (m_pWrite == m_pRead+1 && m_count > 1) {
		m_pRead = m_pWrite;
	} else {
		m_count++;
	}
}

bool CircularBuffer::Pop(uint8_t* value_ref)
{
	//if (m_pRead == m_pWrite) // buffer is empty
	if (m_count == 0)
		return false;
		
	*value_ref = m_buffer[m_pRead++];
	m_count--;
	
	if (m_pRead >= BUFFER_SIZE)
		m_pRead = 0;
		
	return true;
}

void CircularBuffer::Clear()
{
	for(int i = 0; i < BUFFER_SIZE; ++i) {
		m_buffer[i] = 0;
	}
	m_pRead = m_pWrite = 0;
}
