#pragma once
#include <iostream>
#include "Scheduler.h"
#include "Patient.h"

using namespace std;

class UI
{
private:

public:
	void print(int timestep, Scheduler scheduler)
	{
		cout << "Current Timestep: " << timestep << endl;
		scheduler.print();
	}
};