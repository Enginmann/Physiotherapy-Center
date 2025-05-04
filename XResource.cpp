#include "XResource.h"
#include "iostream"

using namespace std;
XResource::XResource(int id, char type, int capacity, int free, int busy, int maint, int A, int B, int C) : Resource(id, type, free,busy,maint)
{
	this->A = A;
	this->B = B;
	this->C = C;
	this->capacity = capacity;
	this->numOfPatient = 0;
} 



void XResource::print(int count)
{
	const char * resetColor = "\033[0m";
	const char * purpleColor = "\033[35m";
	
	cout << resetColor;
	cout << purpleColor;

	cout << "R" << this->id << "[" << numOfPatient << "," << capacity << "]";

	cout << resetColor;
}

int XResource::getCapacity()
{
	return capacity;
}

int XResource::getNumOfPatient()
{
	return numOfPatient;
}

int XResource::getA()
{
	return A;
}
int XResource::getB()
{
	return B;
}
int XResource::getC()
{
	return C;
}

void XResource::decNumOfPatient()
{
	if (numOfPatient == 0)
		return;
	numOfPatient--;
}

bool XResource::incNumOfPatient() {
	if (numOfPatient == capacity)
		return false;
	numOfPatient++;
	return true;
}
