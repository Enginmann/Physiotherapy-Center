#include "Scheduler.h"
#include <string>
#include <fstream>


Scheduler::Scheduler()
{
	srand(0);
	timeStep = 0;
	rCount = 0;
	nCount = 0;
	earlyCount = 0;
	lateCount = 0;
	totalPenality = 0;
	over = false;
	silentMode = false;
	autoMode = false;
	failedFreeDevicesCount = 0;
	EUdevicesCount = 0;
	failedBusyDevicesCount = 0;
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
	inFile >> eDevice;
	EUdevicesCount += eDevice;
	int  pFree, pBusy, maintenence;
	
	for (int i = 0; i < eDevice; i++)
	{
		inFile >> pFree >> pBusy >> maintenence;
		Resource* eResource = new Resource(i, 'E', pFree, pBusy, maintenence);
		eDevices.enqueue(eResource);
	}
	inFile >> uDevice;
	EUdevicesCount += uDevice;
	for (int i = 0; i < uDevice; i++)
	{
		inFile >> pFree >> pBusy >> maintenence;
		Resource* uResource = new Resource(i, 'U', pFree, pBusy, maintenence);
		uDevices.enqueue(uResource);
	}
	inFile >> xRoom;
	for (int i = 0; i < xRoom; i++)
	{
		int xCap;
		int A, B, C;
		inFile >> xCap >> A >> B >> C;
		XResource* xR = new XResource(i, 'X', xCap,0,0,0, A, B, C);
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
				int tool;
				inFile >> tool;

				LinkedQueue<Tooltreatment*> toolTreatments;
				for (int k = 0; k < tool; k++)
				{
					char type;
					int duration;
					inFile >> type >> duration;
					Tooltreatment * temp = new Tooltreatment(type, duration);
					toolTreatments.enqueue(temp);
				}
				treatment = new XTreatment(tType, tDuration, toolTreatments);
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

		patient->setEnterTime(timeStep);

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
			patient->setStatus(3);
			if (patient->getType() == 'R')
			{
				Treatment* treatment = patient->chooseMinLatency(eWaiting.calcTreatmentLatency(), uWaiting.calcTreatmentLatency(), xWaiting.calcTreatmentLatency());
				treatment->addToWait(patient, this);
			}
			else
			{
				Treatment* treatment = patient->getTreatment();
				treatment->addToWait(patient, this);
			}
		}

		patient = nullptr;
		allPatients.peek(patient);
	}
}

void Scheduler::simulate()
{
	movePatientFromAll();
	moveFromMaintenence();
	moveFromEarly();
	moveFromLate();
	assignE();
	assignU();
	assignX();
	checkInTreatBusy();
	moveFromInTreatmentToWaitOrFinish();
	if (!silentMode)
		print();
	else
		ui.clear();

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
		xRooms,
		maintenance,
		eInterruptedPatients,
		uInterruptedPatients);

	if (autoMode)
		return;
	
	char key = ui.getKey();
	
	if (key == 27) /// Escape key
		over = true;
	else if (key == 32) /// Space key
		silentMode = true;
	else if (key == 102) /// F key
		autoMode = true;
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
		patient->setStatus(5);
		patient->setFt(timeStep);
		patient->setIsCancel(true);
		finishedPatients.push(patient);
	}
	patient = nullptr;
	xWaiting.peek(patient);
	
	while (patient && patient->getTreatment()->canAssign(this))
	{
		XResource* xroom;
		xWaiting.dequeue(patient);
		patient->getTreatment()->setSt(timeStep);
		inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
		xRooms.peek(xroom);
		xroom->incNumOfPatient();
		patient->getTreatment()->setResource(xroom);
		patient->setStatus(4);

		int temp1 = patient->getEnterTime();
		int temp2 = patient->getWt();
		patient->setWt(temp2 + timeStep - temp1);
		patient->setEnterTime(timeStep);

		if (xroom->getCapacity() == xroom->getNumOfPatient())
		{
			xRooms.dequeue(xroom);
		}
		patient = nullptr;
		xWaiting.peek(patient);
	}
}

