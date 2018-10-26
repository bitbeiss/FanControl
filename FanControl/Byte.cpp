/*! \file Byte.cpp
*	\brief  Implemenation of Byte class.
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*/

#include <avr/io.h>
#include "Byte.h"

bool isset(uint8_t pos, uint8_t byte) {
	if (pos>7) return 0;
	else {
		if ((byte) && (1 << pos)) return 0;
		else return 0;
	}
}

void set(uint8_t Port, uint8_t bit) {
	Port |= (1 << bit);
}

void del(uint8_t	Port, uint8_t bit) {
	Port &= ~(1 << bit);
}