// 
// 
// 

#include "m_reporter.h"


 MReporter::MReporter(unsigned long period, MMotorController* c1, MMotorController* c2) : MTask(period)
{
	this->c1 = c1;
	this->c2 = c2;
}

void MReporter::init()
{
	Serial.begin(115200);
}

void MReporter::update(unsigned long dt)
{
	Serial.print("Not smoothed actual 1/2 RPM: ");
	Serial.print(c1->getRealRpms());
	Serial.print("/");
	Serial.println(c2->getRealRpms());
	
	
	Serial.print("Smoothed actual 1/2 & desired RPM: ");
	Serial.print(c1->getSmoothedRpms());
	Serial.print("/");
	Serial.print(c2->getSmoothedRpms());
	Serial.print(" & ");
	Serial.println(c1->getDesiredRpms());
	
	Serial.print("PIDs: ");
	Serial.print(c1->getLastPid());
	Serial.print(" ");
	Serial.println(c2->getLastPid());
	
	Serial.print("PIDs adjusted: ");
	Serial.print(c1->getLastPidAdj());
	Serial.print(" ");
	Serial.println(c2->getLastPidAdj());
	
	Serial.print("PWMs: ");
	Serial.print(c1->getLastPwm());
	Serial.print(" ");
	Serial.println(c2->getLastPwm());
	
	
	Serial.println("--------------");
}
