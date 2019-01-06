/*!
*	\file Prescalers.h
*	\author Clemens J. Zuzan
*	\author Klemens Svetitsch
*	\brief Header defining available prescalers. ADC or PWM prescalers are made available via namespace.
*/ 


#ifndef PRESCALERS_H_
#define PRESCALERS_H_

namespace ADCPrescalers {
	enum _adc_prescaler_enum { _2, _4, _8, _16, _32, _64, _128 };
}
typedef ADCPrescalers::_adc_prescaler_enum ADCPrescaler;



namespace PWMPrescalers {
	enum _pwm_prescaler_enum { _stop, _1, _8, _32, _64, _128, _256, _1024 };
}
typedef PWMPrescalers::_pwm_prescaler_enum PWMPrescaler;




#endif /* PRESCALERS_H_ */