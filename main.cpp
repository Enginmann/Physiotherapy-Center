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
		int x = rand() % 100;

		scheduler.print(x); /// we pass the x to the function only for demo purposes

		scheduler.movePatientFromAll();
		
		scheduler.simulate(x);			

		scheduler.incrementTimeStep();
	}
}


int main()
{
	simulate();

	cout << "========= End =========" << endl;

	return 0;
}