void Scheduler::assignU()
{
	Patient* patient = nullptr;
	Resource* resource = nullptr;
	uInterruptedPatients.peek(patient);
	if (patient)
	{
		for (;;)
		{
			resource = nullptr;
			uDevices.peek(resource);
			if (!resource)
				break;
			int random = rand() % 100;
			if (random < resource->getFree())
			{
				uDevices.dequeue(resource);
				maintenance.enqueue(resource, -(resource->getmaint() + timeStep));
				failedFreeDevicesCount++;
			}
			else
				break;
		}
		if (patient->getTreatment()->canAssign(this))
		{


			uInterruptedPatients.dequeue(patient);


			patient->getTreatment()->setSt(timeStep);
			inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
			uDevices.dequeue(resource);
			patient->getTreatment()->setResource(resource);
			patient->setStatus(4);

			int temp1 = patient->getEnterTime();
			int temp2 = patient->getWt();
			patient->setWt(temp2 + timeStep - temp1);
			patient->setEnterTime(timeStep);
		}
	}
	else
	{
		patient = nullptr;
		uWaiting.peek(patient);
		if (patient)
		{
			for (;;)
			{
				uDevices.peek(resource);
				if (!resource)
					break;
				int random = rand() % 100;
				if (random < resource->getFree())
				{
					uDevices.dequeue(resource);
					maintenance.enqueue(resource, -(resource->getmaint() + timeStep));
					failedFreeDevicesCount++;
				}
				else
					break;
			}
			if (patient->getTreatment()->canAssign(this))
			{

				
				uWaiting.dequeue(patient);
				

				patient->getTreatment()->setSt(timeStep);
				inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
				uDevices.dequeue(resource);
				patient->getTreatment()->setResource(resource);
				patient->setStatus(4);

				int temp1 = patient->getEnterTime();
				int temp2 = patient->getWt();
				patient->setWt(temp2 + timeStep - temp1);
				patient->setEnterTime(timeStep);
			}
		}
	}
}

void Scheduler::assignE()
{
	Patient* patient = nullptr;
	Resource* resource = nullptr;
	eInterruptedPatients.peek(patient);
	if (patient)
	{
		if (patient)
		{
			for (;;)
			{
				resource = nullptr;
				eDevices.peek(resource);
				if (!resource)
					break;
				int random = rand() % 100;
				if (random < resource->getFree())
				{
					eDevices.dequeue(resource);
					maintenance.enqueue(resource, -(resource->getmaint() + timeStep));
					failedFreeDevicesCount++;
				}
				else
					break;
			}
			if (patient->getTreatment()->canAssign(this))
			{



				eInterruptedPatients.dequeue(patient);

				patient->getTreatment()->setSt(timeStep);
				
				eDevices.dequeue(resource);
				patient->getTreatment()->setResource(resource);
				patient->setStatus(4);
				inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
				int temp1 = patient->getEnterTime();
				int temp2 = patient->getWt();
				patient->setWt(temp2 + timeStep - temp1);
				patient->setEnterTime(timeStep);
			}
		}
	}
	else
	{
		patient = nullptr;
		eWaiting.peek(patient);
		if (patient)
		{
			for (;;)
			{
				eDevices.peek(resource);
				if (!resource)
					break;
				int random = rand() % 100;
				if (random < resource->getFree())
				{
					eDevices.dequeue(resource);
					maintenance.enqueue(resource, -(resource->getmaint() + timeStep));
					failedFreeDevicesCount++;
				}
				else
					break;
			}
			if (patient->getTreatment()->canAssign(this))
			{


				eWaiting.dequeue(patient);
				

				patient->getTreatment()->setSt(timeStep);
				
				eDevices.dequeue(resource);
				patient->getTreatment()->setResource(resource);
				patient->setStatus(4);
				inTreatmentPatients.enqueue(patient, -(patient->getTreatmentDuration() + patient->getTreatment()->getSt()));
				int temp1 = patient->getEnterTime();
				int temp2 = patient->getWt();
				patient->setWt(temp2 + timeStep - temp1);
				patient->setEnterTime(timeStep);
			}
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
			patient->setFt(timeStep);

			int temp1 = patient->getEnterTime();
			int temp2 = patient->getTt();
			patient->setTt(temp2 + timeStep - temp1);
			patient->setEnterTime(timeStep);
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

			int temp1 = patient->getEnterTime();
			int temp2 = patient->getTt();
			patient->setTt(temp2 + timeStep - temp1);
			patient->setEnterTime(timeStep);
		}
		patient = nullptr;
		inTreatmentPatients.peek(patient, due);
	}
}

