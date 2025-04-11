#pragma once
#include <iostream>
#include "Scheduler.h"
#include "Patient.h"

using namespace std;

class UI
{
private:

public:
	void print(
        int timestep, 
        LinkedQueue<Patient*>& allPatients,
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
        
		cout << "========= All List =========" << timestep << endl;
        allPatients.print();
        
		/*cout << "========= Waiting Lists =========" << timestep << endl;
        uWaiting.print();
        eWaiting.print();
        xWaiting.print();

		cout << "========= Early List =========" << timestep << endl;
        earlyPatients.print();

		cout << "========= Late List =========" << timestep << endl;
        latePatients.print();
        
		cout << "========= Avail E-devices =========" << timestep << endl;
        eDevices.print();

		cout << "========= Avail U-devices =========" << timestep << endl;
        uDevices.print();

		cout << "========= Avail X-Rooms =========" << timestep << endl;
        xRooms.print();

		cout << "========= In-treatment List =========" << timestep << endl;
        inTreatmentPatients.print();
        
		cout << "========= Finished List =========" << timestep << endl;
        finishedPatients.print();*/
	}
};