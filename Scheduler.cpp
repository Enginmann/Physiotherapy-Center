#include "Scheduler.h"

Scheduler::Scheduler()
{
}

void Scheduler::loadAllPatients()
{
}

int Scheduler::getAllPatientsCount()
{
	return allPatients.getCount();
}

int Scheduler::getFinishedPatientsCount()
{
	return finishedPatients.getCount();
}
