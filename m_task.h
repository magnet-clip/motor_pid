// m_task.h

#ifndef _M_TASK_h
#define _M_TASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class MTask {
private:
	unsigned long period;
	unsigned int task_id;
	unsigned long prev;
	bool disabled = false;

public:
	MTask(unsigned long period);
		
	virtual void init() = 0;
	
	void enable();
	void disable();
	bool enabled();
	
	void execute();
	
	unsigned int getId();
protected:
	virtual void update(unsigned long dt) = 0;

};

#endif

