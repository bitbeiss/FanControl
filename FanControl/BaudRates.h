/*
 * BaudRates.h
 *
 * Created: 9.12.18 14:52:38
 *  Author: filmu
 */ 


#ifndef BAUDRATES_H_
#define BAUDRATES_H_

namespace BaudRates {
	enum _baud_rate_enum { _2400, _4800, _9600, _14400, _19200, _28800, _38400, _57600 };
}
typedef BaudRates::_baud_rate_enum BaudRate;



#endif /* BAUDRATES_H_ */