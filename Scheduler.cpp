#include "Scheduler.h"
#include <string>
#include <fstream>


Scheduler::Scheduler()
{
	srand(time(0));
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
	Treatment* treatment = nullptr;
	
	for (int i = 0; i < num; i++)
	{
		inFile >> pType >> pt >> vt >> tNum;

		Patient * patient = new Patient(i, pt, vt, pType);
		
		for (int j = 0; j < tNum; j++)
		{
			inFile >> tType >> tDuration;
			if (tType == 'E')
			{
				
				//treatment = new ETreatment(tType, tDuration);
				patient->setReqTreatment(treatment); // enqueue treatment but does not have assigned resource
			}
			else if (tType == 'U')
			{
				//treatment = new UTreatment(tType, tDuration);
				patient->setReqTreatment(treatment); // enqueue treatment but does not have assigned resource
			}
			else if (tType == 'X')
			{
				//treatment = new XTreatment(tType, tDuration);
				patient->setReqTreatment(treatment); // enqueue treatment but does not have assigned resource
			}
			
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
			if (patient->getType() == 'R')
			{
				Treatment* treatment = patient->chooseMinLatency(eWaiting.calcTreatmentLatency(), uWaiting.calcTreatmentLatency(), xWaiting.calcTreatmentLatency());
				treatment->addToWait(patient, this);
				patient->setStatus(3);
				
				
			}
			else
			{
				Treatment* treatment = patient->getTreatment();
				treatment->addToWait(patient, this);
				patient->setStatus(3);
			}
		}

		patient = nullptr;
		allPatients.peek(patient);
	}
}


void Scheduler::simulate(int x)
{
	
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

void Scheduler::handleRPatient(Patient* patient)
{

}

void Scheduler::addEWaiting(Patient* patient)
{
	if (patient->getStatus() == 2) {
		int penalty = (patient->getVt() - patient->getPt()) / 2+ patient->getPt();
		eWaiting.insertSorted(patient, penalty);
	}
	else
	eWaiting.enqueue(patient);
}

void Scheduler::addUWaiting(Patient* patient)
{
	if (patient->getStatus() == 2) {
		int penalty = (patient->getVt() - patient->getPt()) / 2 + patient->getPt();
		uWaiting.insertSorted(patient, penalty);
	}
	else
		uWaiting.enqueue(patient);
}

void Scheduler::addXWaiting(Patient* patient)
{
	if (patient->getStatus() == 2) {
		int penalty = (patient->getVt() - patient->getPt()) / 2 + patient->getPt();
		xWaiting.insertSorted(patient, penalty);
	}
	else
		xWaiting.enqueue(patient);
}

void Scheduler::assignX()
{
	Patient* patient = nullptr;
	xWaiting.peek(patient);
	if (patient)
	{
		if (patient->getTreatment()->canAssign(this))
		{
			XResource* xroom;
			xWaiting.dequeue(patient);
			inTreatmentPatients.enqueue(patient, -patient->getTreatmentDuration());
			xRooms.peek(xroom);
			if (!xroom->incNumOfPatient())
			{
				xRooms.dequeue(xroom);
				xRooms.peek(xroom);
				if (xroom)
				{
					xroom->incNumOfPatient();
					patient->getTreatment()->setResource(xroom);
				}
			}
			else
			{
				patient->getTreatment()->setResource(xroom);
			}
		}
	}
}

void Scheduler::assignU()
{
	Patient* patient = nullptr;
	uWaiting.peek(patient);
	if (patient)
	{
		if (patient->getTreatment()->canAssign(this))
		{
			Resource* resource;
			uWaiting.dequeue(patient);
			inTreatmentPatients.enqueue(patient, -patient->getTreatmentDuration());
			uDevices.dequeue(resource);
			patient->getTreatment()->setResource(resource);

		}
	}
}

void Scheduler::assignE()
{
	Patient* patient = nullptr;
	eWaiting.peek(patient);
	if (patient)
	{
		if (patient->getTreatment()->canAssign(this))
		{
			Resource* resource;
			eWaiting.dequeue(patient);
			inTreatmentPatients.enqueue(patient,-patient->getTreatmentDuration());
			eDevices.dequeue(resource);
			patient->getTreatment()->setResource(resource);

		}
	}
}

void Scheduler::moveFromEarly()
{
	Patient* patient;
	int pT=-99;
	earlyPatients.peek(patient,pT);
	if (!(-pT == timeStep))
		return;
	earlyPatients.dequeue(patient, pT);

	if (patient->getType() == 'R')
	{
		Treatment* treatment = patient->chooseMinLatency(eWaiting.calcTreatmentLatency(), uWaiting.calcTreatmentLatency(), xWaiting.calcTreatmentLatency());
		treatment->addToWait(patient, this);
		patient->setStatus(3);

		
	}
	else
	{
		 
		patient->getTreatment()->addToWait(patient, this);
		patient->setStatus(3);
	}


}

void Scheduler::moveFromLate()
{
	Patient* patient;
	int pT = -99;
	latePatients.peek(patient, pT);
	if (!(-pT == timeStep))
		return;
	latePatients.dequeue(patient, pT);

	if (patient->getType() == 'R')
	{
		Treatment* treatment = patient->chooseMinLatency(eWaiting.calcTreatmentLatency(), uWaiting.calcTreatmentLatency(), xWaiting.calcTreatmentLatency());
		treatment->addToWait(patient, this);
		patient->setStatus(3);


	}
	else
	{

		patient->getTreatment()->addToWait(patient, this);
		patient->setStatus(3);
	}

}

bool Scheduler::isEAvailable()
{
	return !eDevices.isEmpty();
}

bool Scheduler::isUAvailable()
{
	return !uDevices.isEmpty();
}

bool Scheduler::isXAvailable()
{
	return !xRooms.isEmpty();
}
