#pragma once
#include <iostream>
#include "Scheduler.h"
#include "Patient.h"

using namespace std;

class UI
{
private:

public:
	void print(int timestep, LinkedQueue<Patient*>& allPatients,
        EarlyPriQueue<Patient*>& earlyPatients,
        priQueue<Patient*>& latePatients,
        priQueue<Patient*>& inTreatmentPatients,
        ArrayStack<Patient*>& finishedPatients,
        EUWaitingQueue<Patient*>& uWaiting,
        EUWaitingQueue<Patient*>& eWaiting,
        XWaitingQueue<Patient*>& xWaiting,
        LinkedQueue<Resource*>& uDevices,
        LinkedQueue<Resource*>& eDevices,
        LinkedQueue<Resource*>& xRooms)
	{
		cout << "Current Timestep: " << timestep << endl;
		
	}
};