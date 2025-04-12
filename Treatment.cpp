#include "Treatment.h"

Treatment::Treatment(char type, int duration)
{
	this->type = type;
	this->duration = duration;
	resource = nullptr;
}

int Treatment::getDuration()
{
	return duration;
}

void Treatment::setSt(int st)
{
	this->st = st;
}

int Treatment::getSt()
{
	return st;
}

bool Treatment::canAssign()
{
	/// logic later
	return false;
}

void Treatment::addToWait()
{
	return;
}

Resource * Treatment::getResource()
{
	return resource;
}

void Treatment::setResource(Resource* resource)
{
	this->resource = resource;
}

void Treatment::print(int count)
{
	return;
}
