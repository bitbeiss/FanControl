/*! 
*	\class CircularBuffer
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	\brief Implementation of a circular buffer as a class.
*/


#include "CircularBuffer.h"


// default constructor
CircularBuffer::CircularBuffer()
{
	this->clear(); //! Initialize buffer.
} //CircularBuffer


// default destructor
CircularBuffer::~CircularBuffer()
{
} //~CircularBuffer

//! Push a value into the buffer.
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

//! Pull a value out of the buffer.
bool CircularBuffer::Pop(uint8_t* value_ref)
{
	//if (m_pRead == m_pWrite) // buffer is empty
	if (m_count == 0)
		return false;
		
	*value_ref = m_buffer[m_pRead++];
	m_count--;
	
	if (m_pRead >= CircularBuffer::BUFFER_SIZE)
		m_pRead = 0;
		
	return true;
}

//! Clear and/or initialize the buffer.
void CircularBuffer::Clear()
{
	for(int i = 0; i < BUFFER_SIZE; ++i) {
		m_buffer[i] = 0;
	}
	m_pRead = m_pWrite = 0;
}

//! Obtain number of currently loaded values within the buffer.
int CircularBuffer::GetLoad()
{
	return m_count;
}
