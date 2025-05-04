#include "Treatment.h"
#include "Resource.h"
#include "Patient.h"
#include "Scheduler.h"
Treatment::Treatment(char type, int duration)
{
	this->type = type;
	this->duration = duration;
	resource = nullptr;
}

char Treatment::getType()
{
	return type;
}

int Treatment::getDuration()
{
	return duration;
}

void Treatment::setDuration(int newde)
{
	duration = newde;
}

void Treatment::setSt(int st)
{
	this->st = st;
}

int Treatment::getSt()
{
	return st;
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
