/*! 
*	\file Port.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header file for the Port class.
*/

#pragma once
#include <avr/io.h>

using namespace std;

class Port {
	
	public:
		Port(volatile uint8_t* port_reg, volatile uint8_t* ddr_reg, volatile uint8_t* pin_reg);
		Port();
		
		inline void setAddress(volatile uint8_t*);
		volatile uint8_t* getAddress();
		
		inline void setDataDirectionRegisterAddress(volatile uint8_t*);
		volatile uint8_t* getDataDirectionRegisterAddress();
		
		inline void setPinRegisterAddress(volatile uint8_t*);
		inline volatile uint8_t* getPinRegisterAddress();
		
		/**Set and delete single pin's on the port (int value: pin #) */
		void setPin(int);
		void delPin(int);
		
		/**Set a uint8_t value changing all the port's pins at once e.g. 0xFF for all pins to true */
		inline void setPortPinValue(uint8_t);
		inline void delPortPinValue(uint8_t);
		
		/**Set a uint8_t value changing all the DDR's pins at once e.g. 0xFF for all pins to true */
		inline void setDdrPinValue(uint8_t);
		inline void delDdrPinValue(uint8_t);
	
	private:
		volatile uint8_t* mPortRegister;
		volatile uint8_t* mDataDirectionRegister;
		volatile uint8_t* mPinRegister;
};