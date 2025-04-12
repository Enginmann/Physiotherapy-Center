#include "XResource.h"
#include "iostream"

using namespace std;
XResource::XResource(int id, char type, int capacity) : Resource(id, type)
{
	this->capacity = capacity;
}

void XResource::print(int count)
{
	cout << "R" << this->id << "[" <<count << "," << capacity << "]";
}

int XResource::getCapacity()
{
	return capacity;
}
