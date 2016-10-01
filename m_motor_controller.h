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
	
private:
	float clicksToRpms(unsigned long clicks, unsigned long dt) {
		return 0.0;
	}
	
	float adcToRpms(unsigned long adcValue) {
		return  adcValue * MAX_RPM / 1024.0;
	}
	
	
protected:
	virtual void update(unsigned long dt) {
		unsigned long clicks = counter->getAndReset();			// number of clicks since last check
		
		unsigned int desiredSpeedInAdcValue = speed->get();							// desired speed as told by pot
		float smoothedSpeedInAdcValue = adcSmoother.smooth(desiredSpeedInAdcValue);  // smoothed pot value (from 0 to 1024)
		float desiredSpeedInRpms = adcToRpms(smoothedSpeedInAdcValue);				// now it's a value from 0 to 100
		
		float currentSpeedInRpms = clicksToRpms(clicks, dt);
		float smoothedSpeedInRpms = rpmSmoother.smooth(currentSpeedInRpms);
		
		float signal = pid->calculate(desiredSpeedInRpms, smoothedSpeedInRpms); // some value to be converted to PWM
		
		// pid value is based on adc scale (0-1024)
		// yet it might be either positive or negative
		
		if (signal < 0) {
			motor->reardrive();
			signal = -signal;
		}
		
		signal = constrain(signal, 0, 1024);
		
		byte pwmSpeed = round(signal/4); //round(signal) >> 2;
		motor->setSpeed(pwmSpeed);
	}
};

#endif

