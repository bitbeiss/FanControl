/*! 
*	\class UsartController
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Provides access to the serial port (USART functionality) of the microcontroller
*/

#include "main.h"

#include <util/delay.h>
#include <avr/io.h>
#include "UsartController.h"
#include "CircularBuffer.h"
	

#define UBRR(baud) ((F_CPU)/(baud*16UL)-1)


CircularBuffer UsartController::s_transmit_buffer(32);
CircularBuffer UsartController::s_receive_buffer(128);
volatile uint8_t UsartController::s_char_buffer = 0;


/*! Creates a default controller for reading and writing data via USART.
By default the controller does not allow settings for number of data-, stop- and parity bits.
Default values are:  parity: none, stop-bits: 1, data-bits: 8 */
UsartController::UsartController(BaudRate baudrate, bool receive, bool transmit)
{
	SetBaudrate(baudrate);
		
	if (receive) {
		UCSRB |= (1 << RXEN) | (1 << RXCIE);
	}
	if (transmit) {
		UCSRB |= (1 << TXEN) | (1 << TXCIE);
	}
	
	
	// get values from arguments
	// data bits: 8
	// parity: none
	// stop-bit: 1
	
	// parity: none
	/*
	// parity: even
	UCSRC |= (1 << URSEL) | (1 << UPM1);
	// parity: odd
	UCSRC |= (1 << URSEL) | (1 << UPM1) | (1 << UPM0);
	*/
	// stop-bit: 1
	// stop-bit: 2
	//UCSRC |= (1 << URSEL) | (1 << USBS);
	
	// character size
	int data_bits = 8;
	
	switch(data_bits) {
		case 5: break;
		case 6:
			UCSRC |= (1 << URSEL) | (1 << UCSZ0);
			break;
		case 7:
			UCSRC |= (1 << URSEL) | (1 << UCSZ1);
			break;
		case 8: 
			UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
			break;
		case 9:
			UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
			UCSRB |= (1 << UCSZ2);
			break;
	}
	
} //UsartController

// default destructor override
UsartController::~UsartController()
{
} //~UsartController


//! Sets up the USART to use the given baudrate. These are limited to specific established values.
void UsartController::SetBaudrate(BaudRate baudrate)
{
	uint16_t baudrate_nr = 0;
	switch(baudrate) {
		case BaudRates::_2400: baudrate_nr = 2400; break;
		case BaudRates::_4800: baudrate_nr = 4800; break;
		case BaudRates::_9600: baudrate_nr = 9600; break;
		case BaudRates::_14400: baudrate_nr = 14400; break;
		case BaudRates::_19200: baudrate_nr = 19200; break;
		case BaudRates::_28800: baudrate_nr = 28800; break;
		case BaudRates::_38400: baudrate_nr = 38400; break;
		case BaudRates::_57600: baudrate_nr = 57600; break;
	}
	baudrate_nr = UBRR(baudrate_nr); // from here on baudrate_nr is ubrr

	UCSRC &= ~(1 << URSEL);
	UBRRH = (baudrate_nr>>8);
	UBRRL = baudrate_nr;
}

void UsartController::Transmit(const char data_tx[], int length) {
	for(int pos = 0; pos < length; ++pos) {
		UsartController::s_transmit_buffer.Push(data_tx[pos]);
	}
	
	// if the current transmit buffer is empty the transmit interrupt will not be called automatically, therefore we trigger it manually
	if (UCSRA & (1 << UDRE)) {
		USART_TXC_vect();
	}
}

//! Push the passed string data into a ring buffer and transmit the data as fast as possible.
void UsartController::Transmit(const char data_tx[])
{
	for(int pos = 0; data_tx[pos] != '\0'; pos++) {
		UsartController::s_transmit_buffer.Push(data_tx[pos]);
	}
	
	// if the current transmit buffer is empty the transmit interrupt will not be called automatically, therefore we trigger it manually
	if (UCSRA & (1 << UDRE)) {
		USART_TXC_vect();
	}
}

void UsartController::ClearTransmitBuffer() {
	UsartController::s_transmit_buffer.Clear();
}

//! Returns the number of elements currently contained in the buffer
uint8_t UsartController::GetReceiveBufferLength() {
	return UsartController::s_receive_buffer.GetLoad();
}

//! Writes the buffer contents into the passed array; the length of the contents can be retrieved from GetReceiveBufferLength()
void UsartController::GetReceiveData(uint8_t* out_data)
{
	char char_buffer;
	while (UsartController::s_receive_buffer.Pop(&char_buffer)) {
		*(out_data++) = char_buffer;
	}
}

void UsartController::GetReceiveData(uint8_t* out_data, uint8_t length)
{
	uint8_t load = UsartController::s_receive_buffer.GetLoad();
	if (load > length) {
		UsartController::s_receive_buffer.ForwardReadPointer(load - length);
	}
	
	char char_buffer;
	while (UsartController::s_receive_buffer.Pop(&char_buffer)) {
		*(out_data++) = char_buffer;
	}
}


char c_char_buffer;
volatile uint16_t char_counter = 0;

//! This interrupt is called when the last USART message has been sent out, indicating that the next message can be sent
void USART_TXC_vect(void) {
	
	//if (UsartController::s_buffer.Pop(const_cast<uint8_t*>(&UsartController::s_char_buffer))) {
	if (UsartController::s_transmit_buffer.Pop(&c_char_buffer)) {
		UDR = c_char_buffer;
	}
}

//! This interrupt is called when a USART message has been received, indicating that the data should be retrieved from the register and stored
void USART_RXC_vect(void) {
	//PORTA ^= (1 << PA1);
	
	char_counter++;
	c_char_buffer = UDR;	
	
	UsartController::s_receive_buffer.Push(c_char_buffer);
}