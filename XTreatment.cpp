#include "XTreatment.h"


XTreatment::XTreatment(char type, int duration) : Treatment(type, duration)
{
	
}

bool XTreatment::canAssign(Scheduler* scheduler)
{
	return scheduler->isXAvailable();
}

void XTreatment::addToWait(Patient* patient, Scheduler* scheduler)
{
	scheduler->addXWaiting(patient);
}
