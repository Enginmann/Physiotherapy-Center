#include "Scheduler.h"


void simulate()
{
	/// Initializations
	Scheduler scheduler;


	/// File loading
	scheduler.inputFileName();
	scheduler.loadInputFile();
	int count = scheduler.getAllPatientsCount();

	/// Main Loop
	while (!scheduler.isOver() && scheduler.getFinishedPatientsCount() != count)
	{
		scheduler.simulate();			

		scheduler.incrementTimeStep();
	}
}


int main()
{
	simulate();

	cout << "========= End =========" << endl;

	return 0;
}