/*! \class Lcd
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	\brief Class implements a hardware driver for a HITACHI 44780 external LCD display.
*	\brief Represents an Lcd data structure with all methods belonging to the liquid crystal display.
*	The class implements an Lcd hardware driver API.
*/
#pragma once
#include <avr/io.h>

#include "Byte.h"
#include "Port.h"

//Configure LCD Port here by commenting in and out.
//#define _PORTA			1
#define _PORTB				1
#define _FOURBITMODE		1
//#define _EIGHTBITMODE		1
//#define _PORTC			1
//#define _PORTD			1

#ifdef						_PORTA
#define LCDPORT				PORTA
#define LCDDDR				DDRA
#define LCDPIN				PINA
#define LCD_PIN_RS			PA2
#define LCD_PIN_E			PA3
#define LCD_PIN_4			PA4
#define LCD_PIN_5			PA5
#define LCD_PIN_6			PA6
#define LCD_PIN_7			PA7
#endif

#ifdef						_PORTB
#define LCDPORT				PORTB
#define LCDDDR				DDRB
#define LCDPIN				PINB
#define LCD_PIN_RS			PB2
#define LCD_PIN_E			PB3
#define LCD_PIN_4			PB4
#define LCD_PIN_5			PB5
#define LCD_PIN_6			PB6
#define LCD_PIN_7			PB7
#endif

#ifdef						_PORTC
#define LCDPORT				PORTC
#define LCDDDR				DDRC
#define LCDPIN				PINC
#define LCD_PIN_RS			PC2
#define LCD_PIN_E			PC3
#define LCD_PIN_4			PC4
#define LCD_PIN_5			PC5
#define LCD_PIN_6			PC6
#define LCD_PIN_7			PC7
#endif

#ifdef						_PORTD
#define LCDPORT				PORTD
#define LCDDDR				DDRD
#define LCDPIN				PIND
#define LCD_PIN_RS			PD2
#define LCD_PIN_E			PD3
#define LCD_PIN_4			PD4
#define LCD_PIN_5			PD5
#define LCD_PIN_6			PD6
#define LCD_PIN_7			PD7
#endif

#define COMMAND				0
#define DATA				1
#define LCD_CLEAR			0x0C
#define LCD_HOME			0x02

#define LCD_DISPLAY_CTRL	0x08
#define LCD_OFF				0x00
#define LCD_ON				0x04
#define LCD_CURSOR_OFF		0x00
#define LCD_CURSOR_ON		0x02
#define LCD_BLINK_OFF		0x00
#define LCD_BLINK_ON		0x01

// combine function set with the modes below
#define LCD_FUNCTION_SET	0x20
#define LCD_4BIT_MODE		0x00
#define LCD_8BIT_MODE		0x10
#define LCD_TWO_LINE_MODE	0x08
#define LCD_ONE_LINE_MODE	0x00
#define LCD_FONT_5X10		0x04
#define LCD_FONT_5X8		0x00

//#define LCD_ON_C			0x0E
//#define LDC_OFF			0x08
//#define LCD_SET_4BIT		0x2C //!<4-Bit mode, Dual-line, F 0/1: 5x8 font		
#define LCD_SET_NOSHIFT		0x06

#define LCD_SET_DRAM_ADDRESS			0x80
#define LCD_FIRST_LINE_START_ADDRESS	0x00
#define LCD_SECOND_LINE_START_ADDRESS	0x40


class Lcd{
	public:
		Lcd();
		void init4bit();
		void init8bit();
		void print(char*);
		void setCursorPosition(int line, uint8_t row);
		void send(uint8_t type, uint8_t input);
		void send(uint8_t type, uint8_t input, bool long_delay);
		void clearDisplay();
		
	private:
		Port mLcdPort;
		
		// we are assuming that during runtime no switch between 4 and 8-bit mode will be necessary
		// because it is unlikely that the pin connections will be changed during runtime
		
		#ifdef _FOURBITMODE
		const bool mUseFourBitMode{true};
		
		#elif _EIGHTBITMODE
		const bool mUseFourBitMode{false};
		
		#endif
		
		
		// sending bits 0-7 on Pins 4-7 and 4-7
		uint8_t mSendPins4Bit[8] = {LCD_PIN_4, LCD_PIN_5, LCD_PIN_6, LCD_PIN_7, LCD_PIN_4, LCD_PIN_5, LCD_PIN_6, LCD_PIN_7};
			
		// sending bits 0-7 on Pins 0-7
		// pins for lower four bits are set the same because we don't actually have them connected
		uint8_t mSendPins8Bit[8] = {LCD_PIN_4, LCD_PIN_5, LCD_PIN_6, LCD_PIN_7, LCD_PIN_4, LCD_PIN_5, LCD_PIN_6, LCD_PIN_7};
		//uint8_t SendPins8Bit[8] = {LCD_PIN_0, LCD_PIN_1, LCD_PIN_2, LCD_PIN_3, LCD_PIN_4, LCD_PIN_5, LCD_PIN_6, LCD_PIN_7};
			
		
		// send Enable Pulse
		void inline enPulse();
		void inline setRegisterSelectPin(uint8_t type);
		
		void configure(uint8_t,uint8_t);
		
		void send4bit(uint8_t type, uint8_t input);
		void send8bit(uint8_t type, uint8_t input);
	};