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
	while (!scheduler.isOver() && scheduler.getFinishedPatientsCount() != count)
	{
		scheduler.movePatientFromAll();

		int x = rand() % 100;

		scheduler.simulate(x);			
		
		scheduler.print();

		scheduler.incrementTimeStep();
	}
}


int main()
{
	simulate();

	cout << "========= End =========" << endl;
	int i;
	cin >> i;
	return 0;
}