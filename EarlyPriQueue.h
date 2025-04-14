#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
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
};