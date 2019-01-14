/*! 
*	\class CircularBuffer
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	\brief Implementation of a circular buffer as a class.
*/


#include <stdlib.h>
#include "CircularBuffer.h"


//! Initialize the buffer
CircularBuffer::CircularBuffer(uint8_t size)
{
	m_buffer_size = size;
	m_buffer = (char*)malloc((size_t)size);
	this->Clear(); 
} //CircularBuffer


//! default destructor
CircularBuffer::~CircularBuffer()
{
	free(m_buffer);
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
	m_pWrite %= m_buffer_size;
	
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
	m_pRead %= m_buffer_size;	
	
	return true;
}

//! Clear and/or initialize the buffer.
void CircularBuffer::Clear()
{
	for(int i = 0; i < m_buffer_size; ++i) {
		m_buffer[i] = 0;
	}
	m_pRead = m_pWrite = 0;
	m_count = 0;
}

//! Obtain number of elements currently contained in the buffer.
uint8_t CircularBuffer::GetLoad()
{
	return m_count;
}

void CircularBuffer::ForwardReadPointer(uint8_t indeces)
{
	indeces = indeces < m_count ? indeces : m_count;
	
	m_pRead += indeces;
	m_pRead %= m_buffer_size;
	m_count -= indeces;
}

bool CircularBuffer::Peek(uint8_t* value_ref)
{
	return Peek(value_ref, 0);
}

bool CircularBuffer::Peek(uint8_t* value_ref, uint8_t offset)
{
	if ((m_count-offset) <= 0)
		return false;
	
	*value_ref = m_buffer[(m_pRead+offset) % m_buffer_size];
	return true;
}
