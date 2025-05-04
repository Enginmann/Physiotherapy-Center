#include "XTreatment.h"


XTreatment::XTreatment(char type, int duration, LinkedQueue<Tooltreatment *> x) : Treatment(type, duration)
{
	while (!x.isEmpty())
	{
		Tooltreatment * temp;
		x.dequeue(temp);
		toolList.enqueue(temp);
	}
}

bool XTreatment::canAssign(Scheduler* scheduler)
{
	return scheduler->isXAvailable();
}

void XTreatment::addToWait(Patient* patient, Scheduler* scheduler)
{
	scheduler->addXWaiting(patient);
}
