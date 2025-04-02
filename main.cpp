#include "Scheduler.h"


void simulate()
{
	/// Initializations
	int timeStep = 0;
	Scheduler scheduler;

	/// File loading
	scheduler.loadInputFile();
	int count = scheduler.getAllPatientsCount();

	/// Loop
	while (scheduler.getFinishedPatientsCount() != count)
	{
		if(count!=0)
			scheduler.addTOEarlyOrLate(timeStep);
		//scheduler.moveEearlyQueue(timeStep);
		
		//scheduler.moveEearlyQueue(timeStep);

		//scheduler.moveWQueues(timeStep);

		/// Logic
		// if () move late
		// else move early
		timeStep++;
	}

	/// Print

	/// End
}


int main()
{
	simulate();

	return 0;
}