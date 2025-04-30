#pragma once
#include "Treatment.h"
#include "Patient.h"
#include "Scheduler.h"
class UTreatment : public Treatment
{
public:
	UTreatment(char type, int duaration);

	virtual bool canAssign();

	virtual void addToWait(Patient* patient , Scheduler* scheduler);
};

