#include "Patient.h"
#include "Resource.h"
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

Treatment* Patient::chooseMinLatency(int eWaiting, int uWaiting, int xWaiting)
{
	Treatment* treatment = nullptr;
	if (reqTreatments.getCount() == 0)
		return treatment;
	if (reqTreatments.getCount() == 1)
	{
		reqTreatments.peek(treatment);
		return treatment;
	}
	if (reqTreatments.getCount() == 2)
	{
		
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

		if (min == latency1)
		{
			reqTreatments.dequeue(treatment);
			Q.enqueue(treatment);
			while (Q.dequeue(treatment))
				reqTreatments.enqueue(treatment);
			reqTreatments.peek(treatment);
			
		}
		else if (min == latency2)
		{
			Q.dequeue(treatment);
			reqTreatments.enqueue(treatment);
			reqTreatments.peek(treatment);
		}
		return treatment;
		
	}

	else if (reqTreatments.getCount() == 3)
	{
		if (eWaiting == uWaiting && uWaiting == xWaiting)
		{
			reqTreatments.peek(treatment);
			return treatment;
		}
		int min = eWaiting;
		if (uWaiting < min)
			min = uWaiting;
		if (xWaiting < min)
			min = xWaiting;
		
		LinkedQueue<Treatment*>Q;
		while (reqTreatments.peek(treatment))
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
			reqTreatments.dequeue(treatment);
			Q.enqueue(treatment);
		}
		
		while (Q.dequeue(treatment))
		{
			reqTreatments.enqueue(treatment);
		}
		reqTreatments.peek(treatment);
		return treatment;
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
		if (!treatment || !treatment->getResource())
		{
			cout << resetColor;
			return;
		}

		Resource * resource = treatment->getResource();
		
		cout << "P" << id << "_" << treatment->getType() << resource->getId();
	}

	cout << resetColor;
}

Treatment * Patient::getTreatment()
{
	Treatment * treatment = nullptr;
	reqTreatments.peek(treatment);
	return treatment;
}

void Patient::toOutFile(ofstream & out)
{
	out << id << '\t';
	out << type << '\t';
	out << pt << '\t';
	out << vt << '\t';
	out << ft << '\t';
	out << wt << '\t';
	out << tt << '\t';
	
	if (cancelState)
		out << 'T' << '\t';
	else
		out << 'F' << '\t';

	if (rescState)
		out << 'T' << '\t';
	else
		out << 'F' << '\t';

	out << endl;
}