/*! \file FanControl.cpp
*	\brief Main function and center of the FanControl software.
*	\date 2018
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/ 


#include <avr/io.h>
#include <util/delay.h>

#include "Port.h"
#include "LedBarMeter.h"
#include "Byte.h"
#include "Lcd.h"


int main(void) {
	Port myPORTA(&PORTA,&DDRA,&PINA);
	LedBarMeter myLedBarMeter(myPORTA);
	
	unsigned char count=0;
	
	Lcd myLCD = Lcd();
	myLCD.init8bit();
	
    while (1) 
    {
		//myLCD.lprintf("Hello World!");
			
		myLedBarMeter.setVoltage(count);
		myLedBarMeter.indicateVoltage();
		count++;
		_delay_ms(100);
		if (count>=255) count=0;
    }
	
	
	
}
