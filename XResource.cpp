#include "XResource.h"
#include "iostream"

using namespace std;
XResource::XResource(int id, char type, int capacity) : Resource(id, type)
{
	this->capacity = capacity;
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
