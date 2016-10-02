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
	float lastValue, smoothedValue;
	
public:
	MEwma(float initVal = 0.0);
	virtual float smooth(float nextValue);
};

template<byte N>
MEwma<N>::MEwma(float initVal /*= 0.0*/)
{
	lastValue = initVal;
	smoothedValue = initVal;
	alpha = 2.0/(N+1);
}


template<byte N>
float MEwma<N>::smooth(float newValue)
{
	lastValue = smoothedValue;
	smoothedValue = alpha*newValue + (1-alpha)*lastValue;
	return smoothedValue;
}

#endif

