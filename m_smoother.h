// m_smoother.h

#ifndef _M_SMOOTHER_h
#define _M_SMOOTHER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class MSmoother {
	virtual float smooth(float nextValue) = 0;	
};

#endif

