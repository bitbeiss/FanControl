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
	static const uint8_t BUFFER_SIZE = 32;

protected:
private:
	uint8_t m_buffer[BUFFER_SIZE];
	uint8_t m_pRead, m_pWrite;
	uint8_t m_count;

//functions
public:
	CircularBuffer();
	~CircularBuffer();
	
	bool Push(uint8_t value);
	bool Pop(uint8_t* value_ref);
	void Clear();
	
protected:
private:

}; //CircularBuffer

#endif //__CIRCULARBUFFER_H__
