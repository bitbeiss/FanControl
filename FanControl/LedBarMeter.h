/*! \class LedBarMeter
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief The LedBarMeter transforms the voltage (set) in the belonging Voltage class-variable to the number (and order) of activated control LEDs
*	The Voltage is meant to be set using setVoltage() method. Followed by a indicateVoltage() method call this leads to a representative LED
*	indication of the voltage currently held by the class.
*/
#pragma once
#include "Port.h"

using namespace std;

class LedBarMeter{
	public:
		LedBarMeter(Port&);
		~LedBarMeter();
		void init();				//!<Set all Pins as output and to value 0
		void setPort(Port&);
		Port getPort();
		void setValue(uint8_t);
		void displayValue();		//!<Show current Voltage on LED indicator
		//void displayVoltage();		//!<Show current Voltage on Lcd display

	private:
		uint8_t m_value;
		Port lbm_Port;
		
};