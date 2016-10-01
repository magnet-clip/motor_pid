// m_pot_reader.h

#ifndef _M_POT_READER_h
#define _M_POT_READER_h

#include "m_task.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MPotReader : public MTask {
private:
	byte pin;
	unsigned int adcValue;
	
public:
	MPotReader(byte pin, unsigned long period): MTask(period) {
		this->pin = pin;
	}
	
	virtual void init() {
		pinMode(pin, INPUT);
	}
	
	unsigned int get() {
		return adcValue;
	}
	
	
protected:
	virtual void update(unsigned long dt) {
		adcValue = analogRead(pin);
	}
};

#endif

