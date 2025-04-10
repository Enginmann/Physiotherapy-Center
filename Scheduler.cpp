#include "Scheduler.h"
#include <string>
#include <fstream>
#include <random>
#include <ctime>

using namespace std;

Scheduler::Scheduler()
{
	srand(time(0));
}

void Scheduler::loadInputFile()
{
	/// Get the name of the input file from the user later

	ifstream inFile("test.txt");
	if (!inFile)
		return;

	

	/// devices,rooms and capacity
	int eDevice, uDevice, xRoom;
	inFile >> eDevice >> uDevice >> xRoom;
	
	for (int i = 0; i < eDevice; i++)
	{
		Resource* eResource = new Resource(i, 'E');
		eDevices.enqueue(eResource);
	}

	for (int i = 0; i < uDevice; i++)
	{
		Resource* uResource = new Resource(i, 'U');
		uDevices.enqueue(uResource);
	}

	for (int i = 0; i < xRoom; i++)
	{
		int xCap;
		inFile >> xCap;
		XResource* xR = new XResource(i, 'X', xCap);
		xRooms.enqueue(xR);
	}

	
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

void Scheduler::movePatient(int timestep)
{
	Patient* patient;
	allPatients.peek(patient);
	if (timestep == patient->getVt())
	{
		allPatients.dequeue(patient);

		if (timestep < patient->getPt()) //early
			earlyPatients.enqueue(patient, -patient->getPt());
		else if (timestep > patient->getPt()) //late
			latePatients.enqueue(patient, -(timestep + (timestep - patient->getPt()) / 2));
		else
			moveToRandomWaiting(patient);
	}
}

void Scheduler::moveToRandomWaiting(Patient * patient)
{
	int n = rand() % 100;

	if (n < 33)
		eWaiting.enqueue(patient);
	else if (n < 66)
		uWaiting.enqueue(patient);
	else
		xWaiting.enqueue(patient);
}

