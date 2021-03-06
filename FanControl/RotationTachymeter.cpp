/*!
* 	\class  RotationTachymeter
* 	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief The RotationTachymeter contains all calculus functions related to period and speed. (Used also by the Fan class.)
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotationTachymeter.h"

volatile uint16_t RotationTachymeter::m_timer_value = 0;

//! Initializes the hardware (interrupt and timer)
RotationTachymeter::RotationTachymeter(uint8_t interrupt_pin)
{
	initInterrupt(interrupt_pin);
	initTimers();
} //RotationTachymeter

//! Default destructor
RotationTachymeter::~RotationTachymeter()
{
} //~RotationTachymeter


// PUBLIC functions
//! returns the measured rotation speed in the unit rounds per minute
uint16_t RotationTachymeter::GetRotationSpeedInRoundsPerMinute()
{
	this->m_rpm = RotationTachymeter::RPM_MULTIPLIER / RotationTachymeter::m_timer_value;
	return this->m_rpm;
}

//! returns the measured period length of one revolution in microseconds
uint16_t RotationTachymeter::GetRotationPeriodLengthInMicroseconds() {
	this->m_period_width_us = RotationTachymeter::PERIOD_WIDTH_MULTIPLIER * RotationTachymeter::m_timer_value;
	return this->m_period_width_us;
}


// PRIVATE functions
//! activates the external interrupts for measuring time between falling edges
void RotationTachymeter::initInterrupt(uint8_t interrupt_pin) {
	// Global Interrupt Control Register
	GICR |= (1 << interrupt_pin); // configure INT0 (PD2) as active external Interrupt
	
	//MCU Control Register
	MCUCR |= (1 << ISC01); // trigger Interrupt INT0 on falling edge
	MCUCR &= ~(1 << ISC00); // ---
}

//! activates the timer with the appropriate prescaler for the typical revolution period of the fan
void RotationTachymeter::initTimers() {
	TCCR1B |= (1 << CS11) | (1 << CS10); // set up timer with prescaler = 64
	TCCR1B &= ~(1 << CS12); // ---
	TCNT1 = 0; // reset counter
}


//! external interrupt from fan (called twice per full revolution)
void INT0_vect(void) {
	RotationTachymeter::m_timer_value = TCNT1;
	TCNT1 = 0; // reset counter
}