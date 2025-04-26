#include "XTreatment.h"


XTreatment::XTreatment(char type, int duration) : Treatment(type, duration)
{

}

bool XTreatment::canAssign()
{
	return false;
}

void XTreatment::addToWait(Patient* patient, Scheduler& scheduler)
{
	scheduler.addXWaiting(patient);
}
