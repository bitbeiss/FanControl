/*! \class UsartController
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief
*/


#ifndef __USARTCONTROLLER_H__
#define __USARTCONTROLLER_H__

#include <avr/io.h>
#include "BaudRates.h"
#include "CircularBuffer.h"
#include "DataLink.h"

extern "C" void USART_TXC_vect(void) __attribute__ ((signal));
extern "C" void USART_RXC_vect(void) __attribute__ ((signal));


class UsartController : public DataLink
{
//variables
public:
protected:
private:
public:
	static CircularBuffer s_transmit_buffer;
	static CircularBuffer s_receive_buffer;
	static volatile uint8_t s_char_buffer;
	

//functions
public:
	UsartController(BaudRate baudrate, bool receive, bool transmit);
	~UsartController();
	
	void Transmit(const char data_tx[]);
	void Transmit(const char data_tx[], int length);
	
	uint8_t GetReceiveBufferLength();
	void GetReceiveData(uint8_t* out_data);
	
	void GetReceiveData(uint8_t* out_data, uint8_t length);
	
	CircularBuffer* GetReceiveBuffer() {
		return &(UsartController::s_receive_buffer);
	};
	
	
	void SetBaudrate(BaudRate baudrate);
	//void SetReceiver(UsartReceiver receiver);
	void ClearTransmitBuffer();
	
protected:
private:
	friend void USART_TXC_vect(void);
	friend void USART_RXC_vect(void);

}; //UsartController





#endif //__USARTCONTROLLER_H__
