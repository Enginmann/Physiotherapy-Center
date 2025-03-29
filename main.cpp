#include "Scheduler.h"


void simulate()
{
	/// Initializations
	int timeStep = 0;
	Scheduler scheduler;

	/// File loading
	scheduler.loadAllPatients();
	int count = scheduler.getAllPatientsCount();

	/// Loop
	while (scheduler.getFinishedPatientsCount() != count)
	{
		/// Logic
		// if () move late
		// else move early
	}

	/// Print

	/// End
}


int main()
{
	return 0;
}