/*! \file main.cpp
*	\brief Main function and center of the FanControl software.
*	\date 2018 December
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/ 

#include "main.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Port.h"
#include "LedBarMeter.h"
#include "Byte.h"
#include "Lcd.h"
#include "Fan.h"
#include "CircularBuffer.h"
#include "UsartController.h"
#include "BaudRates.h"


// the delays specified below cannot be used to calculate FPS because this would ignore the time taken by the instructions excecuted in each loop
//! LEDs are updated with small delay (e.g. 10 milliseconds ~= 100fps)
const int small_delay = 10; 
//! display is updated with large delay (e.g. 50 milliseconds ~= 20fps)
const int large_delay = 50; 
//! relation of large to small delay, used to determine when large delay has elapsed
const int large_delay_steps = large_delay / small_delay;


int main(void)
{	
	//DDRA |= (1 << PA2);
	UsartController serial_controller = UsartController(BaudRates::_9600, true, true);
	Fan fan = Fan();
	sei();
	
	Port ledBarPort(&PORTA,&DDRA,&PINA);
	LedBarMeter ledBarMeter(ledBarPort); 
	
	// create an instance of the Lcd display and initialize it with a "welcome" message
	Lcd lcd = Lcd();
	lcd.Init4bit();
	lcd.SetCursorPosition(1, 0);
	lcd.Print("Fan Control");
	lcd.SetCursorPosition(2, 0);
	lcd.Print("ITS uC Labor");
	_delay_ms(250);
	lcd.ClearDisplay();
	int lcd_delay_update_counter = 0;
	
	char pulse_length_us_str[5];
	char adc_output_str[5];
	uint16_t fan_duty_cycle = 0;
	
	serial_controller.Transmit("\e[1A\r\n");
	serial_controller.Transmit("\e[38;5;196m");
	serial_controller.Transmit("ITS uC Labor\r\n");
	serial_controller.Transmit("Fan Controller\r\n");
	serial_controller.Transmit(" Duty  | Pulse Time |  RPM  \r\n");
	serial_controller.Transmit("\r\n");
	serial_controller.Transmit("\e[38;5;255m");
	
	_delay_ms(50);
	
    while (1) 
    {
		_delay_ms(small_delay);
		
		fan_duty_cycle = fan.GetFanSpeedAsSingleByte(); // returns number between 0 and 255
		ledBarMeter.setValue(fan_duty_cycle);
		ledBarMeter.displayValue();
		
		
		lcd_delay_update_counter++;
		
		// print the counter value and send USART message only every ~50ms
		// we could further increase display output speed by sending the USART message less frequently
		// or we could increase USART speed by removing the display
		if (lcd_delay_update_counter >= large_delay_steps) {
			//lcd.clearDisplay(); // this may be too slow
			
			fan_duty_cycle = (100 * fan_duty_cycle) / 255;
			
			lcd.SetCursorPosition(1, 0);
			lcd.Print("Fan: ");
			sprintf(adc_output_str, "%3d", fan_duty_cycle);
			lcd.Print(adc_output_str);
			lcd.Print("%");
			
			
			
			serial_controller.Transmit("\e[1A ");
			serial_controller.Transmit(adc_output_str);
			serial_controller.Transmit("%  ");
			
			
			
			//send current data to Lcd for display
			lcd.SetCursorPosition(2, 0);
			sprintf(pulse_length_us_str, "%5ld", fan.GetFanRevolutionPeriodInMicroseconds());
			lcd.Print(pulse_length_us_str);
			lcd.Print("us");
			lcd.Print(" ");
			
			//send current data to USART (PC). Data can be fetched on USB using a terminal like putty.
			serial_controller.Transmit("|   ");
			serial_controller.Transmit(pulse_length_us_str);
			serial_controller.Transmit("us");
			serial_controller.Transmit("  ");
			
			sprintf(pulse_length_us_str, "%4d", fan.GetFanSpeedInRoundsPerMinute());
			lcd.Print(pulse_length_us_str);
			lcd.Print("rpm");
			
			
			serial_controller.Transmit("| ");
			serial_controller.Transmit(pulse_length_us_str);
			serial_controller.Transmit("\r\n");
			
			
			lcd_delay_update_counter = 0;
			
			
			// SECTION receive data via serial interface
			uint8_t receive_buf_len = serial_controller.GetReceiveBufferLength();
			
			if (receive_buf_len > 0) {
				uint8_t receive_buf[receive_buf_len];
				serial_controller.GetReceiveData(receive_buf);
			
				//serial_controller.Transmit((char *)receive_buf);
				//serial_controller.Transmit("\r\n");
				
				//lcd.setCursorPosition(0, 10);
				//lcd.print((char *)receive_buf);
			}
		}
    }
}


