/*! \file Lcd.cpp
*	\brief Implementation of Lcd class. 
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/

#include "main.h"

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


void Lcd::send8bit(uint8_t type, uint8_t input) {
	setRegisterSelectPin(type);
	//Port led_port = Port(&PORTA, &DDRA, &PINA);
	//DDRA = 0xFF;
	
	// ToDo i<=7 in 8 bit mode?
	for(volatile int i = 0; i <= 3; i++) {
		//bit_value = isset(input, i);
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins8Bit[i]);
			//led_port.setPin(i);
		}
		else {
			this->mLcdPort.delPin(mSendPins8Bit[i]);
			//led_port.delPin(i);
		}
	}
	
	//latch
	this->sendEnablePulse();
}

void Lcd::send4bit(uint8_t type, uint8_t input) {
	setRegisterSelectPin(type);
	
	//Port led_port = Port(&PORTA, &DDRA, &PINA);
	
	// send upper Nibble
	for(int i = 4; i <= 7; i++) {
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins4Bit[i]);
			//led_port.setPin(i);
		}
		else {
			this->mLcdPort.delPin(mSendPins4Bit[i]);
			//led_port.delPin(i);
		}
	}
	
	// latch
	this->sendEnablePulse();
	
	// send lower nibble
	for(int i = 0; i <= 3; i++) {
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins4Bit[i]);
			//led_port.setPin(i);
		}
		else {
			this->mLcdPort.delPin(mSendPins4Bit[i]);
			//led_port.delPin(i);
		}
	}
	
	// latch
	this->sendEnablePulse();
}

void inline Lcd::setRegisterSelectPin(uint8_t type) {
	if (type == COMMAND) {
		// set RS Pin to 0 to send a command
		this->mLcdPort.delPin(LCD_PIN_RS);
	}
	else {
		// set RS pin to 1 to send data
		this->mLcdPort.setPin(LCD_PIN_RS); // @Clemens sollte das nicht PIN_RS sein?
	}
}

// sends a command or data to the HD44780 LCD display and waits 50us afterwards
void Lcd::Send(uint8_t type, uint8_t input) {
	this->Send(type, input, false);
}

// sends a command or data to the HD44780 LCD display and then adds either a long (2ms) or short delay (50us)
void Lcd::Send(uint8_t type, uint8_t input, bool long_delay) {
		
	// if we are working in 4-bit mode we need to send the upper nibble now
	// otherwise we set all of the pins and send then
	if(this->mUseFourBitMode) {
		this->send4bit(type, input);
	} else {
		this->send8bit(type, input);
	}
	
	// wait for lcd controller to finish operation
	if (long_delay) {
		_delay_ms(2);
	} else {
		_delay_us(50);	
	}
}

void Lcd::Print(const char* text) {
	// retrieve single characters from text and send them
	
	for(int pos = 0; text[pos] != '\0'; pos++) {
		this->Send(DATA, text[pos], false);
	}
}

// send Enable Pulse
void inline Lcd::sendEnablePulse() {
	this->mLcdPort.setPin(LCD_PIN_E);
	_delay_us(50);
	this->mLcdPort.delPin(LCD_PIN_E);
	//_delay_us(50);
}


void Lcd::Init4bit() {
	*(this->mLcdPort.getAddress()) = 0x00;
	
	// wait for some time, according to data sheet
	_delay_ms(15);
	
	this->send8bit(COMMAND, 0x03);
	_delay_ms(5);
	
	this->send8bit(COMMAND, 0x03);
	_delay_us(200);
	
	this->send8bit(COMMAND, 0x03);
	_delay_us(40);
	
	// enable 4-bit mode (we are still in 8 bit mode)
	this->send8bit(COMMAND, 0x02);
	//_delay_ms(2);
	_delay_us(50);
	
	volatile uint8_t command;
	
	// set two line mode
	command = LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_TWO_LINE_MODE | LCD_FONT_5X8;
	this->send4bit(COMMAND, command);
	_delay_us(40);
	
	// set LCD on, cursor off, cursor blinking off
	//command = LCD_DISPLAY_CTRL | LCD_ON | LCD_CURSOR_ON | LCD_BLINK_ON;
	command = LCD_DISPLAY_CTRL | LCD_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	this->send4bit(COMMAND, command);
	_delay_us(40);
	
	this->ClearDisplay();
	
	// return home
	this->send4bit(COMMAND, 0x02);
	_delay_us(1700);
	
	/*this->send4bit(DATA, 49);
	_delay_ms(2);*/
}

/*void Lcd::Init8bit() {
	_delay_us(15);
	this->send8bit(COMMAND,0x2C);
	this->enPulse();
	_delay_ms(5);
	this->send8bit(COMMAND,0x0C);
	this->enPulse();
	_delay_us(200);
	this->send8bit(COMMAND,0x01);
	this->enPulse();
	this->send8bit(COMMAND,0x2);
	this->enPulse();
}*/

//void Lcd::configure(uint8_t type, uint8_t lcd_cmd) {}


// sets the cursor position to the given line and row
void Lcd::SetCursorPosition(uint8_t line, uint8_t row)
{
	if (row >= 40) row %= 40;
	
	uint8_t command = LCD_SET_DRAM_ADDRESS;
	
	if (line == 2) {
		command |= LCD_SECOND_LINE_START_ADDRESS + row;
	} else {
		command |= LCD_FIRST_LINE_START_ADDRESS + row;
	}
	
	this->Send(COMMAND, command, false);
}


// clears the screen
void Lcd::ClearDisplay() {
	this->Send(COMMAND, 0x01);
	_delay_ms(1);
}