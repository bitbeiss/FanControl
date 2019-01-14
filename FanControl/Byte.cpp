/*! 
*	\file Byte.cpp
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/

#include <avr/io.h>
#include "Byte.h"

/*! Check if a bit is set or not at a given position.
	\param pos position of byte to check on
	\param byte byte containing the parameter of interest
	\return true/false */
bool isset(uint8_t pos, uint8_t byte) {
	return (byte & (1 << pos));
}