// m_pid.h

#ifndef _M_PID_h
#define _M_PID_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MPid {
	private:
		float p, i, d; // coefficient
		float acc;		// accumulated error
		float err;		// last error
		
		float max_acc, max_pid;

	public:
		MPid(float kp, float ki, float kd, float max_pid, float max_acc) {
			p = kp;
			i = ki;
			d = kd;
			this->max_acc = max_acc;
			this->max_pid = max_pid;
			
			acc = 0.0;
			err = 0.0;
		}
	
		float calculate(float desired, float actual) {
			float error = desired - actual;
			
			acc += error;
			
			float p_term = p * error;
			float i_term = i * constrain(acc, -max_acc, max_acc); 
			float d_term = d * (error - err);
			
			err = error;
			
			float pid = p_term + i_term + d_term;
			return constrain(pid, -max_pid, max_pid);
		}			
};

#endif

