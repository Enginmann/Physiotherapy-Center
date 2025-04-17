#include "Scheduler.h"
#include <string>
#include <fstream>


Scheduler::Scheduler()
{
	srand(time(0));
	//srand(0);
	timeStep = 0;
	over = false;
}

void Scheduler::incrementTimeStep()
{
	timeStep++;
}

int Scheduler::getTimeStep()
{
	return timeStep;
}

int Scheduler::getAllPatientsCount()
{
	return allPatients.getCount();
}

int Scheduler::getFinishedPatientsCount()
{
	return finishedPatients.getCount();
}

bool Scheduler::isOver()
{
	return over;
}

void Scheduler::loadInputFile()
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
			patient->setReqTreatment(treatment); // enqueue treatment but does not have assigned resource
		}
		if (i == num - 1)
			maxPt = patient->getVt();
		allPatients.enqueue(patient);
	}

	inFile.close();
}

void Scheduler::movePatientFromAll()
{
	Patient* patient = nullptr;
	allPatients.peek(patient);
	
	while (patient && patient->getVt() == timeStep)
	{
		allPatients.dequeue(patient);

		/// early
		if (timeStep < patient->getPt())
		{
			patient->setStatus(1);
			earlyPatients.enqueue(patient, -patient->getPt());
		}
		/// late
		else if (timeStep > patient->getPt())
		{
			patient->setStatus(2);
			latePatients.enqueue(patient, -(timeStep + (timeStep - patient->getPt()) / 2));
		}
		/// serve
		else
		{
			patient->setStatus(3);
			moveToRandomWaiting(patient);
		}

		patient = nullptr;
		allPatients.peek(patient);
	}
}

void Scheduler::moveToRandomWaiting(Patient * patient)
{
	if (!patient)
		return;

	int n = rand() % 100;

	/// late
	if (patient->getStatus() == 2) 
	{
		if (n < 33)
			eWaiting.insertSorted(patient, patient->getPt() + (patient->getVt() - patient->getPt()) / 2);
		else if (n < 66)
			uWaiting.insertSorted(patient, patient->getPt() + (patient->getVt() - patient->getPt()) / 2);
		else
			xWaiting.insertSorted(patient, patient->getPt() + (patient->getVt() - patient->getPt()) / 2);
	}
	/// serve
	else if (patient->getStatus() == 4)
	{
		if (n < 33)
			eWaiting.insertSorted(patient, patient->getPt());
		else if (n < 66)
			uWaiting.insertSorted(patient, patient->getPt());
		else
			xWaiting.insertSorted(patient, patient->getPt());
	}
	/// early
	else
	{
		if (n < 33)
			eWaiting.enqueue(patient);
		else if (n < 66)
			uWaiting.enqueue(patient);
		else
			xWaiting.enqueue(patient);
	}
}

void Scheduler::simulate(int x)
{
	/// check null
	Patient* patient = nullptr;
	int temp;
	
	/// early ==> random wait
	if (x < 10) 
	{
		earlyPatients.dequeue(patient, temp);
		if (patient)
		{
			moveToRandomWaiting(patient);
			patient->setStatus(3); // wait
		}
	}
	/// late ==> random wait
	else if (x < 20) 
	{
		latePatients.dequeue(patient, temp);
		if (patient)
		{
			moveToRandomWaiting(patient);
			patient->setStatus(3); // wait
		}
	}
	/// 2 patients random wait ==> in-treatment
	else if (x < 40)
	{
		int n = rand() % 100;
		Patient* patient2 = nullptr;

		if (n < 33)
		{
			eWaiting.dequeue(patient);
			eWaiting.dequeue(patient2);
		}
		else if (n < 66)
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
		{
			patient->setStatus(4); // serve
			inTreatmentPatients.enqueue(patient, -patient->getTreatmentDuration());
		}
		if (patient2)
		{
			patient2->setStatus(4); // serve
			inTreatmentPatients.enqueue(patient2, -patient2->getTreatmentDuration());
		}
	}
	/// in-treatment ==> random wait
	else if (x < 50)
	{
		inTreatmentPatients.dequeue(patient, temp);
		if (patient)
		{
			moveToRandomWaiting(patient);
			patient->setStatus(3); // wait
		}
	}
	/// in-treatment ==> finish
	else if (x < 60)
	{
		inTreatmentPatients.dequeue(patient, temp);
		if (patient)
		{
			patient->setStatus(5); // finish
			finishedPatients.push(patient);
		}
	}
	/// x-waiting ==> finish
	else if (x < 70)
	{
		xWaiting.cancel(patient);
		if (patient)
		{
			patient->setStatus(5); // finish
			finishedPatients.push(patient);
		}
	}
	/// reschedule
	else if (x < 80)
	{
		earlyPatients.reschedule(maxPt);
	}
}

void Scheduler::print(int x)
{
	ui.print(
		timeStep,
		allPatients,
		earlyPatients,
		latePatients,
		inTreatmentPatients,
		finishedPatients,
		uWaiting,
		eWaiting,
		xWaiting,
		uDevices,
		eDevices,
		xRooms,
		x /// for demo purposes
	);

	char key = ui.getKey();
	if (key == 27) // Escape key
		over = true;
}

void Scheduler::inputFileName()
{
	fileName = ui.inputFileName();
}

void Scheduler::reset()
{
	return;
}