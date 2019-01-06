/*! \class Lcd
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Implementation of Lcd display driver. Offers initialization and send commands.
*/

#include "main.h"

#include <avr/io.h>
#include <util/delay.h>

#include "Byte.h"
#include "Lcd.h"
#include "Port.h"

//! Constructor: initialize Ports and set Pins to write mode.
Lcd::Lcd()
{
	//! Create and assign new Port configuration for LCD display.	
	Port lcdPort(&LCDPORT,&LCDDDR,&LCDPIN);
	this->mLcdPort = lcdPort;
	
	//! Set all Pins of the LCD Port to WRITE
	*(this->mLcdPort.getDataDirectionRegisterAddress()) |= (0xFF);
}

//! Used mainly for initialization of the display. (Which is initially in eight bit receive at one mode. Due to hardware wirings only 4 bits are actually sent though.)
void Lcd::send8bit(uint8_t type, uint8_t input) {
	setRegisterSelectPin(type);
	
	for(volatile int i = 0; i <= 3; i++) {
		//bit_value = isset(input, i);
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins8Bit[i]);
		}
		else {
			this->mLcdPort.delPin(mSendPins8Bit[i]);
		}
	}
	
	//latch
	this->sendEnablePulse();
}

//! Sending a character value to the display will be using this function with the given hardware-wiring.
void Lcd::send4bit(uint8_t type, uint8_t input) {
	setRegisterSelectPin(type);
	
	// send upper Nibble
	for(int i = 4; i <= 7; i++) {
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins4Bit[i]);
		}
		else {
			this->mLcdPort.delPin(mSendPins4Bit[i]);
		}
	}
	
	// latch
	this->sendEnablePulse();
	
	// send lower nibble
	for(int i = 0; i <= 3; i++) {
		if(input & (1 << i)) {
			this->mLcdPort.setPin(mSendPins4Bit[i]);
		}
		else {
			this->mLcdPort.delPin(mSendPins4Bit[i]);
		}
	}
	
	// latch
	this->sendEnablePulse();
}

//! Determine if to send data or a command by setting the RS Pin.
void inline Lcd::setRegisterSelectPin(uint8_t type) {
	if (type == COMMAND) {
		// set RS Pin to 0 to send a command
		this->mLcdPort.delPin(LCD_PIN_RS);
	}
	else {
		// set RS pin to 1 to send data
		this->mLcdPort.setPin(LCD_PIN_RS);
	}
}

//! Sends a command or data to the HD44780 LCD display and waits 50us afterwards.
void Lcd::Send(uint8_t type, uint8_t input) {
	this->Send(type, input, false);
}

//! Sends a command or data to the HD44780 LCD display and then adds either a long (2ms) or short delay (50us).
void Lcd::Send(uint8_t type, uint8_t input, bool long_delay) {
		
	// if we are working in 4-bit mode we need to send the upper nibble now
	// otherwise we set all of the pins and send them
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

//! Initialize LCD display to operate in four bit mode. (Sending a character of eight bits thus then will take two send-operations.)
void Lcd::Init4bit() {
	*(this->mLcdPort.getAddress()) = 0x00;
	
	// wait for some time, according to data sheet
	_delay_ms(15);
	
	//sytax: type ("COMMAND" | "DATA", "value to send")
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
	
	//set LCD on, cursor off, cursor blinking off
	//command = LCD_DISPLAY_CTRL | LCD_ON | LCD_CURSOR_ON | LCD_BLINK_ON;
	command = LCD_DISPLAY_CTRL | LCD_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	this->send4bit(COMMAND, command);
	_delay_us(40);
	
	this->ClearDisplay();
	
	// return home
	this->send4bit(COMMAND, 0x02);
	_delay_us(1700);
}

//! Sets the cursor position to the given line and row.
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


//! Clears the screen.
void Lcd::ClearDisplay() {
	this->Send(COMMAND, 0x01);
	_delay_ms(1);
}