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

template<unsigned long MAX_RPM>
class MMotorController : public MTask {
private:
	MDcMotor* motor;
	MClickCounter* counter;
	MPid* pid;
	MPotReader* speed;
	MEwma<10> adcSmoother;
	MEwma<10> rpmSmoother;
	
public:
	MMotorController(unsigned long period, MDcMotor* motor, MClickCounter* counter, MPid* pid, MPotReader* speed) : MTask(period), adcSmoother(0.0), rpmSmoother(0.0) {
		this->motor = motor;
		this->counter = counter;
		this->pid = pid;
		this->speed = speed;
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
		
		unsigned int desiredSpeedInAdcValue = speed->get();
		float smoothedSpeedInAdcValue = adcSmoother.smooth(desiredSpeedInAdcValue);  // value from 0 to 1024
		float desiredSpeedInRpms = adcToRpms(smoothedSpeedInAdcValue);
		
		float currentSpeedInRpms = clicksToRpms(clicks, dt);
		float smoothedSpeedInRpms = rpmSmoother.smooth(currentSpeedInRpms);
		
		pid->calculate(desiredSpeedInRpms, smoothedSpeedInRpms);
	}
};

#endif

