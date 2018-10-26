/*! \class Port
*	\brief class contains all data and methods regarding ports
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	Data contained are port address, DDR, PIN
*	The port address can be seta and retrieved as DDR and PIN can.
*	setPin and delPin methods allow so set a single pin to true or false.
*/

#pragma once
#include <avr/io.h>

using namespace std;

class Port {
	
	public:
		Port(volatile uint8_t*, volatile uint8_t*, volatile uint8_t*);
		Port();
		
		inline void setAddress(volatile uint8_t*);
		volatile uint8_t* getAddress();
		
		inline void setDataDirectionRegisterAddress(volatile uint8_t*);
		volatile	uint8_t* getDataDirectionRegisterAddress();
		
		inline void setPinRegisterAddress(volatile uint8_t*);
		inline volatile uint8_t* getPinRegisterAddress();
		
		/**Set and delete single pin's on the port (int value: pin #) */
		void setPin(int);
		void delPin(int);
		
		/**Set a uint8_t value changing all the port's pins at once e.g. 0xFF for all pins to true */
		inline void setPortPinValue(uint8_t);
		inline void delPortPinValue(uint8_t);
	
	private:
		volatile uint8_t* PORT;
		volatile uint8_t* DDR;
		volatile uint8_t* PIN;
};