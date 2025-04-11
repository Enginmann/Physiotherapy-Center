#include "Scheduler.h"
#include <string>
#include <fstream>


Scheduler::Scheduler()
{
	srand(time(0));

}

void Scheduler::loadInputFile(string fileName)
{
	ifstream inFile(fileName + ".txt");
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
		if (i == num - 1)
			maxPt = patient->getVt();
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

void Scheduler::movePatientFromAll(int timestep)
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

void Scheduler::simulate(int x)
{
	/// check null
	Patient* patient = nullptr;
	int temp;

	if (x < 10)
	{
		int temp;
		earlyPatients.dequeue(patient, temp);
		if (patient)
			moveToRandomWaiting(patient);
	}
	else if (x < 20)
	{
		latePatients.dequeue(patient, temp);
		if (patient)
			moveToRandomWaiting(patient);
	}
	else if (x < 40)
	{
		int n = rand() % 100;
		Patient* patient2 = nullptr;

		if (n < 33)
		{
			eWaiting.dequeue(patient);
			eWaiting.dequeue(patient2);
		}
		if (n < 66)
		{
			uWaiting.dequeue(patient);
			uWaiting.dequeue(patient2);
		}
		else
		{
			xWaiting.dequeue(patient);
			xWaiting.dequeue(patient2);
		}
		if (patient)
			inTreatmentPatients.enqueue(patient, -patient->getTreatmentDuration());
		if (patient2)
			inTreatmentPatients.enqueue(patient2, -patient2->getTreatmentDuration());
	}
	else if (x < 60)
	{
		inTreatmentPatients.dequeue(patient, temp);
		if (patient)
			finishedPatients.push(patient);
	}
	else if (x < 70)
	{
		xWaiting.cancel(patient);
		if (patient)
			finishedPatients.push(patient);
	}
	else if (x < 80)
	{
		earlyPatients.reschedule(maxPt);
	}
}

void Scheduler::print()
{
	cout << "======== ALL List =======" << endl;
	cout << allPatients.getCount() << " patients remaining: ";
	//print 10 patients
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

