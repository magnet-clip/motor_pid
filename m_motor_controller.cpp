// 
// 
// 

#include "m_motor_controller.h"

 MMotorController::MMotorController(unsigned long period, unsigned int MAX_RPM, byte ENCODER_LINES, MDcMotor* motor, MClickCounter* counter, MPid* pid, MPotReader* speed) : MTask(period), adcSmoother(0.0), rpmSmoother(0.0)
{
	this->motor = motor;
	this->counter = counter;
	this->pid = pid;
	this->speed = speed;
	this->MAX_RPM = MAX_RPM;
	this->ENCODER_LINES = ENCODER_LINES;
}

float MMotorController::clicksToRpms(unsigned long clicks, unsigned long dt)
{
	return clicks*(60000.0/(ENCODER_LINES*dt));
}

float MMotorController::adcToRpms(unsigned long adcValue)
{
	return adcValue * MAX_RPM / 1024.0;
}

void MMotorController::update(unsigned long dt)
{
	unsigned long clicks = counter->get();			// number of clicks since last check
	
	unsigned int desiredSpeedInAdcValue = speed->get();							// desired speed as told by pot
	float smoothedSpeedInAdcValue = adcSmoother.smooth(desiredSpeedInAdcValue);  // smoothed pot value (from 0 to 1024)
	desiredRpms = adcToRpms(smoothedSpeedInAdcValue);				// now it's a value from 0 to 100
	
	realRpms = clicksToRpms(clicks, dt);
	smoothedRpms = rpmSmoother.smooth(realRpms);
	
	lastPid = pid->calculate(desiredRpms, smoothedRpms); // some value to be converted to PWM
	
	// pid value is based on ADC scale (0-1024)
	// yet it might be either positive or negative
	
	lastPidAdj = lastPid;
	if (lastPid < 0) {
		motor->reardrive();
		lastPidAdj = -lastPid;
	} 
	
	lastPidAdj = constrain(lastPidAdj, 0, 1023);
	
	pwmSpeed = lastPidAdj/4.0;
	motor->setSpeed(pwmSpeed);
}
