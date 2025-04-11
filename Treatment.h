#pragma once
#include "Resource.h"
class Treatment
{
private:
	char type;
	int duration;
	int st;
	Resource* assigned_Resource;

public:
	Treatment(char type, int duration);

	int getDuration();

	void setSt(int st);

	int getSt();

	bool canAssign();

	void addToWait();
	void print(int count);
};