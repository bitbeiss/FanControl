/* 
* CircularBuffer.h
*
* Created: 2.12.18 13:39:15
* Author: filmu
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
	const static uint8_t BUFFER_SIZE = 96;

	uint8_t m_buffer[BUFFER_SIZE];
	volatile uint8_t m_pRead, m_pWrite;
	volatile uint8_t m_count;

//functions
public:
	CircularBuffer();
	~CircularBuffer();
	
	void SetBufferSize(uint16_t buffer_size);
	
	bool Push(uint8_t value);
	bool Pop(uint8_t* value_ref);
	void Clear();
	int GetLoad();
	
protected:
private:

}; //CircularBuffer

#endif //__CIRCULARBUFFER_H__
