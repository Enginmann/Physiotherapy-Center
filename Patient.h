#pragma once

#include "Queue.h"
#include "Treatment.h"

class Patient
{
private:
	int id;
	int pt;
	int vt;
	int type; // 0->Normal, 1->Recovering
	int status;

	LinkedQueue<Treatment *> reqTreatments;

public:
	int getId();
	int getPt();
	int getVt();
	int getType();
	int getStatus();

	// Treatment * getTreatment();
};