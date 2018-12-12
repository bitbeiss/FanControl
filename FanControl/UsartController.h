/* 
* UsartController.h
*
* Created: 9.12.18 14:29:30
* Author: filmu
*/


#ifndef __USARTCONTROLLER_H__
#define __USARTCONTROLLER_H__

#include <avr/io.h>
#include "BaudRates.h"
#include "CircularBuffer.h"

extern "C" void USART_TXC_vect(void) __attribute__ ((signal));
extern "C" void USART_RXC_vect(void) __attribute__ ((signal));


class UsartController
{
//variables
public:
protected:
private:
	static CircularBuffer s_transmit_buffer;
	static CircularBuffer s_receive_buffer;
	static volatile uint8_t s_char_buffer;

//functions
public:
	UsartController(BaudRate baudrate, bool receive, bool transmit);
	~UsartController();
	
	void SetBaudrate(BaudRate baudrate);
	void Transmit(char data_tx[]);
	
	uint8_t GetReceiveBufferLength();
	void GetReceiveData(uint8_t* out_data);
	//void SetReceiver(UsartReceiver receiver);
	
protected:
private:
	friend void USART_TXC_vect(void);
	friend void USART_RXC_vect(void);

}; //UsartController





#endif //__USARTCONTROLLER_H__
