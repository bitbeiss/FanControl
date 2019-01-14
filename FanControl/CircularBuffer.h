/*! 
*	\file CircularBuffer.h
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
	/*! BUFFER_SIZE Is the freely chosen size of the circular buffer (supposed good tradeoff between memory usage and speed) */
	const static uint8_t BUFFER_SIZE = 96; 

	char m_buffer[BUFFER_SIZE];
	volatile uint8_t m_pRead, m_pWrite;
	volatile uint8_t m_count;

//functions
public:
	CircularBuffer();
	~CircularBuffer();
	
	bool Push(char value);
	bool Pop(char* value_ref);
	void Clear();
	int GetLoad();
	
protected:
private:

}; //CircularBuffer

#endif //__CIRCULARBUFFER_H__
