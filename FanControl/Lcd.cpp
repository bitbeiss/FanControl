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

Lcd::Lcd()
{
	// create and assign new Port configuration for LCD display	
	Port lcdPort(&LCDPORT,&LCDDDR,&LCDPIN);
	this->mLcdPort = lcdPort;
	
	// set all Pins of the LCD Port to WRITE
	*(this->mLcdPort.getDataDirectionRegisterAddress()) |= (0xFF);
}

// sends a command or data to the HD44780 LCD display
void Lcd::send(uint8_t type, uint8_t input) {
	if (type == COMMAND) {
		// set RS Pin to 0 to send a command
		this->mLcdPort.delPin(LCD_PIN_RS);
	}
	else {
		// set RS pin to 1 to send data
		this->mLcdPort.setPin(LCD_PIN_RS); // @Clemens sollte das nicht PIN_RS sein?
	}
	
	//Upper Nibble senden
	for(int i = 4; i <= 7; i++) {
		if(isset(input, i)) {
			this->mLcdPort.setPin(SendPins[i]);
		}
		else {
			this->mLcdPort.delPin(SendPins[i]);
		}
	}
	
	// if we are working in 4-bit mode we need to send the upper nibble now
	// otherwise we set all of the pins and send then
	if(this->mUseFourBitMode) {
		this->enPulse();
	}
	
	//Lower Nibble senden
	for(int i = 0; i <= 3; i++) {
		if(isset(input, i)) {
			 this->mLcdPort.setPin(SendPins[i]);
		}
		else {
			this->mLcdPort.delPin(SendPins[i]);
		}
	}
	
	this->enPulse();
	
	//Auf LCD Controller warten
	_delay_us(50);
}

void Lcd::print(const char* text) {
	//Einzelne Zeichen aus Text holen und senden.
	
	for(int pos = 0; text[pos] != '\0'; pos++) {
		this->send(DATA, text[pos]);
	}
}

// send Enable Pulse
void inline Lcd::enPulse() {
	this->mLcdPort.setPin(LCD_PIN_E);
	_delay_us(50);
	this->mLcdPort.delPin(LCD_PIN_E);
	//_delay_us(50);
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
	_delay_us(200);
	
	// enable 4-bit mode
	this->send(COMMAND,0x02);
	//this->enFlanke();
	_delay_ms(5);
	
	// clear screen
	this->send(COMMAND,0x01);
	_delay_ms(2);
}

void Lcd::init8bit() {
	_delay_us(15);
	this->send(COMMAND,0x2C);
	this->enPulse();
	_delay_ms(5);
	this->send(COMMAND,0x0C);
	this->enPulse();
	_delay_us(200);
	this->send(COMMAND,0x01);
	this->enPulse();
	this->send(COMMAND,0x2);
	this->enPulse();
}

void Lcd::configure(uint8_t type, uint8_t lcd_cmd) {
}



void Lcd::setCursorPosition(int row, uint8_t line)
{
	if (row >= 40) row %= 40;
	
	uint8_t command = LCD_SETDRAM;
	
	if (line == 2) {
		command |= LCD_SECOND_LINE_START_ADDRESS + row;
	} else {
		command |= LCD_FIRST_LINE_START_ADDRESS + row;
	}
	
	this->send(COMMAND, command);
}