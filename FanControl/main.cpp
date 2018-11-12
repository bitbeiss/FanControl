/*! \file FanControl.cpp
*	\brief Main function and center of the FanControl software.
*	\date 2018
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/ 

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
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
	bool nr_started = false;
	
	for(int pot = 9; pot >= 0; pot--) {
		cmp = ten_power(pot);
		if ((quot = number / cmp) > 0) {
			nr_started = true;
			number -= cmp * quot;
			chars[i] = quot + 0x30;
		} else if (nr_started) {
			chars[i] = 0x30;
		}
		if (nr_started)
			++i;
	}
	chars[i] = '\0';
}

int pulse_cnt = 0;
int pulse_width_us, current_timer_value;
int rpm;
const unsigned long PULSE_WIDTH_MULTIPLIER = 1000000 * 64 / F_CPU; // factor to get from timer counts to microseconds with prescaler 64
const unsigned long RPM_MULTIPLIER = (1000000 * 60)/(2 * PULSE_WIDTH_MULTIPLIER); // times 60 for rps to rpm, times 1m for rpus to rps

ISR (INT0_vect) {
	current_timer_value = TCNT1;
	
	pulse_cnt++;
	//rpm = 3750000 / pulse_width_cnt;
	pulse_width_us = PULSE_WIDTH_MULTIPLIER * current_timer_value;
	//rpm = (60*1000000/pulse_width_us) >> 1;
	rpm = RPM_MULTIPLIER/current_timer_value;
	TCNT1 = 0; // reset counter
}

void initPWMCounterInterrupt() {
	GICR |= (1 << INT0); // configure INT0 (PD2) as active external Interrupt
	MCUCR |= (1 << ISC01); 
	MCUCR &= ~(1 << ISC00); // trigger Interrupt INT0 on falling edge
}	

void initFanRPMPins() {
	
}

void initFanRPMMeasurementTimer() { 
	TCCR1B |= (1 << CS11) | (1 << CS10); // set up timer with prescaler = 64
	TCNT1 = 0; // initialize counter
}

int main(void) {
	//initFanRPMPins();
	initPWMCounterInterrupt();
	initFanRPMMeasurementTimer();
	sei();
	
	
	Port ledBarPort(&PORTA,&DDRA,&PINA);
	LedBarMeter ledBarVoltageMeter(ledBarPort);
	
	uint8_t voltage=0;
	
	Lcd lcd = Lcd();
	lcd.init4bit();
	lcd.setCursorPosition(1, 0);
	lcd.print("Hello World!");
	int lcd_delay_update_counter = 0;
	
	volatile long lcd_counter_output = LCD_COUNTER_START;
	char counter_output_str[7], pulse_length_us_str[5];
	
    while (1) 
    {
		ledBarVoltageMeter.setVoltage(voltage);
		ledBarVoltageMeter.indicateVoltage();
		voltage += 1;
		_delay_ms(small_delay);
		if (voltage >= 255) voltage=0;
		
		
		lcd_delay_update_counter++;
		
		// print the counter value only every 200ms
		if (lcd_delay_update_counter >= large_delay_steps) {
			lcd.setCursorPosition(2, 0);
			//number_to_ascii(lcd_counter_output, counter_output_str);
			//lcd.print(counter_output_str);
			
			//number_to_ascii(pulse_cnt>>1, counter_output_str);
			number_to_ascii(pulse_width_us, pulse_length_us_str);
			lcd.print(pulse_length_us_str);
			lcd.print("us");
			
			lcd.print(" ");
			
			number_to_ascii(rpm, pulse_length_us_str);
			lcd.print(pulse_length_us_str);
			lcd.print("rpm");
			
			lcd_counter_output += 1000;
			lcd_delay_update_counter = 0;
		}
		
		// reset the output to 100,000 once it has reached 500,000
		if (lcd_counter_output >= LCD_COUNTER_END) {
			lcd_counter_output = LCD_COUNTER_START;
		}
    }
}

