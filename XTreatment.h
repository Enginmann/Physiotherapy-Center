#pragma once
#include "Treatment.h"
#include "Patient.h"
#include "Scheduler.h"
#include "ToolTreatment.h"
#include "Queue.h"

class XTreatment : public Treatment
{
	LinkedQueue<Tooltreatment *> toolList;
public:
	XTreatment(char type, int duration, LinkedQueue<Tooltreatment*> x);

	virtual bool canAssign(Scheduler* scheduler);

	virtual void addToWait(Patient* patient, Scheduler* scheduler);
};

