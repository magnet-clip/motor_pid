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
	unsigned long period;
	unsigned int task_id;
	unsigned long prev;
	bool disabled = false;

public:
	MTask(unsigned long period) {
		this->period = period;
		task_id = task_id_counter++;
		prev = 0;
	}
		
	virtual void init() = 0;
	
	void enable() {
		disabled = false;
	}
	
	void disable() {
		disabled = true;
	}

	
	bool enabled() {
		return !disabled;
	}
	
	void execute() {
		auto curr = millis();
		unsigned long dt = curr - prev;
		if (curr - prev < period || disabled) return;
		update(dt);
		prev = curr; 
	}
	
	unsigned int getId() {
		return task_id;
	}
protected:
	virtual void update(unsigned long dt) = 0;

};

#endif