void Scheduler::moveFromMaintenence()
{
	Resource* resource = nullptr;
	int maint = -99;
	maintenance.peek(resource, maint);
	while (resource && timeStep == -maint)
	{
		
		maintenance.dequeue(resource, maint);
		if (resource->getType() == 'E')
		{
			eDevices.enqueue(resource);
		}
		else if (resource->getType() == 'U')
		{
			uDevices.enqueue(resource);
		}
		
		resource = nullptr;
		maintenance.peek(resource, maint);
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

	int finishedCount = finishedPatients.getCount();
	for (int i = 0; i < finishedCount; i++)
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
	outFile << endl;

	outFile << "Total number of timesteps = " << timeStep << endl;
	outFile << endl;

	outFile << "Total number of all, N, and R patients = " << allCount << ", " << nCount << ", " << rCount << endl;
	outFile << endl;
	
	float allWait = nWait + rWait;
	outFile << "Average total waiting time of all, N, and R patients = " << allWait / allCount << ", " << nWait / nCount << ", " << rWait / rCount << endl;
	outFile << endl;

	float allTreat = nTreat + rTreat;
	outFile << "Average total treatment time of all, N, and R patients = " << allTreat / allCount << ", " << nTreat / nCount << ", " << rTreat / rCount << endl;
	outFile << endl;
	
	outFile << "Percentage of patients of an accepted cancellation = " << cancelCount / allCount * 100 << " %" << endl;
	outFile << endl;
	
	outFile << "Percentage of patients of an accepted rescheduling = " << rescCount / allCount * 100 << " %" << endl;
	outFile << endl;
	
	outFile << "Percentage of early patients = " << earlyCount / allCount * 100 << " %" << endl;
	outFile << endl;

	outFile << "Percentage of late patients = " << lateCount / allCount * 100 << " %" << endl;
	outFile << endl;

	outFile << "Average late penalty = " << totalPenality / lateCount << endl;
	outFile << endl;

	outFile << "Percentage of free devices that failed = " << failedFreeDevicesCount / EUdevicesCount * 100 << " %" << endl;
	outFile << endl;

	outFile << "Percentage of busy devices that failed = " << failedBusyDevicesCount / EUdevicesCount * 100 << " %" << endl;

	outFile.close();
}

void Scheduler::checkInTreatBusy()
{
	priQueue<Patient*>temp;
	Patient* patient = nullptr;
	bool done = false;
	int random = rand() % 100;
	int incount = inTreatmentPatients.getCount();
	int tev,count=0;
	int carrier;
	while (!done&&(count!= incount)) {
		int random2 = rand() % inTreatmentPatients.getCount();
		for (int i = 0; i <= random2; i++) {
			inTreatmentPatients.dequeue(patient,tev);
			if (!(i == random2))
				temp.enqueue(patient,tev);

		}
		if (patient && patient->getTreatment()->getResource()->getType() == 'X') {
			temp.enqueue(patient, tev);
			while (inTreatmentPatients.dequeue(patient, tev))
				temp.enqueue(patient, tev);
			while (temp.dequeue(patient, tev)) {
				if (patient->getTreatment()->getResource()->getType() == 'X')
					count++;
				inTreatmentPatients.enqueue(patient, tev);
			}
		}
		else 
		{
			if (patient && random < patient->getTreatment()->getResource()->getBusy()) {
				patient->getTreatment()->setDuration(patient->getTreatment()->getDuration() - (timeStep - patient->getTreatment()->getSt()));
				if(patient->getTreatment()->getType()=='E')
					eInterruptedPatients.enqueue(patient);
				else if(patient->getTreatment()->getType()=='U')
					uInterruptedPatients.enqueue(patient);

				maintenance.enqueue(patient->getTreatment()->getResource(), -(patient->getTreatment()->getResource()->getmaint() + timeStep));
				patient->getTreatment()->setResource(nullptr);
				while (inTreatmentPatients.dequeue(patient, tev))
					temp.enqueue(patient, tev);
				while (temp.dequeue(patient, tev)) {
					if (patient->getTreatment()->getResource()->getType() == 'X')
						count++;
					inTreatmentPatients.enqueue(patient, tev);
				}
				failedBusyDevicesCount++;
			}
			else if(patient)
			{
				temp.enqueue(patient, tev);
				while (inTreatmentPatients.dequeue(patient, tev))
					temp.enqueue(patient, tev);
				while (temp.dequeue(patient, tev)) {
					if (patient->getTreatment()->getResource()->getType() == 'X')
						count++;
					inTreatmentPatients.enqueue(patient, tev);
				}

			}
			done = true;

		}
	}
}
