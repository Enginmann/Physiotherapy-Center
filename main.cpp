#include "Scheduler.h"
#include "UI.h"

void simulate()
{
	/// Initializations
	int timeStep = 0;
	Scheduler scheduler;
	//UI ui;
	/// File loading
	cout << "Enter File Name (without .txt): ";
	string fileName;
	cin >> fileName;
	scheduler.loadInputFile(fileName);
	int count = scheduler.getAllPatientsCount();

	/// Main Loop
	while (scheduler.getFinishedPatientsCount() != count)
	{
		
		//ui.print(timeStep, scheduler);
		scheduler.movePatientFromAll(timeStep);

		int x = rand() % 100;
		cout << scheduler.getFinishedPatientsCount() << endl;

		scheduler.simulate(x);			
		
		timeStep++;
	}
}


int main()
{
	simulate();
	cout << "end" << endl;
	return 0;
}