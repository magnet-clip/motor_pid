// m_test_motor.h
#include "m_task.h"
#include "m_pot_reader.h"
#include "m_dc_motor.h"

#ifndef _M_TEST_MOTOR_h
#define _M_TEST_MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MTestMotor: public MTask {
	private:
		MDcMotor* motor;
		MPotReader* reader;
	public:	
		MTestMotor(unsigned long period, MDcMotor* motor, MPotReader* reader) : MTask(period) {
			this->motor = motor;
			this->reader = reader;
		}
		
		void init() {
		}
		
	protected:
		void update(unsigned long dt) {
			motor->setSpeed(reader->get()/4);
		}
};

#endif

