#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	// phase 1.2
	bool reschedule(int& maxPt) {
		Patient* patient = nullptr;
		if (!this->count)
			return false;

		int temp = 0;
		int index = rand() % this->count;
		if (!index) {
			this->dequeue(patient, temp);
			if (maxPt == patient->getPt())
				maxPt += 2;
			int npt = patient->getPt() + rand() % (maxPt - patient->getPt()) + 1;
			patient->setPt(npt);
			this->enqueue(patient, -npt);
			return true;

		}
		priNode<T>* ptr = this->head;
		for (int i = 0; i < index - 1; i++) {
			ptr = ptr->getNext();
		}

		priNode<T>* ptr2 = ptr->getNext();
		ptr->setNext(ptr2->getNext());
		ptr2->setNext(nullptr);
		patient = ptr2->getItem(temp);
		if (patient->getPt() == maxPt)
			maxPt += 2;
		int npt = patient->getPt() + rand() % (maxPt - patient->getPt()) + 1;
		patient->setPt(npt);
		this->count--;
		this->enqueue(patient, -npt);

		return true;
	}
	// phase 2
	bool reschedule2(int& maxPt, int Presc)
	{
		Patient* patient = nullptr;
		if (!this->count)
			return false;
		int resc = rand() % 100;
		if (resc < Presc)
		{
			int temp = 0;
			int index = rand() % this->count;
			if (!index) {
				this->peek(patient, temp);
				if (maxPt == patient->getPt())
					maxPt += 10;
				else if (maxPt < patient->getPt())
					maxPt = patient->getPt() + 10;
				int npt = rand() % maxPt;
				if (npt <= patient->getPt())
					return false;
				this->dequeue(patient, temp);
				patient->setPt(npt);
				this->enqueue(patient, -npt);
				return true;

			}
			priNode<T>* ptr = this->head;
			for (int i = 0; i < index - 1; i++) {
				ptr = ptr->getNext();
			}

			priNode<T>* ptr2 = ptr->getNext();
			patient = ptr2->getItem(temp);
			if (patient->getPt() == maxPt)
				maxPt += 10;
			else if (maxPt < patient->getPt())
				maxPt = patient->getPt() + 10;
			int npt = rand() % maxPt;
			if (npt <= patient->getPt())
				return false;
			ptr->setNext(ptr2->getNext());
			ptr2->setNext(nullptr);
			patient->setPt(npt);
			this->count--;
			this->enqueue(patient, -npt);

			return true;
		}
		else
			return false;
	}
};