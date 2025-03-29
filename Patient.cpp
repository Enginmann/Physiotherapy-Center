#include "Patient.h"
#include <iostream>
#include <fstream>

using namespace std;

Patient::Patient(int id, int pt, int vt, int type)
{
	this->id = id;
	this->pt = pt;
	this->vt = vt;
	this->type = type;
	wt = 0;
	tt = 0;
	ft = 0;
	status = 0; // Idle
	cancelState = false;
	rescState = false;
}

int Patient::getId()
{
	return id;
}

int Patient::getPt()
{
	return pt;
}

int Patient::getVt()
{
	return vt;
}

int Patient::getFt()
{
	return ft;
}

int Patient::getWt()
{
	return wt;
}

int Patient::getTt()
{
	return tt;
}

char Patient::getType()
{
	return type;
}

int Patient::getStatus()
{
	return status;
}

void Patient::setStatus(int status)
{
	this->status = status;
}

void Patient::setReqTreatment(Treatment * treatment)
{
	reqTreatments.enqueue(treatment);
}

bool Patient::isCancel()
{
	return cancelState;
}

bool Patient::isResc()
{
	return rescState;
}

Treatment * Patient::getTreatment()
{
	Treatment * treatment = nullptr;
	reqTreatments.dequeue(treatment);
	return treatment;
}

fstream & operator<<(fstream & out, Patient * patient)
{
	out << patient->getId() << '\t';
	out << patient->getType() << '\t';
	out << patient->getPt() << '\t';
	out << patient->getVt() << '\t';
	out << patient->getFt() << '\t';
	out << patient->getWt() << '\t';
	out << patient->getTt() << '\t';
	
	if (patient->isCancel())
		out << 'T' << '\t';
	else
		out << 'F' << '\t';

	if (patient->isResc())
		out << 'T' << '\t';
	else
		out << 'F' << '\t';
	cout << endl;

	return out;
}