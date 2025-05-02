#include "Scheduler.h"
#include <string>
#include <fstream>


Scheduler::Scheduler()
{
	srand(time(0));
	timeStep = 0;
	rCount = 0;
	nCount = 0;
	earlyCount = 0;
	lateCount = 0;
	totalPenality = 0;
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
	inputFileName = ui.inputFileName();

	ifstream inFile(inputFileName + ".txt");
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

		if (pType == 'N')
			nCount++;
		else
			rCount++;

		Patient * patient = new Patient(i, pt, vt, pType);
		
		for (int j = 0; j < tNum; j++)
		{
			inFile >> tType >> tDuration;
			if (tType == 'E')
			{
				
				treatment = new ETreatment(tType, tDuration);
				patient->setReqTreatment(treatment);
			}
			else if (tType == 'U')
			{
				treatment = new UTreatment(tType, tDuration);
				patient->setReqTreatment(treatment);
			}
			else if (tType == 'X')
			{
				treatment = new XTreatment(tType, tDuration);
				patient->setReqTreatment(treatment);
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
			earlyCount++;
		}
		/// late
		else if (timeStep > patient->getPt())
		{
			patient->setStatus(2);
			latePatients.enqueue(patient, -(timeStep + (timeStep - patient->getPt()) / 2));
			lateCount++;
			totalPenality += (timeStep - patient->getPt()) / 2;
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


void Scheduler::simulate()
{
	movePatientFromAll();
	moveFromEarly();
	moveFromLate();
	assignE();
	assignU();
	assignX();
	moveFromInTreatmentToWaitOrFinish();
	print();
	timeStep++;
}

void Scheduler::print()
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
		xRooms);

	char key = ui.getKey();
	if (key == 27) // Escape key
		over = true;
}

void Scheduler::reset()
{
	return;
}

void Scheduler::addEWaiting(Patient* patient)
{
	if (patient->getStatus() == 2) {
		int penalty = (patient->getVt() - patient->getPt()) / 2+ patient->getPt();
		eWaiting.insertSorted(patient, penalty);
	}
	else if (patient->getStatus() == 4) {
		eWaiting.insertSorted(patient, patient->getPt());
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
	else if (patient->getStatus() == 4) {
		uWaiting.insertSorted(patient, patient->getPt());
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
	else if (patient->getStatus() == 4) {
		xWaiting.insertSorted(patient, patient->getPt());
	}
	else
		xWaiting.enqueue(patient);
}

void Scheduler::assignX()
{
	Patient* patient = nullptr;
	if (xWaiting.cancel(patient, pCancel))
	{
		finishedPatients.push(patient);
		patient->setStatus(5);
	}
	patient = nullptr;
	xWaiting.peek(patient);
	
	if (patient)
	{
		if (patient->getTreatment()->canAssign(this))
		{
			XResource* xroom;
			xWaiting.dequeue(patient);
			patient->getTreatment()->setSt(timeStep);
			inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
			xRooms.peek(xroom);
			xroom->incNumOfPatient();
			patient->getTreatment()->setResource(xroom);
			patient->setStatus(4);

			if (xroom->getCapacity() == xroom->getNumOfPatient())
			{
				xRooms.dequeue(xroom);
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
			patient->getTreatment()->setSt(timeStep);
			inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
			uDevices.dequeue(resource);
			patient->getTreatment()->setResource(resource);
			patient->setStatus(4);

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
			patient->getTreatment()->setSt(timeStep);
			inTreatmentPatients.enqueue(patient,-(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
			eDevices.dequeue(resource);
			patient->getTreatment()->setResource(resource);
			patient->setStatus(4);
		}
	}
}

void Scheduler::moveFromEarly()
{
	Patient* patient = nullptr;
	int pT;
	if (earlyPatients.isEmpty())
		return;
	earlyPatients.reschedule(maxPt, pResc);
	earlyPatients.peek(patient,pT);
	while (patient && patient->getPt() == timeStep)
	{
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

		patient = nullptr;
		earlyPatients.peek(patient, pT);
	}

}

void Scheduler::moveFromLate()
{
	Patient* patient = nullptr;
	int pT;
	if (latePatients.isEmpty())
		return;
	latePatients.peek(patient, pT);

	while (patient && -pT == timeStep)
	{
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

		patient = nullptr;
		latePatients.peek(patient, pT);
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

void Scheduler::moveFromInTreatmentToWaitOrFinish()
{
	Patient* patient = nullptr;
	int due;

	inTreatmentPatients.peek(patient,due);

	while (patient && - due == timeStep)
	{
		inTreatmentPatients.dequeue(patient, due);
		
		Treatment* treatment = patient->removeTreatment();
		XResource* resource = dynamic_cast<XResource*>(treatment->getResource());
		if (resource)
		{
			resource->decNumOfPatient();
			if (resource->getCapacity() - resource->getNumOfPatient()==1)
				xRooms.enqueue(resource);
		}
		else if (treatment->getResource()->getType()=='E')
		{
			eDevices.enqueue(treatment->getResource());
		}
		else
		{
			uDevices.enqueue(treatment->getResource());
		}
		
		treatment = nullptr;
		treatment = patient->getTreatment();
		
		if (!treatment)
		{
			finishedPatients.push(patient);
			patient->setStatus(5);
		}
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
				patient->getTreatment()->addToWait(patient, this);
				patient->setStatus(3);
			}
		}
		patient = nullptr;
		inTreatmentPatients.peek(patient, due);
	}
}

void Scheduler::exportOutputFile()
{
	outputFileName = ui.outputFileName();

	ofstream outFile(outputFileName + ".txt");
	if (!outFile)
		return;

	float allCount = finishedPatients.getCount();

	outFile << "PID\tPType\tPT\tVT\tFT\tWT\tTT\tCancel\tResc" << endl;

	float nWait = 0;
	float rWait = 0;
	float nTreat = 0;
	float rTreat = 0;
	float cancelCount = 0;
	float rescCount = 0;

	for (int i = 0; i < finishedPatients.getCount(); i++)
	{
		Patient * patient = nullptr;
		finishedPatients.pop(patient);

		if (patient->getType() == 'N')
		{
			nWait += patient->getWt();
			nTreat += patient->getTt();
		}
		else
		{
			rTreat += patient->getTt();
			rWait += patient->getWt();
		}

		if (patient->isCancel())
			cancelCount++;

		if (patient->isResc())
			rescCount++;

		patient->toOutFile(outFile);
	}

	outFile << endl;

	outFile << "Total number of timesteps = " << timeStep << endl;

	outFile << "Total number of all, N, and R patients = " << allCount << ", " << nCount << ", " << rCount << endl;
	
	float allWait = nWait + rWait;
	outFile << "Average total waiting time of all, N, and R patients = " << allWait / allCount << ", " << nWait / nCount << ", " << rWait / rCount << endl;

	float allTreat = nTreat + rTreat;
	outFile << "Average total treatment time of all, N, and R patients = " << allTreat / allCount << ", " << nTreat / nCount << ", " << rTreat / rCount << endl;
	
	outFile << "Percentage of patients of an accepted cancellation = " << cancelCount / allCount * 100 << endl;
	
	outFile << "Percentage of patients of an accepted rescheduling = " << rescCount / allCount * 100 << endl;
	
	outFile << "Percentage of early patients = " << earlyCount / allCount * 100 << endl;

	outFile << "Percentage of late patients = " << lateCount / allCount * 100 << endl;

	outFile << "Average late penality = " << totalPenality / lateCount << endl;

	outFile.close();
}