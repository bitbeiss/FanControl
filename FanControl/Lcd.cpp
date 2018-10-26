/*! \file Lcd.cpp
*	\brief Implemenation of Lcd class. 
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/
#include <avr/io.h>
#include <util/delay.h>

#include "Byte.h"
#include "Lcd.h"
#include "Port.h"

Lcd::Lcd() {
	// create and assign new Port configuration for LCD display	
	Port myPort(&LCDPORT,&LCDDDR,&LCDPIN);
	this->LcdPort = myPort;
	
	// set all Port B Pins to WRITE
	*(this->LcdPort.getDataDirectionRegisterAddress()) |= (0xFF);
}

// sends a command or data to the HD44780 LCD display
void Lcd::send(uint8_t type, uint8_t input) {
	if (type == COMMAND) {
		this->LcdPort.setPin(LCD_PIN_RS);
	}
	else {
		this->LcdPort.setPin(LCD_PIN_7); // @Clemens sollte das nicht PIN_RS sein?
	}
	
	//Lower Nibble senden
	uint8_t pins_upper[] = {LCD_PIN_7, LCD_PIN_6, LCD_PIN_5, LCD_PIN_4};
		
	for(int i = 0; i < 4; i++) {
		if(isset(input,pins_upper[i])) {
			this->LcdPort.setPin(pins_upper[i]);
		}
		else {
			this->LcdPort.delPin(pins_upper[i]);
		}
	}
	
	this->enFlanke();
	
	//////////////////////////////////////////////////////////////////////////
	// es findet kein Bitshift statt, somit sendet die zweite Hälfte der Funktion die gleichen Daten wie die erste?
	// es müssten ja zuerst die oberen 4 Bits und dann die unteren 4 Bits gesendet werden
	// dh. zuerst input senden (obere 4 Bits weil auf Pins 4,5,6,7 gesendet wird), dann input << 4 (untere 4 Bits werden auf Bits 4,5,6,7 geshiftet)
	
	//Upper Nibble senden
	input <<= 4;
	uint8_t pins_lower[] = {LCD_PIN_7, LCD_PIN_6, LCD_PIN_5, LCD_PIN_4};
		
	for(int i = 0; i < 4; i++) {
		if(isset(input,pins_lower[i])) {
			 this->LcdPort.setPin(pins_lower[i]);
		}
		else {
			this->LcdPort.delPin(pins_lower[i]);
		}
	}
	
	this->enFlanke();
	
	//Auf LCD Controller warten
	_delay_ms(5);
}

void Lcd::lprintf(const char* text) {
	//Einzelne Zeichen aus Text holen und senden.
	
	for(int pos = 0; text[pos] != '\0'; pos++) {
		this->send(DATA, text[pos]);
	}
}

// send Enable Pulse
void inline Lcd::enFlanke() {
	this->LcdPort.setPin(LCD_PIN_E);
	_delay_us(50);
	this->LcdPort.delPin(LCD_PIN_E);
	_delay_us(50);
}

void Lcd::init4bit() {
	_delay_ms(15);
	this->send(COMMAND,0x03);
	this->enFlanke();
	_delay_ms(5);
	this->send(COMMAND,0x03);
	this->enFlanke();
	_delay_us(200);
	this->send(COMMAND,0x03);
	this->enFlanke();
	this->send(COMMAND,0x02);
	this->enFlanke();
}

void Lcd::init8bit() {
	_delay_us(15);
	this->send(COMMAND,0x2C);
	this->enFlanke();
	_delay_ms(5);
	this->send(COMMAND,0x0C);
	this->enFlanke();
	_delay_us(200);
	this->send(COMMAND,0x01);
	this->enFlanke();
	this->send(COMMAND,0x2);
	this->enFlanke();
}

void Lcd::configure(uint8_t type, uint8_t lcd_cmd) {
}


