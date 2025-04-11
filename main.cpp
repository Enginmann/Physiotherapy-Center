#include "Scheduler.h"
#include "UI.h"

void simulate()
{
	/// Initializations
	int timeStep = 0;
	Scheduler scheduler;

	/// File loading
	cout << "Enter File Name (without .txt): ";
	string fileName;
	cin >> fileName;
	scheduler.loadInputFile(fileName);
	int count = scheduler.getAllPatientsCount();

	/// Main Loop
	while (scheduler.getFinishedPatientsCount() != count)
	{
		scheduler.movePatientFromAll(timeStep);

		int x = rand() % 100;

		scheduler.simulate(x);			
		
		timeStep++;
	}
}


int main()
{
	simulate();

	return 0;
}