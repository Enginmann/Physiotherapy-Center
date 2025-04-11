#include "Patient.h"
#include <iostream>
#include <fstream>

using namespace std;

Patient::Patient(int id, int pt, int vt, char type)
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

void Patient::setPt(int pt)
{
	this->pt = pt;
}

void Patient::setReqTreatment(Treatment * treatment)
{
	reqTreatments.enqueue(treatment);
}

int Patient::getTreatmentDuration()
{
	/// check null
	Treatment * treatment = nullptr;
	reqTreatments.peek(treatment);
	if (!treatment)
		return -1;
	return treatment->getDuration();
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

void Patient::print(int count)
{
	// 0:idle, 1:early, 2:late, 3:wait, 4:serve, 5:finish
	if (status == 0) // idle
		cout << "P" << id << "_" << vt;
	else if (
		status == 1 || // early
		status == 2 || // late
		status == 3 || // wait
		status == 5	 // finish
		)
		cout << id;
	else			// serve
	{
		Treatment * treatment = nullptr;
		reqTreatments.peek(treatment);
		if (!treatment)
			return;
		Resource * resource = treatment->getResource();
		if (!resource)
			return;
		cout << "P" << id << "_" << resource->getType() << resource->getId();
	}
}

//ostream & operator<<(ostream & out, Patient * patient)
//{
//	// 0:idle, 1:early, 2:late, 3:wait, 4:serve, 5:finish
//	if (patient->getStatus() == 0) // idle
//		out << "P" << patient->getId() << "_" << patient->getVt() << ", ";
//	else if (
//		patient->getStatus() == 1 || // early
//		patient->getStatus() == 2 || // late
//		patient->getStatus() == 3 || // wait
//		patient->getStatus() == 5	 // finish
//		)
//		out << patient->getId() << ", ";
//	else							 // serve
//		out << "P" << patient->getId() << "_";
//
//	return out;
//}

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