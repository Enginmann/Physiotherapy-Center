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
#include "Treatment.h"
#include "ETreatment.h"
#include "UTreatment.h"
#include "XTreatment.h"

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
	float rCount;
	float nCount;
	float earlyCount;
	float lateCount;
	float totalPenality;
	bool silentMode;
	bool autoMode;

	string inputFileName;
	string outputFileName;
	UI ui;

	LinkedQueue<Patient*> allPatients;		/// patients are loaded sorted according to their vt from the input file
	EarlyPriQueue<Patient*> earlyPatients;	/// patients are sorted according to their pt, reschedule
	priQueue<Patient*> latePatients;		/// patients are sorted according to their pt + penality

	priQueue<Patient*> inTreatmentPatients;	/// patients are sorted according to their treatment duration
	ArrayStack<Patient*> finishedPatients;	/// patients are pushed to the stack in order according to their finish time

	EUWaitingQueue<Patient*> uWaiting;		/// patients are sorted according to their pt, insertSorted
	EUWaitingQueue<Patient*> eWaiting;		/// patients are sorted according to their pt, insertSorted
	XWaitingQueue<Patient*> xWaiting;		/// patients are sorted according to their pt, insertSorted, cancel

	LinkedQueue<Resource*> uDevices;		/// devices are inserted whenever they get available
	LinkedQueue<Resource*> eDevices;		/// devices are inserted whenever they get available
	LinkedQueue<XResource*> xRooms;			/// devices are inserted whenever they get available

public:
	Scheduler();

	void incrementTimeStep();

	int getTimeStep();

	bool loadInputFile();

	int getAllPatientsCount();

	int getFinishedPatientsCount();

	void movePatientFromAll(); /// takes the patient from allPatients list and moves them to early or late or random waiting

	bool isOver();

	void simulate();

	void print();

	void reset();
	
	void addEWaiting(Patient* patient);

	void addUWaiting(Patient* patient);

	void addXWaiting(Patient* patient);

	void assignX();
	void assignU();
	void assignE();

	void moveFromEarly();
	void moveFromLate();
	bool isEAvailable();
	bool isUAvailable();
	bool isXAvailable();
	void moveFromInTreatmentToWaitOrFinish();

	void exportOutputFile();
};