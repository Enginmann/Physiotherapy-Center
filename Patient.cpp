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

Treatment * Patient::removeTreatment()
{
	Treatment * treatment = nullptr;
	reqTreatments.dequeue(treatment);
	return treatment;
}

bool Patient::canCancel(){// for phase 2
	Treatment* temp=nullptr;
	reqTreatments.peek(temp);
	if (!temp)return 0;
	return (reqTreatments.getCount() == 1) && (temp->getType() == 'X');
}

void Patient::chooseMinLatency(int eWaiting, int uWaiting, int xWaiting)
{
	if (reqTreatments.getCount() == 1 || reqTreatments.getCount() == 0)
		return;
	if (reqTreatments.getCount() == 2)
	{
		Treatment* treatment = nullptr;
		reqTreatments.peek(treatment);
		char type1 = treatment->getType();
		int latency1 = 0;
		if (type1 == 'E')
			latency1 = eWaiting;
		else if (type1 == 'U')
			latency1 = uWaiting;
		else if (type1 == 'X')
			latency1 = xWaiting;

		LinkedQueue<Treatment*>Q;
		reqTreatments.dequeue(treatment);
		Q.enqueue(treatment);
		reqTreatments.peek(treatment);
		char type2 = treatment->getType();
		int latency2 = 0;
		if (type2 == 'E')
			latency2 = eWaiting;
		else if (type2 == 'U')
			latency2 = uWaiting;
		else if (type2 == 'X')
			latency2 = xWaiting;

		int min = latency1;
		if (latency2 < min)
			min = latency2;

		if (min == latency2)
		{
			reqTreatments.dequeue(treatment);
			Q.dequeue(treatment);
			reqTreatments.enqueue(treatment);
		}
		else if (min == latency1)
		{
			Q.dequeue(treatment);
		}

		
	}
	else if (reqTreatments.getCount() == 3)
	{
		int min = eWaiting;
		if (uWaiting < min)
			min = uWaiting;
		if (xWaiting < min)
			min = xWaiting;
		Treatment* treatment = nullptr;
		LinkedQueue<Treatment*>Q;
		while (reqTreatments.dequeue(treatment))
		{
			if (treatment->getType() == 'E' && min == eWaiting)
			{
				break;
			}
			else if (treatment->getType() == 'U' && min == uWaiting)
			{
				break;
			}
			else if (treatment->getType() == 'X' && min == xWaiting)
			{
				break;
			}
			Q.enqueue(treatment);
		}
		Treatment* treatment2 = nullptr;
		while (Q.dequeue(treatment2))
		{
			reqTreatments.enqueue(treatment2);
		}
	}
}

void Patient::print(int count)
{
	const char * resetColor = "\033[0m";
	const char * greenColor = "\033[32m";
	const char * yellowColor = "\033[33m";
	
	cout << resetColor;
	if (type == 'N')
		cout << greenColor;
	else
		cout << yellowColor;

	
	// 0:idle, 1:early, 2:late, 3:wait, 4:serve, 5:finish

	if (status == 0) // idle
		cout << "P" << id << "_" << vt;
	else if (
		status == 1 || // early
		status == 2 || // late
		status == 3 || // wait
		status == 5	 // finish
		)
		cout << "P" << id;
	else			// serve
	{
		Treatment * treatment = nullptr;
		reqTreatments.peek(treatment);
		//if (!treatment || !treatment->getResource()) // no resource yet in phase 1.2
		if (!treatment)
		{
			cout << resetColor;
			return;
		}

		Resource * resource = treatment->getResource();
		
		cout << "P" << id << "_" << treatment->getType() << "#"; // '#' no resource is assigned to the treatment yet (phase 1.2)
		//resource->print();
	}

	cout << resetColor;
}

Treatment * Patient::getTreatment()
{
	Treatment * treatment = nullptr;
	reqTreatments.peek(treatment);
	return treatment;
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