#pragma once

#include "Queue.h"
#include "Treatment.h"

class Patient
{
private:
	char type;
	int id;
	int pt;
	int vt;
	int status; // 0:idle, 1:early, 2:late, 3:wait, 4:serve, 5:finish
	int ft;
	int wt;
	int tt;
	bool cancelState;
	bool rescState;

	LinkedQueue<Treatment *> reqTreatments;

public:
	Patient(int id, int pt, int vt, char type);

	int getId();
	int getPt();
	int getVt();
	int getFt();
	int getWt();
	int getTt();
	char getType();
	int getStatus();
	bool isCancel();
	bool isResc();

	void setStatus(int status);
	void setReqTreatment(Treatment * treatment);


	Treatment * getTreatment();
};