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

	int getDuration();

	void setSt(int st);

	int getSt();

	bool canAssign();

	void addToWait();

	Resource * getResource();

	void setResource(Resource* resource);

	void print(int count);
};