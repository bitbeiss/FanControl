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

// was tut die Funktion? übergeben wird ja ein value, kein Pointer; dh die Funktion ändert den value, gibt aber nichts zurück
/*void set(uint8_t port, uint8_t bit) {
	port |= (1 << bit);
}

// Same here
void del(uint8_t port, uint8_t bit) {
	port &= ~(1 << bit);
}
*/