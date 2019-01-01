/* 
* RotationTachymeter.cpp
*
* Created: 13.11.18 10:01:47
* Author: filmu
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotationTachymeter.h"

volatile uint16_t RotationTachymeter::m_timer_value = 0;

// default constructor
RotationTachymeter::RotationTachymeter(uint8_t interrupt_pin)
{
	initInterrupts(interrupt_pin);
	initTimers();
} //RotationTachymeter

// default destructor
RotationTachymeter::~RotationTachymeter()
{
} //~RotationTachymeter



// PUBLIC functions
int RotationTachymeter::GetRotationSpeedInRoundsPerMinute()
{
	this->m_rpm = RotationTachymeter::RPM_MULTIPLIER / RotationTachymeter::m_timer_value;
	return this->m_rpm;
}

long RotationTachymeter::GetRotationPeriodLengthInMicroseconds() {
	this->m_period_width_us = RotationTachymeter::PERIOD_WIDTH_MULTIPLIER * RotationTachymeter::m_timer_value;
	return this->m_period_width_us;
}


// PRIVATE functions
void RotationTachymeter::initInterrupts(uint8_t interrupt_pin) {
	// Global Interrupt Control Register
	GICR |= (1 << interrupt_pin); // configure INT0 (PD2) as active external Interrupt
	
	//MCU Control Register
	MCUCR |= (1 << ISC01); // trigger Interrupt INT0 on falling edge
	MCUCR &= ~(1 << ISC00); // ---
}

void RotationTachymeter::initTimers() {
	TCCR1B |= (1 << CS11) | (1 << CS10); // set up timer with prescaler = 64
	TCCR1B &= ~(1 << CS12); // ---
	TCNT1 = 0; // reset counter
}


// external interrupt from fan (called twice per round)
void INT0_vect(void) {
	RotationTachymeter::m_timer_value = TCNT1;
	TCNT1 = 0; // reset counter
}