#include "ETreatment.h"


ETreatment::ETreatment(char type, int duration) : Treatment(type, duration)
{

}
bool ETreatment::canAssign(Scheduler* scheduler)
{
	return scheduler->isEAvailable();
}

void ETreatment::addToWait(Patient* patient, Scheduler* scheduler)
{
	scheduler->addEWaiting(patient);
}


