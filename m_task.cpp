// 
// 
// 

#include "m_task.h"

static unsigned int task_id_counter = 0;

 MTask::MTask(unsigned long period)
{
	this->period = period;
	task_id = task_id_counter++;
	prev = 0;
}

void MTask::enable()
{
	disabled = false;
}

void MTask::disable()
{
	disabled = true;
}

bool MTask::enabled()
{
	return !disabled;
}

void MTask::execute()
{
	auto curr = millis();
	unsigned long dt = curr - prev;
	if (curr - prev < period || disabled) return;
	update(dt);
	prev = curr;
}

unsigned int MTask::getId()
{
	return task_id;
}
