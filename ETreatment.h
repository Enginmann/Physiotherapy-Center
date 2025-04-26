#pragma once
#include "Treatment.h"
#include "Scheduler.h"
#include "Patient.h"
class ETreatment : public Treatment
{
public:

	ETreatment(char type, int duration);
	
	virtual bool canAssign();

	virtual void addToWait(Patient* patient, Scheduler& scheduler);

};

