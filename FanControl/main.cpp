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
	Port ledBarPort(&PORTA,&DDRA,&PINA);
	LedBarMeter ledBarVoltageMeter(ledBarPort);
	
	unsigned char count=0;
	
	Lcd lcd = Lcd();
	lcd.init4bit();
	lcd.Print("Hello World!");
	
    while (1) 
    {
		ledBarVoltageMeter.setVoltage(count);
		ledBarVoltageMeter.indicateVoltage();
		count++;
		_delay_ms(10);
		if (count>=255) count=0;
    }
}

