#include "Treatment.h"

Treatment::Treatment(char type, int duration)
{
	this->type = type;
	this->duration = duration;
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
