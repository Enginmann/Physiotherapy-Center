#include "XResource.h"
#include "iostream"

using namespace std;
XResource::XResource(int id, char type, int capacity) : Resource(id, type)
{
	this->capacity = capacity;
	this->numOfPatient = 0;
}

void XResource::print(int count)
{
	const char * resetColor = "\033[0m";
	const char * purpleColor = "\033[35m";
	
	cout << resetColor;
	cout << purpleColor;

	cout << "R" << this->id << "[" <<count << "," << capacity << "]";

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

void XResource::decNumOfPatient()
{
	numOfPatient--;
}

bool XResource::incNumOfPatient() {
	if (numOfPatient == capacity)
		return false;
	numOfPatient++;
	return true;
}
