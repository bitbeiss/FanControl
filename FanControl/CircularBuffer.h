/*! \file CircularBuffer.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header of the circular buffer class.
*/


#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include <avr/io.h>

class CircularBuffer
{
//variables
public:
protected:
private:
	const static uint8_t BUFFER_SIZE = 96; /*! \var BUFFER_SIZE Is the freely chosen size of the circular buffer (supposed good tradeoff between memory usage and speed */

	uint8_t m_buffer[BUFFER_SIZE];
	volatile uint8_t m_pRead, m_pWrite;
	volatile uint8_t m_count;

//functions
public:
	CircularBuffer();
	~CircularBuffer();
	
	//void SetBufferSize(uint16_t buffer_size);
	
	bool Push(uint8_t value); 		//! Adds a new element at the end of the buffer
	bool Pop(uint8_t* value_ref); 	//! Returns the first element in the buffer and removes it
	void Clear(); 					//! Empties the buffer
	int GetLoad(); 					//! returns the number of elements currently contained in the buffer
	
protected:
private:

}; //CircularBuffer

#endif //__CIRCULARBUFFER_H__
