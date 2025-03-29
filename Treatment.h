#pragma once
#include "Resource.h"
class Treatment
{
private:
	int duration;
	int assignment_Time;
	Resource* assigned_Resource;
public:
	int getDuration()
	{
		return duration;
	}
	int getST()
	{
		return assignment_Time;
	}
	bool canAssign()
	{

	}
	void AddToWait()
	{

	}
};

