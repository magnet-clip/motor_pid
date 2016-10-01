// m_motor_controller.h

#include "m_task.h"

#ifndef _M_MOTOR_CONTROLLER_h
#define _M_MOTOR_CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class MMotorController : public MTask {
private:
	
public:
	MMotorController(unsigned long period) : MTask(period) {
	}
	
	virtual void init() {
		
	}
	
	void setClicks(unsigned long clicks) {
		
	}
	
protected:
	virtual void update(unsigned long dt) {
		
	}
};

#endif

