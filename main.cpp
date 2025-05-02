#include "Scheduler.h"


void simulate()
{
	/// Initializations
	Scheduler scheduler;

	/// File loading
	scheduler.loadInputFile();

	int count = scheduler.getAllPatientsCount();

	/// Main Loop
	while (!scheduler.isOver() && scheduler.getFinishedPatientsCount() != count)
		scheduler.simulate();			

	scheduler.exportOutputFile();
}


int main()
{
	simulate();

	cout << "========= End =========" << endl;

	return 0;
}