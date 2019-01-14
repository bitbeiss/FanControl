/* 
* FanDataDisplay.h
*
* Created: 12.1.19 18:27:49
* Author: filmu
*/


#ifndef __FANDATADISPLAY_H__
#define __FANDATADISPLAY_H__

#include "Lcd.h"
#include "Fan.h"

class FanDataDisplay
{
//variables
public:
protected:
private:
	Lcd& m_lcd_display;
	Fan& m_fan;
	
	volatile uint8_t m_fan_duty_cycle = 0;
	volatile uint16_t m_fan_rpm = 0;
	volatile uint32_t m_fan_revolution_time = 0;
	
	char str_buffer[5];
	

//functions
public:
	FanDataDisplay(Fan& fan, Lcd& lcd);
	~FanDataDisplay();
	
	void UpdateDisplay();
	
	
protected:
private:
	FanDataDisplay( const FanDataDisplay &c );
	FanDataDisplay& operator=( const FanDataDisplay &c );

}; //FanDataDisplay

#endif //__FANDATADISPLAY_H__
