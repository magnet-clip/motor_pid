// m_dc_motor.h

#ifndef _M_DC_MOTOR_h
#define _M_DC_MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class MDcMotor {
private:
	byte M1_PIN, M2_PIN, PWM_PIN;
public:
	MDcMotor(byte M1_PIN, byte M2_PIN, byte PWM_PIN);
	
	void init();
	void forward();
	void reardrive();
	void halt();
	void setSpeed(byte speed);
};

#endif

