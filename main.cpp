#include "Scheduler.h"


void simulate()
{
	/// Initializations
	Scheduler scheduler;


	/// File loading
	cout << "Enter File Name (without .txt): ";
	string fileName;
	cin >> fileName;
	scheduler.loadInputFile(fileName);
	int count = scheduler.getAllPatientsCount();

	/// Main Loop
	//while (scheduler.getFinishedPatientsCount() != count)
	{
		scheduler.print();
		
		scheduler.movePatientFromAll();

		int x = rand() % 100;
		cout << scheduler.getFinishedPatientsCount() << endl;

		scheduler.simulate(x);			
		
		scheduler.incrementTimeStep();
	}
}


int main()
{
	simulate();
	cout << "end" << endl;
	return 0;
}