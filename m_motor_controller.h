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
	byte ENCODER_LINES;
	
	float realRpms, smoothedRpms, lastPid, desiredRpms, lastPidAdj;
	byte pwmSpeed;
	
public:
	MMotorController(unsigned long period, unsigned int MAX_RPM, byte ENCODER_LINES, MDcMotor* motor, MClickCounter* counter, MPid* pid, MPotReader* speed);
	
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
	float clicksToRpms(unsigned long clicks, unsigned long dt);
	
	float adcToRpms(unsigned long adcValue);
	
	
protected:
	virtual void update(unsigned long dt);
public:
};

#endif

