#include "Scheduler.h"
#include <string>
#include <fstream>

using namespace std;

Scheduler::Scheduler()
{
}

void Scheduler::loadInputFile()
{
	/// Get the name of the input file from the user later

	ifstream inFile("test.txt");
	if (!inFile)
		return;

	string buffer;

	/// devices later
	getline(inFile, buffer);

	/// rooms and capacity later
	getline(inFile, buffer);

	/// cancel and resc
	inFile >> pCancel >> pResc;

	/// reading patients
	int num;
	inFile >> num;

	int pt, vt, tNum, tDuration;
	char pType, tType;
	Treatment * treatment;
	
	for (int i = 0; i < num; i++)
	{
		inFile >> pType >> pt >> vt >> tNum;

		Patient * patient = new Patient(i, pt, vt, pType);
		
		for (int j = 0; j < tNum; j++)
		{
			inFile >> tType >> tDuration;
			treatment = new Treatment(tType, tDuration);
			patient->setReqTreatment(treatment);
		}
		allPatients.enqueue(patient);
	}

	inFile.close();
}


int Scheduler::getAllPatientsCount()
{
	return allPatients.getCount();
}

int Scheduler::getFinishedPatientsCount()
{
	return finishedPatients.getCount();
}
