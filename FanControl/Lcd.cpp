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
		Port myPort(&LCDPORT,&LCDDDR,&LCDPIN);
		this->LcdPort = myPort;
		*(this->LcdPort.getDataDirectionRegisterAddress()) |= (0xFF);
}

void Lcd::send(uint8_t type, uint8_t input) {
	if (type == BEFEHL) {
		this->LcdPort.setPin(LCD_PIN_RS);
	}
	else
	this->LcdPort.setPin(LCD_PIN_7);
	
	//Lower Nibble senden
	uint8_t pins_upper[] = {LCD_PIN_7,LCD_PIN_6,LCD_PIN_5,LCD_PIN_4};
	for(int i=7; i>=0; i--) {
		if(isset(input,pins_upper[i])) {
			this->LcdPort.setPin(pins_upper[i]);
		}
		else {
			this->LcdPort.delPin(pins_upper[i]);
		}
	}
	
	this->enFlanke();
	
	//Upper Nibble senden
	uint8_t pins_lower[] = {LCD_PIN_7,LCD_PIN_6,LCD_PIN_5,LCD_PIN_4};
	for(uint8_t i=7; i>=0; i--) {
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

void Lcd::lprintf(char* text) {
	//Einzelne Zeichen aus Text holen und senden.
	uint8_t pos;
	for(pos=0; pos<255; pos++) {
		if(text[pos]==0) return;
		else this->send(DATEN, text[pos]);
	}
}

void inline Lcd::enFlanke() {
	this->LcdPort.setPin(LCD_PIN_E);
	_delay_us(50);
	this->LcdPort.delPin(LCD_PIN_E);
	_delay_us(50);
}

void Lcd::init4bit() {
	_delay_us(15);
	this->send(BEFEHL,0x03);
	this->enFlanke();
	_delay_ms(5);
	this->send(BEFEHL,0x03);
	this->enFlanke();
	_delay_us(200);
	this->send(BEFEHL,0x03);
	this->enFlanke();
	this->send(BEFEHL,0x02);
	this->enFlanke();
}

void Lcd::init8bit() {
	_delay_us(15);
	this->send(BEFEHL,0x2C);
	this->enFlanke();
	_delay_ms(5);
	this->send(BEFEHL,0x0C);
	this->enFlanke();
	_delay_us(200);
	this->send(BEFEHL,0x01);
	this->enFlanke();
	this->send(BEFEHL,0x2);
	this->enFlanke();
}

void Lcd::configure(uint8_t type, uint8_t lcd_cmd) {
}


