/*! \class Port.cpp
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Class contains all data and methods regarding ports	
*	\brief Data contained are port address, DDR, PIN
*	\brief The port address can be seta and retrieved as DDR and PIN can.
*	\brief setPin and delPin methods allow so set a single pin to true or false.
*/
#include "Port.h"
#include <avr/io.h>

using namespace std;

Port::Port(
	volatile uint8_t* port_reg,
	volatile uint8_t* ddr_reg,
	volatile uint8_t* pin_reg)
	:
	mPortRegister(port_reg),
	mDataDirectionRegister(ddr_reg),
	mPinRegister(pin_reg) {}

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

inline void Port::setDdrPinValue(uint8_t val) {
	*(this->getDataDirectionRegisterAddress()) |= (val);
}

inline void Port::delDdrPinValue(uint8_t val) {
	*(this->getDataDirectionRegisterAddress()) &= ~(val);
}

inline void Port::setAddress(volatile uint8_t* port) {
	this->mPortRegister = port;
}

volatile uint8_t* Port::getAddress() {
	return this->mPortRegister;
}

inline void Port::setDataDirectionRegisterAddress(volatile uint8_t* ddr) {
	this->mDataDirectionRegister = ddr;
}

volatile uint8_t* Port::getDataDirectionRegisterAddress() {
	return this->mDataDirectionRegister;
}

inline void Port::setPinRegisterAddress(volatile uint8_t* pin) {
	this->mPinRegister = pin;
}

volatile uint8_t* Port::getPinRegisterAddress() {
	return this->mPinRegister;
}