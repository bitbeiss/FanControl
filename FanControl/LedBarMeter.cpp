/*! \file LedBarMeter.cpp
*	\brief Implemenation of LedBarMeter class.
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/

#include "LedBarMeter.h"
#include "Port.h"

using namespace std;

LedBarMeter::LedBarMeter(Port& pt){	
	this->setPort(pt);
	this->setVoltage(0);
	this->init();			
}

LedBarMeter::~LedBarMeter(){
}

void LedBarMeter::init() {
	*(this->getPort().getDataDirectionRegisterAddress()) |= 0x0F;  //leave P0-P3 as output untouched; lower 4 are indicator LEDs.
	*(this->getPort().getAddress()) &= ~(0x00);
}

void LedBarMeter::setPort(Port& pt ) {
	this->lbm_Port = pt;
}

Port LedBarMeter::getPort() {
	return this->lbm_Port;
}

void LedBarMeter::setVoltage(uint8_t voltage) {
	this->Voltage = voltage;
}

void LedBarMeter::indicateVoltage() {
	if (Voltage < 51){
		this->getPort().delPin(4);
		this->getPort().delPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (Voltage >= 51 && Voltage < 102) {	
		this->getPort().setPin(4);
		this->getPort().delPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (Voltage >= 102 && Voltage < 153) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (Voltage >= 153 && Voltage < 204) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().setPin(6);
		this->getPort().delPin(7);
	}
	if (Voltage >= 204 ) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().setPin(6);
		this->getPort().setPin(7);
	}	
}


