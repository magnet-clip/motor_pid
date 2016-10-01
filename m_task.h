// m_task.h

#ifndef _M_TASK_h
#define _M_TASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

static unsigned int task_id_counter = 0;

class MTask {
private:
	unsigned long _period;
	unsigned long _prev;
	unsigned int _task_id;
public:
	MTask(unsigned long period) : _period(period), _prev(0UL) {
		_task_id = task_id_counter++;
	}
		
	virtual void init() = 0;
	virtual void update() = 0;
	
	void execute() {
		auto curr = millis();
		if (curr - _prev < _period) return;
		update();
		_prev = curr;
	}
	
	unsigned int getId() {
		return _task_id;
	}
};

#endif

