// m_click_counter.h

#ifndef _M_CLICK_COUNTER_h
#define _M_CLICK_COUNTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MClickCounter {
private:
	unsigned long count = 0;
	
public:
	void increment();
	unsigned long get();
	void reset();
	
	// Interrupts-safe version of click count reader
	unsigned long getAndReset();
};

#endif

