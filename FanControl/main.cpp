/*! \file FanControl.cpp
*	\brief Main function and center of the FanControl software.
*	\date 2018
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/ 

#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Port.h"
#include "LedBarMeter.h"
#include "Byte.h"
#include "Lcd.h"
#include "Fan.h"
#include "CircularBuffer.h";

const long LCD_COUNTER_START = 100000;
const long LCD_COUNTER_END = 500000;
const int small_delay = 10;
const int large_delay = 100;
const int large_delay_steps = large_delay / small_delay;

inline uint32_t ten_power(int power) {
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
	} else if (number == 0) {
		chars[0] = 0x30;
		chars[1] = '\0';
		return;
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


int main(void) {
	/*
	uint8_t c_buf_val;
	
	CircularBuffer buffer;
	buffer.Push(0x01);
	
	buffer.Pop(&c_buf_val);
	buffer.Push(0x02);
	buffer.Push(0x03);
	buffer.Push(0x04);
	
	buffer.Pop(&c_buf_val);
	buffer.Push(0x05);
	buffer.Push(0x06);
	buffer.Push(0x07);
	buffer.Push(0x08);
	buffer.Push(0x09);
	buffer.Push(0x0A);
	
	buffer.Pop(&c_buf_val);
	for(int i = 0; i < 10; ++i) {
		buffer.Push(0x10);
	}
	buffer.Pop(&c_buf_val);
	buffer.Push(0x0B);
	
	buffer.Pop(&c_buf_val);
	for(int i = 0; i < 10; ++i) {
		buffer.Push(0x20);
	}
	buffer.Pop(&c_buf_val);
	buffer.Push(0x0C);
	buffer.Pop(&c_buf_val);
	for(int i = 0; i < 8; ++i) {
		buffer.Push(0x30);
	}
	*/
	
	Fan fan = Fan();
	sei();
	
	
	Port ledBarPort(&PORTA,&DDRA,&PINA);
	LedBarMeter ledBarMeter(ledBarPort);
	
	Lcd lcd = Lcd();
	lcd.init4bit();
	lcd.setCursorPosition(1, 0);
	lcd.print("Fan Control");
	lcd.setCursorPosition(2, 0);
	lcd.print("ITS uC Labor");
	_delay_ms(250);
	
	int lcd_delay_update_counter = 0;
	
	volatile long lcd_counter_output = LCD_COUNTER_START;
	char counter_output_str[7], pulse_length_us_str[5];
	char trigger_pin_str[2], adc_output_str[5];
	int fan_duty_cycle = 0;
	
	//uint8_t voltage=0;
	
    while (1) 
    {
		//ledBarVoltageMeter.setVoltage(voltage);
		//ledBarVoltageMeter.indicateVoltage();
		//voltage += 1;
		_delay_ms(small_delay);
		//if (voltage >= 255) voltage=0;
		
		fan_duty_cycle = fan.GetDutyCycle();
		ledBarMeter.setValue(fan_duty_cycle * 2.55);
		ledBarMeter.displayValue();
		
		lcd_delay_update_counter++;
		
		// print the counter value only every 200ms
		if (lcd_delay_update_counter >= large_delay_steps) {
			lcd.clearDisplay();
			
			lcd.setCursorPosition(1, 0);
			lcd.print("Duty Cycle: ");
			number_to_ascii(fan_duty_cycle, adc_output_str);
			lcd.print(adc_output_str);
			lcd.print("%");
			
			
			lcd.setCursorPosition(2, 0);
			//number_to_ascii(lcd_counter_output, counter_output_str);
			//lcd.print(counter_output_str);
			
			//number_to_ascii(pulse_cnt>>1, counter_output_str);
			number_to_ascii(fan.GetPulseTimeMicroseconds(), pulse_length_us_str);
			lcd.print(pulse_length_us_str);
			lcd.print("us");
			
			lcd.print(" ");
			
			number_to_ascii(fan.GetRPM(), pulse_length_us_str);
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


