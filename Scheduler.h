#pragma once

#include "Patient.h"
#include "Resource.h"
#include "Queue.h"
#include "PriQueue.h"
#include "Stack.h"
#include "EarlyPriQueue.h"
#include "EUWaitingQueue.h"
#include "XWaitingQueue.h"
#include "Patient.h"


class Scheduler
{
private:
	int pCancel;
	int pResc;

	LinkedQueue<Patient *> allPatients;
	EarlyPriQueue<Patient *> earlyPatients;
	priQueue<Patient *> latePatients;
	
	priQueue<Patient *> inTreatmentPatients;
	ArrayStack<Patient *> finishedPatients;

	EUWaitingQueue<Patient *> uWaiting;
	EUWaitingQueue<Patient *> eWaiting;
	XWaitingQueue<Patient *> xWaiting;

	LinkedQueue<Resource *> uDevices;
	LinkedQueue<Resource *> eDevices;
	LinkedQueue<Resource *> xRooms;

public:
	Scheduler();

	void loadInputFile();

	int getAllPatientsCount();

	int getFinishedPatientsCount();

	void addTOEarlyOrLate(int timestep);

	/// Phase 2 methods
};