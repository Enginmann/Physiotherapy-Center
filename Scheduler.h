#pragma once

#include "Patient.h"
#include "Resource.h"
#include "XResource.h"
#include "Queue.h"
#include "PriQueue.h"
#include "Stack.h"
#include "EarlyPriQueue.h"
#include "EUWaitingQueue.h"
#include "XWaitingQueue.h"

#include <string>


using namespace std;

class Scheduler
{
private:
	int pCancel;
	int pResc;
	int maxPt;

	LinkedQueue<Patient*> allPatients;
	EarlyPriQueue<Patient*> earlyPatients;
	priQueue<Patient*> latePatients;

	priQueue<Patient*> inTreatmentPatients;
	ArrayStack<Patient*> finishedPatients;

	EUWaitingQueue<Patient*> uWaiting;
	EUWaitingQueue<Patient*> eWaiting;
	XWaitingQueue<Patient*> xWaiting;

	LinkedQueue<Resource*> uDevices;
	LinkedQueue<Resource*> eDevices;
	LinkedQueue<Resource*> xRooms;

public:
	Scheduler();

	void loadInputFile(string fileName);

	int getAllPatientsCount();

	int getFinishedPatientsCount();

	void movePatientFromAll(int timestep); /// takes the patient from allPatients list and moves them to early or late or random waiting

	//void moveToWaiting(int timestep); /// moves the patient to his correct waiting list

	//void moveToTreatment(int timestep);

	void simulate(int x);

	void print();

private:
	void moveToRandomWaiting(Patient * patient);
};