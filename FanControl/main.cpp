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
#include "FanDataDisplay.h"


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
	DDRA = 0xFF;
	
	UsartController serial_controller = UsartController(BaudRates::_9600, true, true);
	Fan fan = Fan(serial_controller);
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
	
	
	uint8_t fan_duty_cycle_byte = 0;
	
	FanDataDisplay fan_data_display(fan, lcd);
	//char str[10];
	
	_delay_ms(50);
	
    while (1) 
    {
		_delay_ms(small_delay);
		
		fan_duty_cycle_byte = fan.GetFanSpeedAsSingleByte(); // returns number between 0 and 255
		ledBarMeter.setValue(fan_duty_cycle_byte);
		ledBarMeter.displayValue();
		
		
		lcd_delay_update_counter++;
		
		// print the counter value and send USART message only every ~50ms
		// we could further increase display output speed by sending the USART message less frequently
		// or we could increase USART speed by removing the display
		if (lcd_delay_update_counter >= large_delay_steps) {
			//lcd.clearDisplay(); // this may be too slow
			
			fan.Update();
			fan_data_display.UpdateDisplay();
			
			
			//lcd.SetCursorPosition(1, 0);
			//sprintf(str, "%u", serial_controller.s_receive_buffer.GetLoad());
			
			//lcd.Print(str);
			
			//char data_package[] = { 0x01, 0x01, 0b01110000, fan_duty_cycle_byte, rpm >> 8, rpm >> 0, revolution_period >> 8, revolution_period >> 0};
			//serial_controller.Transmit(data_package);
			
			
			
			lcd_delay_update_counter = 0;
		}
    }
}


