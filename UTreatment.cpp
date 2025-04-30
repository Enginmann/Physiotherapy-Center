#include "UTreatment.h"

UTreatment::UTreatment(char type, int duration) : Treatment(type, duration)
{
}

bool UTreatment::canAssign(Scheduler* scheduler)
{
	return scheduler->isUAvailable();
}

void UTreatment::addToWait(Patient* patient, Scheduler* scheduler)
{
	scheduler->addUWaiting(patient);
}
