#pragma once

#include "Resource.h"

class XResource : public Resource
{
private:
	int capacity;
	int numOfPatient;

public:
	XResource(int id, char type, int capacity, int free, int busy, int maint);
	void print(int count = 0);
	int getCapacity();
	void decNumOfPatient();
	bool incNumOfPatient();
	int getNumOfPatient();
};

