/*! \class Lcd
	\author Clemens J. Zuzan	
	\brief Class implements a harware driver for a HITACHI external LCD display.
*/
#pragma once
#include <avr/io.h>

#include "Byte.h"
#include "Port.h"

//Configure LCD Port here by commenting in and out.
//#define _PORTA			1
#define _PORTB				1
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
/*! \class Lcd
	\brief Represents an Lcd data structure with all methods belonging to the liquid crystal display.
	The class implements an Lcd hardware driver API.
*/

#define BEFEHL				0
#define DATEN				1
#define LCD_CLEAR			0x0C
#define LCD_HOME			0x02
#define LCD_ON_C			0x0E
#define LDC_OFF				0x08
#define LCD_SET_4BIT		0x2C //!<4-Bit mode, Dual-line, F 0/1: 5x8 font		
#define LCD_SET_NOSHIFT		0x06
#define LCD_SETDRAM			0x80


class Lcd{
	public:
		Lcd();
		void init4bit();
		void init8bit();
		void lprintf(char*);
		
	private:
		Port LcdPort;
		
		void inline enFlanke();
		
		void configure(uint8_t,uint8_t);
		void inline send(uint8_t, uint8_t);
		
	};