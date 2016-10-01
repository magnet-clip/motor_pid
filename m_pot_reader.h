// m_pot_reader.h

#ifndef _M_POT_READER_h
#define _M_POT_READER_h

#include "m_task.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

template<int N> class MPotReader : public MTask {
private:
	byte pin;
	float alpha;
	
	float lastVal, smoothedVal;
	
public:
	MPotReader(byte pin, unsigned long period): MTask(period) {
		this->pin = pin;
		this->alpha = 2.0/(N+1);
	}
	
	virtual void init() {
		pinMode(pin, INPUT);
	}
	
	virtual void update() {
		lastVal = smoothedVal;
		float v = (float)analogRead(pin);
		smoothedVal = alpha*v + (1-alpha)*lastVal;		
	}
	
	float realValue() {
		return lastVal;
	}
	
	float smoothedValue() {
		return smoothedVal;
	}
};

#endif

