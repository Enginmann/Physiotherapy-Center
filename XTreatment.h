#pragma once
#include "Treatment.h"
#include "Patient.h"
#include "Scheduler.h"
class XTreatment : public Treatment
{
public:
	XTreatment(char type, int duration);

	virtual bool canAssign(Scheduler* scheduler);

	virtual void addToWait(Patient* patient, Scheduler* scheduler);
};

