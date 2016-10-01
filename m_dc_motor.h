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
	MDcMotor(byte M1_PIN, byte M2_PIN, byte PWM_PIN) {
		this->M1_PIN = M1_PIN;
		this->M2_PIN = M2_PIN;
		this->PWM_PIN = PWM_PIN;
	}
	
	void init() {
		pinMode(M1_PIN, OUTPUT);
		pinMode(M2_PIN, OUTPUT);
		pinMode(PWM_PIN, OUTPUT);
	}
	
	void forward() {
		digitalWrite(M1_PIN, HIGH);
		digitalWrite(M2_PIN, LOW);
	}

	void reardrive() {
		digitalWrite(M1_PIN, LOW);
		digitalWrite(M2_PIN, HIGH);
	}

	void halt() {
		digitalWrite(M1_PIN, LOW);
		digitalWrite(M2_PIN, LOW);
	}
	
	void setSpeed(byte speed) {
		analogWrite(PWM_PIN, speed);
	}
};

#endif

