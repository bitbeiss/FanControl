/* 
* Fan.cpp
*
* Created: 13.11.18 09:52:02
* Author: filmu
*/

#include "RotationTachymeter.h"
#include "Fan.h"

// default constructor
Fan::Fan()
{
	m_tachymeter = RotationTachymeter();
} //Fan

// default destructor
Fan::~Fan()
{
} //~Fan

int Fan::GetPulseTimeMicroseconds() {
	return m_tachymeter.GetPeriodLengthMicroseconds();
}
// measure RPM permanently, cache value
int Fan::GetRPM() {
	return m_tachymeter.GetRPM();
}
/*void Fan::SetStrength(uint8_t percent) {
	return;
}*/