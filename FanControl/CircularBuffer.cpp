/*! 
*	\class CircularBuffer
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	\brief Implementation of a circular buffer as a class.
*/


#include "CircularBuffer.h"


//! Initialize the buffer
CircularBuffer::CircularBuffer()
{
	this->Clear(); 
} //CircularBuffer


//! default destructor
CircularBuffer::~CircularBuffer()
{
} //~CircularBuffer

//! Adds a new element at the end of the buffer
bool CircularBuffer::Push(char value)
{
	// if buffer is full, do not push
	if (m_pWrite == m_pRead && m_count > 0)
		return false;
		
	m_buffer[m_pWrite] = value;
	m_count++;
	
	m_pWrite++;
	m_pWrite %= BUFFER_SIZE;
	
	return true;
}

//! Returns the first element in the buffer and removes it
bool CircularBuffer::Pop(char* value_ref)
{
	if (m_count <= 0)
		return false;
		
	*value_ref = m_buffer[m_pRead];
	m_count--;
	
	m_pRead++;
	m_pRead %= BUFFER_SIZE;	
	
	return true;
}

//! Clear and/or initialize the buffer.
void CircularBuffer::Clear()
{
	for(int i = 0; i < BUFFER_SIZE; ++i) {
		m_buffer[i] = 0;
	}
	m_pRead = m_pWrite = 0;
	m_count = 0;
}

//! Obtain number of elements currently contained in the buffer.
int CircularBuffer::GetLoad()
{
	return m_count;
}
