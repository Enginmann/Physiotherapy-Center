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
        cout << endl;
        
		cout << "========= All List =========" << endl;
        cout << allPatients.getCount() << " patients remaining: ";
        allPatients.print(10);
        cout << endl;
        
		cout << "========= Waiting Lists =========" << endl;
        eWaiting.print();
        uWaiting.print();
        xWaiting.print();
        cout << endl;

		cout << "========= Early List =========" << endl;
        cout << earlyPatients.getCount() << " patients: ";
        earlyPatients.print();
        cout << endl;

		cout << "========= Late List =========" << endl;
        cout << latePatients.getCount() << " patients: ";
        latePatients.print();
        cout << endl;
        
		cout << "========= Avail E-devices =========" << endl;
        eDevices.print();

		/*cout << "========= Avail U-devices =========" << timestep << endl;
        uDevices.print();

		cout << "========= Avail X-Rooms =========" << timestep << endl;
        xRooms.print();

		cout << "========= In-treatment List =========" << timestep << endl;
        inTreatmentPatients.print();
        
		cout << "========= Finished List =========" << timestep << endl;
        finishedPatients.print();*/
	}
};