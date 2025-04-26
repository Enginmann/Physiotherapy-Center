#include "ETreatment.h"


ETreatment::ETreatment(char type, int duration) : Treatment(type, duration)
{

}
bool ETreatment::canAssign()
{
	return false;
}

void ETreatment::addToWait(Patient* patient, Scheduler& scheduler)
{
	scheduler.addEWaiting(patient);
}


