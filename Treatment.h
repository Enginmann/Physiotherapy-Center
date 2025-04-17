#pragma once
#include "Resource.h"
class Treatment
{
private:
	char type;
	int duration;
	int st;
	Resource* resource;

public:
	Treatment(char type, int duration);
	char getType();
	int getDuration();

	void setSt(int st);

	int getSt();

	virtual bool canAssign(); /// phase 2

	virtual void addToWait(); /// phase 2

	Resource * getResource();

	void setResource(Resource* resource);

	void print(int count);
};