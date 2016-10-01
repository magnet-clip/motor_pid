// m_reporter.h

#include "m_task.h"
#include "m_motor_controller.h"

#ifndef _M_REPORTER_h
#define _M_REPORTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MReporter : public MTask {
private:
	MMotorController* c1;
	MMotorController* c2;
public:
	MReporter(unsigned long period, MMotorController* c1, MMotorController* c2) : MTask(period) {
		this->c1 = c1;
		this->c2 = c2;
	}

	virtual void init() {
		Serial.begin(57600);
	}
	
	virtual void update(unsigned long dt) {
		Serial.print("Not smoothed actual 1/2 RPM: ");
		Serial.print(c1->getRealRpms());
		Serial.print("/");
		Serial.println(c2->getRealRpms());
		
		
		Serial.print("Smoothed actual 1/2 & desired RPM: ");
		Serial.print(c1->getSmoothedRpms());
		Serial.print("/");
		Serial.print(c1->getSmoothedRpms());
		Serial.print(" & ");
		Serial.println(c1->getDesiredRpms());
		
		Serial.print("PIDs: ");
		Serial.print(c1->getLastPid());
		Serial.print(" ");
		Serial.println(c2->getLastPid());
		
		Serial.print("PIDs adjusted: ");
		Serial.print(c1->getLastPidAdj());
		Serial.print(" ");
		Serial.println(c2->getLastPidAdj());
		
		Serial.print("PWMs: ");
		Serial.print(c1->getLastPwm());
		Serial.print(" ");
		Serial.println(c2->getLastPwm());
		
		
		Serial.println("--------------");		
	}
};

#endif

