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
#ifdef _FOURBITMODE
	this->UseFourBitMode = true;
#elif _EIGHTBITMODE
	this->UseFourBitMode = false;
#endif
	
	// create and assign new Port configuration for LCD display	
	Port myPort(&LCDPORT,&LCDDDR,&LCDPIN);
	this->LcdPort = myPort;
	
	// set all Pins of the LCD Port to WRITE
	*(this->LcdPort.getDataDirectionRegisterAddress()) |= (0xFF);
}

// sends a command or data to the HD44780 LCD display
void Lcd::send(uint8_t type, uint8_t input) {
	if (type == COMMAND) {
		// set RS Pin to 0 to send a command
		this->LcdPort.delPin(LCD_PIN_RS);
	}
	else {
		// set RS pin to 1 to send data
		this->LcdPort.setPin(LCD_PIN_RS); // @Clemens sollte das nicht PIN_RS sein?
	}
	
	//Upper Nibble senden
	for(int i = 4; i <= 7; i++) {
		if(isset(input, i)) {
			this->LcdPort.setPin(SendPins[i]);
		}
		else {
			this->LcdPort.delPin(SendPins[i]);
		}
	}
	
	// if we are working in 4-bit mode we need to send the upper nibble now
	// otherwise we set all of the pins and send then
	if(this->UseFourBitMode) {
		this->enFlanke();
	}
	
	//Lower Nibble senden
	for(int i = 0; i <= 3; i++) {
		if(isset(input, i)) {
			 this->LcdPort.setPin(SendPins[i]);
		}
		else {
			this->LcdPort.delPin(SendPins[i]);
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
	//this->enFlanke();
	_delay_ms(5);
	this->send(COMMAND,0x03);
	//this->enFlanke();
	_delay_us(200);
	this->send(COMMAND,0x03);
	//this->enFlanke();
	this->send(COMMAND,0x02);
	//this->enFlanke();
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


