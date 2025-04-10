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
			scheduler.movePatient(timeStep);

		int x = rand() % 100;

		scheduler.simulate(x);
			
			
			
			
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