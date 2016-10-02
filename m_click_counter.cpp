// 
// 
// 

#include "m_click_counter.h"


void MClickCounter::increment()
{
	count += 1;
}

unsigned long MClickCounter::get()
{
	return count;
}

void MClickCounter::reset()
{
	count = 0;
}

unsigned long MClickCounter::getAndReset()
{
	//noInterrupts();
	unsigned long res = get();
	reset();
	//interrupts();
	return res;
}
