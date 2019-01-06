﻿/*! 
*	\file LedBarMeter.cpp
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief 
*/

#include "LedBarMeter.h"
#include "Port.h"

using namespace std;

LedBarMeter::LedBarMeter(Port& pt){	
	this->setPort(pt);
	this->setValue(0);
	this->init();			
}

LedBarMeter::~LedBarMeter(){
}

//! Initialize the LedBar: set data direction register of the led's Pins to output and set the output to zero as an initial state.
void LedBarMeter::init() {
	*(this->getPort().getDataDirectionRegisterAddress()) |= 0xF0;  //upper nibble (lower LED row) shall be the indicator
	*(this->getPort().getAddress()) &= ~(0x0F); //set all Pins for LED bar output to 0 (LEDs off), leave lower pins untouched
}

void LedBarMeter::setPort(Port& pt ) {
	this->lbm_Port = pt;
}

Port LedBarMeter::getPort() {
	return this->lbm_Port;
}

void LedBarMeter::setValue(uint8_t value) {
	this->m_value = value;
}

//! Map the numeric value (0 to 254) to classes then represented by a shining led.
void LedBarMeter::displayValue() {
	if (m_value < 51){
		this->getPort().delPin(4);
		this->getPort().delPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (m_value >= 51 && m_value < 102) {	
		this->getPort().setPin(4);
		this->getPort().delPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (m_value >= 102 && m_value < 153) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().delPin(6);
		this->getPort().delPin(7);
	}
	if (m_value >= 153 && m_value < 204) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().setPin(6);
		this->getPort().delPin(7);
	}
	if (m_value >= 204 ) {
		this->getPort().setPin(4);
		this->getPort().setPin(5);
		this->getPort().setPin(6);
		this->getPort().setPin(7);
	}	
}


