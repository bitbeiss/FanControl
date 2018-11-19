/*
 * Prescalers.h
 *
 * Created: 18.11.18 19:24:28
 *  Author: filmu
 */ 


#ifndef PRESCALERS_H_
#define PRESCALERS_H_

namespace ADCPrescalers {
	enum _enum { _2, _4, _8, _16, _32, _64, _128 };
}
typedef ADCPrescalers::_enum ADCPrescaler;



namespace PWMPrescalers {
	enum _enum { _stop, _1, _8, _32, _64, _128, _256, _1024 };
}
typedef PWMPrescalers::_enum PWMPrescaler;




#endif /* PRESCALERS_H_ */