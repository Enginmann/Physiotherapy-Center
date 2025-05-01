#pragma once
#include <iostream>
#include "Scheduler.h"
#include "Patient.h"
#include <conio.h>
#include <string>
#include <fstream>


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
        LinkedQueue<XResource*>& xRooms)
	{
        system("cls");

		cout << "Current Timestep: " << timestep << endl;
        cout << endl;


		cout << "========= All List =========" << endl;
        cout << allPatients.getCount() << " patients remaining: ";
        allPatients.print(10);
        cout << endl;
        
		cout << "========= Waiting Lists =========" << endl;
        cout << eWaiting.getCount() << " E-therapy patients: ";
        eWaiting.print();
        cout << uWaiting.getCount() << " U-therapy patients: ";
        uWaiting.print();
        cout << xWaiting.getCount() << " X-therapy patients: ";
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
        cout << eDevices.getCount() << " Electro devices: ";
        eDevices.print();
        cout << endl;

		cout << "========= Avail U-devices =========" << endl;
        cout << uDevices.getCount() << " Ultra devices: ";
        uDevices.print();
        cout << endl;

		cout << "========= Avail X-Rooms =========" <<  endl;
        cout << xRooms.getCount() << " rooms: ";
        xRooms.print();
        cout << endl;
        
		cout << "========= In-treatment List =========" << endl;
        cout << inTreatmentPatients.getCount() << " In-treatment patients" << ": ";
        inTreatmentPatients.print();
        cout << endl;

		cout << "========= Finished List =========" << endl;
        cout << finishedPatients.getCount() << " Finished patients" << ": ";
        finishedPatients.print();
        cout << endl;

        cout << endl;
        cout << "Press any key to Continue - Press Escape to Exit" << endl;
	}

    char getKey()
    {
        char key = 0;
        while (!(key = _getch()));
        return key;
    }

    string inputFileName()
    {
        cout << "Enter File Name (without .txt): ";
        string fileName;
        cin >> fileName;
        return fileName;
    }
};