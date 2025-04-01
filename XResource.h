#pragma once

#include "Resource.h"

class XResource : public Resource
{
private:
	int capacity;

public:
	XResource(int id, char type, int capacity);

	int getCapacity();
};