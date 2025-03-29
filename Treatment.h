#pragma once
#include "Resource.h"
class Treatment
{
private:
	int duration;
	int st;
	Resource* assigned_Resource;

public:
	int getDuration();

	int getSt();

	bool canAssign();

	void addToWait();
};

