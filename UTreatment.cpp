#include "UTreatment.h"

UTreatment::UTreatment(char type, int duration) : Treatment(type, duration)
{
}

bool UTreatment::canAssign()
{
	return false;
}

void UTreatment::addToWait(Patient* patient, Scheduler& scheduler)
{
	scheduler.addUWaiting(patient);
}
