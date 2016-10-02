// m_motor_controller.h

#include "m_task.h"
#include "m_dc_motor.h"
#include "m_click_counter.h"
#include "m_pid.h"
#include "m_pot_reader.h"
#include "m_ewma.h"

#ifndef _M_MOTOR_CONTROLLER_h
#define _M_MOTOR_CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MMotorController : public MTask {
private:
	MDcMotor* motor;
	MClickCounter* counter;
	MPid* pid;
	MPotReader* speed;
	MEwma<10> adcSmoother;
	MEwma<10> rpmSmoother;
	unsigned int MAX_RPM;
	
	float realRpms, smoothedRpms, lastPid, desiredRpms, lastPidAdj;
	byte pwmSpeed;
	
public:
	MMotorController(unsigned long period, unsigned int MAX_RPM, MDcMotor* motor, MClickCounter* counter, MPid* pid, MPotReader* speed) : MTask(period), adcSmoother(0.0), rpmSmoother(0.0) {
		this->motor = motor;
		this->counter = counter;
		this->pid = pid;
		this->speed = speed;
		this->MAX_RPM = MAX_RPM;
	}
	
	virtual void init() {
	}
	
	float getRealRpms() {
		return realRpms;
	}
	
	float getSmoothedRpms() {
		return smoothedRpms;
	}
	
	float getDesiredRpms() {
		return desiredRpms;
	}
	
	float getLastPid() {
		return lastPid;
	}
	
	float getLastPidAdj() {
		return lastPidAdj;
	}
	
	byte getLastPwm() {
		return pwmSpeed;
	}
	
private:
	float clicksToRpms(unsigned long clicks, unsigned long dt) {
		return 0.0;
	}
	
	float adcToRpms(unsigned long adcValue) {
		return adcValue * MAX_RPM / 1024.0;
	}
	
	
protected:
	virtual void update(unsigned long dt) {
		unsigned long clicks = counter->get();			// number of clicks since last check
		
		unsigned int desiredSpeedInAdcValue = speed->get();							// desired speed as told by pot
		float smoothedSpeedInAdcValue = adcSmoother.smooth(desiredSpeedInAdcValue);  // smoothed pot value (from 0 to 1024)
		desiredRpms = adcToRpms(smoothedSpeedInAdcValue);				// now it's a value from 0 to 100
		
		realRpms = clicksToRpms(clicks, dt);
		smoothedRpms = rpmSmoother.smooth(realRpms);
		
		lastPid = pid->calculate(desiredRpms, smoothedRpms); // some value to be converted to PWM
		
		// pid value is based on adc scale (0-1024)
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
};

#endif

