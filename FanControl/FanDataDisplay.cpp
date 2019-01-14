/* 
* FanDataDisplay.cpp
*
* Created: 12.1.19 18:27:49
* Author: filmu
*/


#include <stdio.h>
#include "FanDataDisplay.h"

void FanDataDisplay::UpdateDisplay()//(uint8_t fan_speed_percent, uint16_t fan_speed_in_rpm, uint32_t fan_revolution_time)
{
	m_fan_duty_cycle = ((uint16_t)100 * m_fan.GetFanSpeedAsSingleByte()) / 255;
	m_fan_rpm = m_fan.GetFanSpeedInRoundsPerMinute();
	m_fan_revolution_time = m_fan.GetFanRevolutionPeriodInMicroseconds();
	
	
	m_lcd_display.SetCursorPosition(1, 0);
	
	m_lcd_display.Print("Fan: ");
	sprintf(str_buffer, "%3d", m_fan_duty_cycle);
	m_lcd_display.Print(str_buffer);
	m_lcd_display.Print("%");
	
	
	m_lcd_display.SetCursorPosition(2, 0);
			
	sprintf(str_buffer, "%5ld", m_fan_revolution_time);
	m_lcd_display.Print(str_buffer);
	m_lcd_display.Print("us ");


	sprintf(str_buffer, "%4d", m_fan_rpm);
	m_lcd_display.Print(str_buffer);
	m_lcd_display.Print("rpm");
}

// default constructor
FanDataDisplay::FanDataDisplay(Fan& fan, Lcd& lcd) : m_lcd_display(lcd), m_fan(fan)
{
} //FanDataDisplay

// default destructor
FanDataDisplay::~FanDataDisplay()
{
} //~FanDataDisplay
