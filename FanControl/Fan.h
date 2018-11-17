/* 
* Fan.h
*
* Created: 13.11.18 09:52:02
* Author: filmu
*/


#ifndef __FAN_H__
#define __FAN_H__

#include "RotationTachymeter.h"

class Fan
{
//variables
public:
protected:
private:
	RotationTachymeter m_tachymeter;

//functions
public:
	Fan();
	~Fan();
	
	int GetPulseTimeMicroseconds();
	int GetRPM(); // measure RPM permanently, cache value
	//void SetStrength(uint8_t percent);
protected:
private:
	

}; //Fan

#endif //__FAN_H__
