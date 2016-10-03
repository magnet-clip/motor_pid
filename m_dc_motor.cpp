// 
// 
// 

#include "m_dc_motor.h"


 MDcMotor::MDcMotor(byte M1_PIN, byte M2_PIN, byte PWM_PIN)
{
	this->M1_PIN = M1_PIN;
	this->M2_PIN = M2_PIN;
	this->PWM_PIN = PWM_PIN;
}

void MDcMotor::init()
{
	pinMode(M1_PIN, OUTPUT);
	pinMode(M2_PIN, OUTPUT);
	pinMode(PWM_PIN, OUTPUT);
}

void MDcMotor::forward()
{
	digitalWrite(M1_PIN, HIGH);
	digitalWrite(M2_PIN, LOW);
}

void MDcMotor::reardrive()
{
	digitalWrite(M1_PIN, LOW);
	digitalWrite(M2_PIN, HIGH);
}

void MDcMotor::halt()
{
	digitalWrite(M1_PIN, LOW);
	digitalWrite(M2_PIN, LOW);
}

void MDcMotor::setSpeed(byte speed)
{
	analogWrite(PWM_PIN, speed);
}
