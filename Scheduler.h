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
#include "UI.h"

#include <string>


using namespace std;

class Scheduler
{
private:
	int pCancel;
	int pResc;
	int maxPt;
	int timeStep;
	bool over;
	string fileName;
	UI ui;

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

	void incrementTimeStep();

	int getTimeStep();

	void loadInputFile();

	int getAllPatientsCount();

	int getFinishedPatientsCount();

	void movePatientFromAll(); /// takes the patient from allPatients list and moves them to early or late or random waiting

	bool isOver();

	//void moveToWaiting(int timestep); /// moves the patient to his correct waiting list

	//void moveToTreatment(int timestep);

	void simulate(int x);

	void print();

	void inputFileName();

	void reset();

private:
	void moveToRandomWaiting(Patient * patient);
};