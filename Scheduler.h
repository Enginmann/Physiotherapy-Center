#pragma once

#include "Patient.h"
#include "Resource.h"
#include "Queue.h"
#include "PriQueue.h"
#include "Stack.h"
#include "EarlyPriQueue.h"
#include "EUWaitingQueue.h"
#include "XWaitingQueue.h"


class Scheduler
{
private:
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

	void loadAllPatients();

	int getAllPatientsCount();

	int getFinishedPatientsCount();

	/// Phase 2 methods
};