#include "XResource.h"


XResource::XResource(int id, char type, int capacity) : Resource(id, type)
{
	this->capacity = capacity;
}

int XResource::getCapacity()
{
	return capacity;
}
