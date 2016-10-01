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
		
	}
};

#endif

