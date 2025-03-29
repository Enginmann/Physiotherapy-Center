#pragma once
#include "Resource.h"
class Treatment
{
private:
	int duration;
	int assignment_Time;
	Resource* assigned_Resource;

public:
	int getDuration();

	int getST();

	bool canAssign();

	void AddToWait();
};

