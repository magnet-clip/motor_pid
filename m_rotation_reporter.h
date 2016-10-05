// m_rotation_reporter.h
#include "m_task.h"
#include "m_click_counter.h"

#ifndef _M_ROTATION_REPORTER_h
#define _M_ROTATION_REPORTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class MRotationReporter : public MTask {
private:	
	MClickCounter* c1;
	MClickCounter* c2;
	
public:
	MRotationReporter(unsigned long periodicity, MClickCounter* c1, MClickCounter* c2) : MTask(periodicity) {
		this->c1 = c1;
		this->c2 = c2;
	}	

	void init() {
		Serial.begin(115200);
	}
	
protected:
	void update(unsigned long dt) {
		Serial.println("==================");
		Serial.print("Period: ");
		Serial.println(dt);
		Serial.print("C1: ");
		Serial.println(c1->getAndReset());
		Serial.print("C2: ");
		Serial.println(c2->getAndReset());
	}	
};

#endif

