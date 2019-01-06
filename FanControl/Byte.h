/*! \file Byte.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch	
*	\brief Contains several methods not necessarily bound to a class.
*/

#pragma once
#include <avr/io.h>

bool isset(uint8_t pos, uint8_t byte); //!<Methods return true if a bit at a given position was set in a byte.
