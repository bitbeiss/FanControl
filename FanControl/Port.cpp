/*! \file Port.cpp
*	\brief Implemenation of Port class.
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/
#include "Port.h"
#include <avr/io.h>

using namespace std;

Port::Port(volatile uint8_t* port_reg, volatile uint8_t* ddr_reg, volatile uint8_t* pin_reg) {
	this->setAddress(port_reg);
	this->setDataDirectionRegisterAddress(ddr_reg);
	this->setPinRegisterAddress(pin_reg);
}

Port::Port() {
}

void Port::setPin(int pin) {
	*(this->getAddress()) |= (1 << pin);
}

void Port::delPin(int pin) {
	*(this->getAddress()) &= ~(1 << pin);
}

inline void Port::setPortPinValue(uint8_t val) {
	*(this->getAddress()) |= (val);
}

inline void Port::delPortPinValue(uint8_t val) {
	*(this->getAddress()) &= ~(val);
}

inline void Port::setAddress(volatile uint8_t* port) {
	this->PORT = port;
}

volatile uint8_t* Port::getAddress() {
	return this->PORT;
}

inline void Port::setDataDirectionRegisterAddress(volatile uint8_t* ddr) {
	this->DDR = ddr;
}

volatile uint8_t* Port::getDataDirectionRegisterAddress() {
	return this->DDR;
}

inline void Port::setPinRegisterAddress(volatile uint8_t* pin) {
	this->PIN = pin;
}

volatile uint8_t* Port::getPinRegisterAddress() {
	return this->PIN;
}