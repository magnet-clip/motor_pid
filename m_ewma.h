// m_ewma.h

#include "m_smoother.h"
#ifndef _M_EWMA_h
#define _M_EWMA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

template<byte N>
class MEwma : public MSmoother {
private:
	float alpha;
	float lastVal, smoothedVal;
	
public:
	MEwma(float initVal = 0.0) {
		lastVal = initVal;
		smoothedVal = initVal;
		alpha = 2.0/(N+1);	
	}
	
	virtual float smooth(float nextValue) {
		lastVal = smoothedVal;
		smoothedVal = alpha*nextValue + (1-alpha)*lastVal;
		return smoothedVal;
	}
};

#endif

