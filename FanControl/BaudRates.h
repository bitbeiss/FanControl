/*!
 * 	\file BaudRates.h
 *	\author Clemens J. Zuzan
 *	\author Klemens Svetitsch
 *	\brief Store (and make available) possible baudrates in an enum.
 */ 


#ifndef BAUDRATES_H_
#define BAUDRATES_H_

namespace BaudRates {
	enum _baud_rate_enum { _2400, _4800, _9600, _14400, _19200, _28800, _38400, _57600 }; /*!< \enum available baudrates*/
}
typedef BaudRates::_baud_rate_enum BaudRate;

#endif /* BAUDRATES_H_ */