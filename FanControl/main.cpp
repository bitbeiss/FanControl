/*! \file FanControl.cpp
*	\brief Main function and center of the FanControl software.
*	\date 2018
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/ 

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "Port.h"
#include "LedBarMeter.h"
#include "Byte.h"
#include "Lcd.h"

const long LCD_COUNTER_START = 100000;
const long LCD_COUNTER_END = 500000;
const int small_delay = 10;
const int large_delay = 200;
const int large_delay_steps = large_delay / small_delay;

uint32_t ten_power(int power) {
	volatile uint32_t out = 1;
	for(int i = 0; i < power; ++i)
		out *= 10;
	return out;
}

void number_to_ascii(long number, char chars[]) {
	int i = 0;
	if (number < 0) {
		chars[i++] = '-';
		number = -number;
	}
	volatile uint32_t cmp = 0;
	volatile uint8_t quot = 0;
	
	for(int pot = 9; pot >= 0; pot--) {
		cmp = ten_power(pot);
		if ((quot = number / cmp) > 0) {
			number -= cmp * quot;
			chars[i] = quot + 0x30;
		} else {	
			chars[i] = 0x30;
		}
		++i;
	}
	chars[i] = '\0';
}

int main(void) {
	//Port ledBarPort(&PORTA,&DDRA,&PINA);
	//LedBarMeter ledBarVoltageMeter(ledBarPort);
	
	uint8_t voltage=0;
	
	Lcd lcd = Lcd();
	lcd.init4bit();
	lcd.setCursorPosition(1, 0);
	lcd.print("Hello World!");
	int lcd_delay_update_counter = 0;
	
	volatile long lcd_counter_output = LCD_COUNTER_START;
	char counter_output_str[7];
	
    while (1) 
    {
		//ledBarVoltageMeter.setVoltage(voltage);
		//ledBarVoltageMeter.indicateVoltage();
		voltage += 1;
		_delay_ms(small_delay);
		if (voltage >= 255) voltage=0;
		
		
		lcd_delay_update_counter++;
		
		// print the counter value only every 200ms
		if (lcd_delay_update_counter >= large_delay_steps) {
			lcd.setCursorPosition(2, 0);
			//itoa(lcd_counter_output, counter_output_str, 10);
			number_to_ascii(lcd_counter_output, counter_output_str);
			lcd.print(counter_output_str);
			lcd_counter_output += 1000;
			
			lcd_delay_update_counter = 0;
		}
		
		// reset the output to 100,000 once it has reached 500,000
		if (lcd_counter_output >= LCD_COUNTER_END) {
			lcd_counter_output = LCD_COUNTER_START;
		}
    }
}

